#ifndef CODEMGR_H
#define CODEMGR_H


#include <string>
#include <vector>
#include <map>


#define MEMORY_WORD_MAX (32 * 1024)


class CodeMgr
{
public:
    static CodeMgr& getInstance();
    void clearFunc(std::string funcName);
    void appendFunc(std::string funcName, std::string codeStr);
    void appendFuncAsm(std::string funcName, std::string codeStr);
    void checkFuncExist(std::string funcName) const;
    
    void pushGetchar(std::string funcName);
    
    void pushLocalVarOffset(std::string funcName, int innerOffset);
    void pushGlobalVarOffset(std::string funcName, int innerOffset);
    
    void pushLocalVarValue(std::string mFunctionName, int offset);
	void pushGlobalVarValue(std::string mFunctionName, int offset);
	void popToLocalVar(std::string mFunctionName, int offset);
	void popToGlobalVar(std::string mFunctionName, int offset);
	void popToArrayVar(std::string funcName);
	void PopToVarAt(std::string mFunctionName);
	void swapStack(std::string mFunction, int i, int j);
    
    void clearStack(std::string nowFuncName, int nowCnt);
    void callFunction(std::string nowFuncName, std::string nextFuncName);
    void jumpEndIf(std::string mFunctionName, int ifIdNow);
    void jzEndIf(std::string mFunctionName, int ifIdNow);
    void setEndIf (std::string mFunctionName, int ifIdNow);
    void setWhileBegin(std::string mFunctionName, int whileId);
    void checkAndJumpEndWhile(std::string mFunctionName, int whileId);
    void backToWhileBegin(std::string mFunctionName, int whileId);
    void setEndWhile(std::string mFunctionName, int whileId);
    void jzElse (std::string mFunctionName, int ifIdNow);
    void setElse  (std::string mFunctionName, int ifIdNow);
    void setGlobalString(std::string mFunctionName, int offset, std::string realString);
    void setGlobalVar(std::string varName, int length);
    void pushConstant(std::string mFunctionName, int value);
    void pushAX(std::string mFunctionName);
    void outputStackTop(std::string funcName);
    
    void getVarAtStackTop(std::string mFunctionName);
    void getNotStackTop(std::string mFunctionName);
    void getArrayElementValue(std::string funcName);
    void testStackTop(std::string mFunctionName);
    void sendTopToAx(std::string mFunctionName);
    void jumpReturn(std::string mFunctionName);
    void jumpEndWhile(std::string funcName, int whileId);
    
    void addStackTop(std::string funcName);
    void subStackTop(std::string funcName); 
    void mulStackTop(std::string funcName); 
    void divStackTop(std::string funcName); 
    void modStackTop(std::string funcName); 
    
    void lssStackTop(std::string funcName);
    void leqStackTop(std::string funcName); 
    void gtrStackTop(std::string funcName); 
    void geqStackTop(std::string funcName); 
    void equStackTop(std::string funcName); 
    void neqStackTop(std::string funcName); 
    void andStackTop(std::string funcName); 
    void orStackTop (std::string funcName); 
    
    void negStackTop(std::string funcName);
    
    void MOV(std::string funcName, std::string regTo, std::string regFrom);
    void memset(std::string mFunctionName, int value);
    
    void outputCode(int stackSegmentSizeWord = 1024);
    
protected:
    std::string& getFuncCode(std::string funcName);
    
    CodeMgr() {stackSegmentLengthWord = 0;}
    
    std::map<std::string, std::string> mFuncCode;
    
    std::string stackSegment;
    int stackSegmentLengthWord;
};


#endif

