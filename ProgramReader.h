#ifndef PROGRAMREADER_H
#define PROGRAMREADER_H


#include <map>
#include <string>
#include <vector> // Current implementation is inefficient
#include <stack>


struct Token {
public:
    Token(int tp, int cl, std::string rw): type(tp), col(cl), raw(rw){}
            int type; // record the class of the token
            int col ; // record the column of the token
    std::string raw ; // the raw message from the program
    void debugOutput() const;
};


typedef std::vector<Token> TokenList;      // the list of token in a single line


class ProgramReader
{
public:
	static ProgramReader& getInstance();
	static ProgramReader& newInstance();
	static void clearAllInstance();
	static void clearInstance();
	static std::stack<ProgramReader*> objStack;
	static std::map<std::string, bool> sFileCompiled; // check if a file is compiled
	static bool fileInStack(std::string fileName);
	
	void open(std::string fileName);
	void matchBrace();                     // match BEGIN and END
	std::string getLine(int line);
	void debugOutput();                    // this function only used when testing
	void debugParser();                    // output all the tokens
	void debugBrace();                     // output all the brace match
	void compile();                        // compile the whole file 
	void compileFunction(int lineFrom);
	void compileVar(int lineFrom);         // check mFunctionName = "" means global var
	void compileAsm(int lineFrom);
	void compileLine(int lineFrom);
	void compileIf(int lineFrom);
	void compileAssign(int lineFrom);
	void compileCall(int lineFrom);
	void compileWhile(int lineFrom);
	void compileFor(int lineFrom);
	void compileImport(int lineNow);
	
	void compileReturn(int lineFrom);
	void compilePutchar(int lineFrom);
	int  getLineNow();
	int  getColumnNow();
	
	std::string getFileName() const;
	
protected:
    void matchExpression();                // important part of evaluate
	void matchIdentifierExpression(int&);
	int matchConstInt();
    
    ProgramReader(): mLineId(-1), mTokenId(-1) {}
    void parseNewline();                   // parse the last line and append to mTokenTable
    
    std::string mFileName;
    std::vector<std::string> mProgramLine; // Record All program Lines
    std::vector  <TokenList> mTokenTable ; // Record ALL tokens from parser
    
    std::vector        <int> mMainPointer; // ENDIF -> IF, BREAK -> WHILE ...
    std::vector <std::vector<int> > mSubPointer; // IF -> ELSE, IF -> ENDIF
    
    int mLineId;
    int mTokenId;
    int mLineNow;
    
    std::string mFunctionName;             // the function which is being compiled
    
    std::stack<int> whileStack;
    std::map<int, int> mGetLineIdByWhileId;// get line id by while id 
    
    void openLine(int lineId);             // open a line and begin to read tokens 
	const Token& getToken();               // get a token
	void nextToken();                      // skip current token
	void backToken();                      // giveback current token
	const Token& match(int type, 
        std::string, bool jump = true);    // match a certain type of token
        
    void compute(std::stack<int>& opeStack);                        // pop an operator from stack
};


#endif

