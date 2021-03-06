#include <algorithm>
#include <cstdio>
#include <sstream>


#include "ErrorReport.h"
#include "KeywordMap.h"
#include "Utils.h"


bool Utils::isUpper(char c) {
    return 'A' <= c && c <= 'Z';
}


bool Utils::isLower(char c) {
    return 'a' <= c && c <= 'z';
}


bool Utils::isLetter(char c) {
    return isUpper(c) || isLower(c) || c == '_';
}


bool Utils::isDigit(char c) {
    return '0' <= c && c <= '9';
}


bool Utils::isEmpty(char c) {
    return 1 <= c && c <= 32;
}


bool Utils::isSingleOpe(char c) {
    return 
        c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
        c == ',' || c == '(' || c == ')' || c == '[' || c == ']' || c == '=';
}


int Utils::getSingleOpeId(char c) {
    switch(c) {
        case '+': return TOKEN_ADD;
        case '-': return TOKEN_SUB;
        case '*': return TOKEN_MUL;
        case '/': return TOKEN_DIV;
        case '%': return TOKEN_MOD;
        case ',': return TOKEN_COMMA;
        case '(': return TOKEN_OPEN;
        case ')': return TOKEN_CLOSE;
        case '[': return TOKEN_INDEXOPEN;
        case ']': return TOKEN_INDEXCLOSE;
        case '=': return TOKEN_EQU;
        default:
            ErrorReport::getInstance().send(
                true,
                "Inner Error",
                std::string("in Utils::getSingleOpeId, char '") + c + "' is not a single operator"
            );
    }
    return -1;
}


bool Utils::isCompare(char c) {
    return c == '=' || c == '<' || c == '>';
}


bool Utils::isOpenType(int type) {
    return type == TOKEN_FUNC || type == TOKEN_IF || type == TOKEN_WHILE || type == TOKEN_FOR;
}


bool Utils::isCloseType(int type) {
    return 
        type == TOKEN_ENDFUNC || type == TOKEN_ENDIF || type == TOKEN_ENDWHILE ||
        type == TOKEN_ENDFOR;
}


int Utils::getPairType(int type) {
    switch(type) {
        case TOKEN_FUNC: case TOKEN_ENDFUNC:
            return TOKEN_FUNC + TOKEN_ENDFUNC - type;
        case TOKEN_IF: case TOKEN_ENDIF:
            return TOKEN_IF + TOKEN_ENDIF - type;
        case TOKEN_WHILE: case TOKEN_ENDWHILE:
            return TOKEN_WHILE + TOKEN_ENDWHILE - type;
        case TOKEN_FOR: case TOKEN_ENDFOR:
            return TOKEN_FOR + TOKEN_ENDFOR - type;
        default:
            ErrorReport::getInstance().send(
                true,
                "Inner Error",
                "Utils::getPairType type'" + std::to_string(type) + "' is not open or close type"
            );
    }
    return -1;
}


bool Utils::isIntegerOperator(int type) {
    return 
        type == TOKEN_ADD || type == TOKEN_SUB || type == TOKEN_MUL || 
        type == TOKEN_DIV || type == TOKEN_MOD ||
        type == TOKEN_LSS || type == TOKEN_LEQ || type == TOKEN_NEQ ||
        type == TOKEN_GTR || type == TOKEN_GEQ || type == TOKEN_EQU ||
        type == TOKEN_AND || type == TOKEN_OR;
}


int Utils::priority(int type) {
    switch(type) {
        case TOKEN_OPEN: return 0;
        case TOKEN_AND :
        case TOKEN_OR  : return 1;
        case TOKEN_NEQ :
        case TOKEN_LSS :
        case TOKEN_LEQ :
        case TOKEN_GTR :
        case TOKEN_GEQ :
        case TOKEN_EQU : return 2;
        case TOKEN_ADD : 
        case TOKEN_SUB : return 3;
        case TOKEN_MUL :
        case TOKEN_DIV :
        case TOKEN_MOD : return 4;
        default:
            ErrorReport::getInstance().send(
                true,
                "Inner Error",
                "Utils::priority type '" + std::to_string(type) + "' is not interger operator"
            );
    }
    return -1;
}


char Utils::getTransChar(char ch, int& flag) {
    flag = 0;
    switch(ch) {
        case '0': return 0;
        case 'a': return 7;
        case 'b': return 8;
        case 'f': return 12;
        case 'n': return 10;
        case 'r': return 13;
        case 't': return 9;
        case 'v': return 11;
        case '\\': return '\\';
        case '\'': return '\'';
        case '\"': return '\"';
        default:
            flag = 1;
    }
    return 0;
}


std::string Utils::getRealString(std::string raw, int line, int col) {
    std::string realStr = "";
    for(int i = 1; i < raw.length() - 1; i ++) {
        if(raw[i] != '\\') {
            realStr += raw[i]; // this is just a normal string
        }else {
            i ++;
            int flag = 0;
            if(raw[i] == 'x') {
                if(i + 2 < raw.length() - 1) {
                    char hexChar[3] = {raw[i+1], raw[i+2], 0};
                    int val;
                    if(!checkHex(hexChar)) {
                        ErrorReport::getInstance().send(
                            true,
                            "Lexical Error",
                            " after '\\x' there should be two HEX digit",
                            line,
                            col + i
                        );
                    }
                    sscanf(hexChar, "%x", &val);
                    realStr += (char)val;
                }else {
                    ErrorReport::getInstance().send(
                        true,
                        "Lexical Error",
                        "'\\x' need two char after it, but get '" + std::to_string(raw.length() - 2 - i) + "'",
                        line,
                        col + i
                    );
                }
                i += 2;
            }else {
                char ch = getTransChar(raw[i], flag);
                if(!flag) {
                    realStr += ch;
                }
            }
        }
    }
    return realStr;
}


std::string Utils::fillStrTo(std::string strNow, int length) {
    while(strNow.length() < length) {
        strNow += " ";
    }
    return strNow;
}


bool Utils::isHexChar(char ch) {
    return Utils::isDigit(ch) || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F');
}


bool Utils::checkHex(char *hexChar) {
    for(int i = 0; hexChar[i]; i ++) {
        if(!isHexChar(hexChar[i])) {
            return false;
        }
    }
    return true;
}


std::string Utils::getOpe(std::string ope) {
    ope = deleteRem(ope);
    char tmp[256] = {};
    sscanf(ope.c_str(), "%s", tmp);
    std::string ans = tmp;
    std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
    return Utils::strip(ans);
}


std::string Utils::getReg1(std::string instruction) {
    char tmp[256];
    // sscanf(instruction.c_str(), "%*s%s", tmp);
    // std::string ans = tmp;
    instruction = deleteRem(instruction);
    std::stringstream ss; ss << instruction;
    ss >> tmp;
    std::string ans;
    std::getline(ss, ans, '\n');
    std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
    int commaPos = ans.find(",");
    if(commaPos != -1) {
        ans = ans.substr(0, commaPos);
    }
    return Utils::strip(ans);
}


std::string Utils::getReg2(std::string ins) {
    ins = deleteRem(ins);
    int pos = ins.find(",");
    if(pos == -1) {
        return ""; // there is no reg2
    }
    std::string reg2 = ins.substr(pos + 1);
    return strip(reg2);
}


std::string Utils::strip(std::string str) {  // delete the empty char
    while(str.size() > 0 && isEmpty(str[0])) {
        str = str.substr(1);
    }
    while(str.size() > 0 && isEmpty(str[str.length() - 1])) {
        str = str.substr(0, str.length() - 1);
    }
    return str;
}


std::string Utils::deleteRem(std::string instruction) {
    int semipos = instruction.find(";");
    if(semipos != -1) {
        instruction = instruction.substr(0, semipos);
    }
    return instruction;
}


bool Utils::isJmpFlag(std::string ins) {
    ins = deleteRem(ins);
    return ins.find(":") != -1; // "FLAG_XXX:" is regarded as flag
}


bool Utils::checkAffectReg(std::string ins, std::string reg) {
    ins = deleteRem(ins);
    std::string ope = getOpe(ins);
    if(ope == "PUSH" || ope == "CALL") {
        if(reg == "SP") {
            return true;
        }else {
            return false; // push does not change the value of any reg except sp
        }
    }
    if(ope[0] == 'J') {   // jump instruction don't change any common 16 register
        return false;
    }
    if(ope == "DIV" || ope == "IDIV" || ope == "MUL" || ope == "IMUL") {
        if(reg == "AX" || reg == "DX") {
            return true;
        }else {
            return false;
        }
    }else {
        bool part = getReg1(ins) == reg;
        return part;
    }
}


bool Utils::checkUseReg(std::string ins, std::string dst) { // read or write a register
    ins = deleteRem(ins);
    std::string Rdst = getReg1(ins);
    std::string Rsrc = getReg2(ins);
    return checkAffectReg(ins, dst) || Rdst == dst || Rsrc == dst;
}


bool Utils::isReg16Name(std::string regName) {
    return 
        regName == "AX" || regName == "BX" || regName == "CX" || regName == "DX" ||
        regName == "SI" || regName == "DI" || regName == "SP" || regName == "BP"; 
}


bool Utils::isImm(std::string str) {
    if(str[0] == '-') {
        str[0] = ' ';
        str = strip(str);
    }
    if(str == "") return false;
    else {
        return '0' <= str[0] && str[0] <= '9';
    }
}

