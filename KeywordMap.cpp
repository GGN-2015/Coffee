#include "KeywordMap.h"


KeywordMap& KeywordMap::getInstance() {
    static KeywordMap kw;
    return kw;
}


int KeywordMap::getKeywordId(std::string keywordName) {
    if(mKeywordId.count(keywordName)) {
        return mKeywordId[keywordName];
    }
    return TOKEN_NOT_KEYWORD; // -1 means not found
}

