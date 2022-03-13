#ifndef PARSER_H
#define PARSER_H


#include <string>


class Parser
{
public:
	Parser(std::string stringValue);
	char getChar();
	void backChar();
	void nextChar();
	void reset();
	int  getColumn();
	bool eos();
	
	std::string getIdentifier();
	std::string getNumber();
	std::string getString(int lineId);
	void jumpSpace();
	void testChar(char ch, int lineId);
	
protected:
    int mCharNow;
    std::string mStringValue;
};

#endif
