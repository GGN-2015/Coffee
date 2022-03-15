#ifndef UTILS_H
#define UTILS_H


#include <string>


class Utils
{
public:
    static bool isUpper(char c);
    static bool isLower(char c);
    static bool isLetter(char c);
    static bool isDigit(char c);
    static bool isEmpty(char c);
    static bool isCompare(char c);
    static bool isSingleOpe(char c); // the operator with only a single char
    static int  getSingleOpeId(char c);
    static std::string getRealString(std::string raw, int line, int col);
    static char getTransChar(char ch, int& flag);
    static std::string fillStrTo(std::string strNow, int length);
    
    static bool isOpenType(int type);
    static bool isCloseType(int type);
    static int  getPairType(int type);
    
    static bool isIntegerOperator(int type);
    static int priority(int type);
    
    static bool checkHex(char *hexChar);
    static bool isHexChar(char ch);
};

#endif
