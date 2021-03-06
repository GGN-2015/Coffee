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
    static std::string deleteRem(std::string instruction);
    static std::string getOpe(std::string instruction);
    static std::string getReg1(std::string instruction);
    static std::string getReg2(std::string instruction);
    static std::string strip(std::string str);
    static bool isJmpFlag(std::string ins);
    static bool checkAffectReg(std::string ins, std::string dst);
    static bool checkUseReg   (std::string ins, std::string dst);
    static bool isReg16Name(std::string regName);
    
    static bool isOpenType(int type);
    static bool isCloseType(int type);
    static int  getPairType(int type);
    
    static bool isIntegerOperator(int type);
    static int priority(int type);
    
    static bool checkHex(char *hexChar);
    static bool isHexChar(char ch);
    static bool isImm(std::string str);
};

#endif
