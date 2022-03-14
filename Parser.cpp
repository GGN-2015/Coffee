#include "ErrorReport.h"
#include "Parser.h"
#include "Utils.h"



Parser::Parser(std::string stringValue)
{
    mStringValue = stringValue;
    mCharNow = 0;
}


char Parser::getChar() {
    if(0 <= mCharNow && mCharNow < (int)mStringValue.length()) {
        return mStringValue[mCharNow];
    }else {
        return 0;
    }
}


void Parser::backChar() {
    if(mCharNow > 0) {
        mCharNow --;
    }
}


void Parser::nextChar() {
    if(0 <= mCharNow && mCharNow < (int)mStringValue.length()) {
        mCharNow ++; 
    }
}


void Parser::reset() {
    mCharNow = 0;
}


int Parser::getColumn() {
    return mCharNow;
}


bool Parser::eos() {
    return mCharNow >= (int)mStringValue.length();
}


void Parser::jumpSpace() {
    while(Utils::isEmpty(getChar())) {
        nextChar();
    }
}


std::string Parser::getIdentifier() {
    std::string ans = "";
    while(Utils::isLetter(getChar()) || Utils::isDigit(getChar())) {
        ans += getChar();
        nextChar();
    }
    return ans;
}


std::string Parser::getNumber() {
    std::string ans = "";
    while(Utils::isDigit(getChar())) {
        ans += getChar();
        nextChar();
    }
    return ans;
}


std::string Parser::getString(int lineId) {
    std::string ans = "";
    ans += getChar(); nextChar();
    while(true) {
        if(getChar() == 0) {
            ErrorReport::getInstance().send(
                true,
                "Lexical Error",
                "Quote is not close",
                lineId,
                getColumn()
            );
        }
        char ch = getChar();
        char lastchar = ans[ans.size() - 1];
        ans += ch;
        nextChar();
        if(ch == ans[0] && lastchar != '\\') {
            break;
        }
    }
    return ans;
}


void Parser::testChar(char ch, int lineId) {
    if(getChar() == ch) {
        nextChar();
    }else {
        ErrorReport::getInstance().send(
            true,
            "Lexical Error",
            std::string("Expected Char '") + ch + "' but get '" + getChar() + "' instead",
            lineId,
            getColumn()
        );
    }
}

