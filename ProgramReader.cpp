#include <cstdio>
#include <cstdlib>
#include <stack>


#include "CodeMgr.h"
#include "ErrorReport.h"
#include "FuncGraph.h"
#include "KeywordMap.h"
#include "ProgramReader.h"
#include "Parser.h"
#include "Utils.h"
#include "VarMgr.h"


static int ifCnt     = 0;
static int stringCnt = 0;
static int whileCnt  = 0;


std::map<std::string, bool> ProgramReader::sFileCompiled;
std::stack<ProgramReader*>  ProgramReader::objStack;


bool ProgramReader::fileInStack(std::string fileName) {
    bool flag = false;
    std::stack<ProgramReader*> tmpStack;
    while(!objStack.empty()) { // check all the data in tmpStack
        ProgramReader* pr = objStack.top();
        if(pr -> mFileName == fileName) {
            flag = true;
        }
        tmpStack.push(pr);
        objStack.pop();
    }
    while(!tmpStack.empty()) { // push data back to objStack
        ProgramReader* pr = tmpStack.top();
        objStack.push(pr);
        tmpStack.pop();
    }
    return flag;
}


int ProgramReader::getColumnNow() {
    if(mTokenId < mTokenTable[mLineId].size()) {
        return mTokenTable[mLineId][mTokenId].col;
    }else {
        return 0;
    }
}


int ProgramReader::getLineNow() {
    return mLineNow;
}


ProgramReader& ProgramReader::newInstance() {
    objStack.push(new ProgramReader);
    return *objStack.top();
}


void ProgramReader::clearInstance() {
    if(!objStack.empty()) {
        delete objStack.top();
        objStack.pop();
    }else {
        ErrorReport::getInstance().send(
            true, 
            "Inner Error",
            "ProgramReader::clearInstance when objStack is empty"
        );
    }
}


void ProgramReader::clearAllInstance() {
    while(!objStack.empty()) {
        delete objStack.top();
        objStack.pop();
    }
}


std::string ProgramReader::getFileName() const {
    return mFileName;
}


void Token::debugOutput() const {
    printf("[%5d %5d %20s]\n", col, type, raw.c_str());
}


ProgramReader& ProgramReader::getInstance() {
    if(objStack.empty()) {
        objStack.push(new ProgramReader);
    }
    return *objStack.top();
}


void ProgramReader::open(std::string fileName) { // open a file and read all the text
    mProgramLine.clear();
    mFileName = fileName;
    FILE* fpin = fopen(fileName.c_str(), "r");
    if(fpin == NULL) {
        ErrorReport::getInstance().send(
            true, 
            "File Error", 
            "Can not open Source File " + fileName
        );
    }else {
        std::string lineString = "";
        while(true) {
            char ch = fgetc(fpin);
            if(ch == '\n' || ch == EOF) {
                mProgramLine.push_back(lineString);
                lineString = "";
                parseNewline();                      // parse the last line of the table
                if(ch == EOF) {
                    break;
                }
            }else {
                if(ch != '\r') lineString += ch;     // we do not record '\n' or '\r' in lineString
            }
        }
    }
    matchBrace();
}


void ProgramReader::matchBrace() {
    std::stack<int> stk;
    for(int i = 0; i < mProgramLine.size(); i ++) {
        Token& token = mTokenTable[i][0];
        if(Utils::isOpenType(token.type)) {  // open type
            stk.push(i);
        }else
        if(Utils::isCloseType(token.type)) { // close type
            if(stk.empty()) {
                ErrorReport::getInstance().send(
                    true,
                    "Syntax Error",
                    "'" + token.raw + "' can not find match",
                    i,
                    token.col
                );
            }
            int nowTop = stk.top();
            stk.pop();
            Token& from = mTokenTable[nowTop][0];
            if(Utils::getPairType(token.type) != from.type) {
                ErrorReport::getInstance().send(
                    true,
                    "Syntax Error",
                    "'" + token.raw + "' can not find match with '" + from.raw + 
                        "' on line " + std::to_string(nowTop),
                    i,
                    token.col
                );
            }else {
                mMainPointer[i] = nowTop;
                mSubPointer[nowTop].push_back(i);
            }
        }else
        if(token.type == TOKEN_ELSE) { // ELSE is sth like 'MID_TYPE'
            int nowTop = stk.top();
            Token& from = mTokenTable[nowTop][0];
            if(from.type != TOKEN_IF) {
                ErrorReport::getInstance().send(
                    true,
                    "Syntax Error",
                    "'" + token.raw + "' can not match with '" + from.raw + 
                        "' on line " + std::to_string(nowTop),
                    i,
                    token.col
                );
            }else {
                mMainPointer[i] = nowTop;
                mSubPointer[nowTop].push_back(i);
            }
        }else
        if(token.type == TOKEN_BREAK || token.type == TOKEN_CONTINUE) {
            std::stack<int> stk2;
            while(!stk.empty() 
                && mTokenTable[stk.top()][0].type != TOKEN_WHILE
                && mTokenTable[stk.top()][0].type != TOKEN_FOR
            ) {
                stk2.push(stk.top());
                stk.pop();
            }
            if(stk.empty()) { // WHILE statement not found 
                ErrorReport::getInstance().send(
                    true,
                    "Syntax Error",
                    "'" + token.raw + "' can not find WHILE or FOR"
                );
            }else {
                int nowTop = stk.top();
                mMainPointer[i] = nowTop;
                mSubPointer[nowTop].push_back(i);
                while(!stk2.empty()) {
                    stk.push(stk2.top()); // regain the value of stack
                    stk2.pop();
                }
            }
        }
    }
    if(!stk.empty()) {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "open type is more than close type, check your begin and end"
        );
    }
    for(int i = 0; i < mProgramLine.size(); i ++) {
        if(mSubPointer[i].size() >= 3) {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "'IF' has more than one ELSE such as on line " + 
                    std::to_string(mSubPointer[i][0]) + " and " + std::to_string(mSubPointer[i][1]),
                i,
                mTokenTable[i][0].col
            );
        }
    }
}


std::string ProgramReader::getLine(int line) {
    if(0 <= line && line < mProgramLine.size()) {
        return mProgramLine[line];
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "The Program has " + 
                std::to_string(mProgramLine.size()) + 
                " lines, but you use line " + std::to_string(line)
        );
    }
}


void ProgramReader::debugOutput() {
    printf("ProgramReader DEBUG: TOTAL %d LINES\n", (int)mProgramLine.size());
    for(int i = 0; i < (int)mProgramLine.size(); i ++) {
        printf("%5d | %s\n", i, mProgramLine[i].c_str());
    }
    printf("\n");
}


void ProgramReader::debugBrace() {
    for(int i = 0; i < mProgramLine.size(); i ++) {
        printf("%5d | ", i + 1);
        if(mMainPointer[i] != -1) {
            printf("%10s, Main: %5d", mTokenTable[i][0].raw.c_str(), mMainPointer[i] + 1);
        }
        if(mSubPointer[i].size()) {
            printf("%10s, Sub: ", mTokenTable[i][0].raw.c_str());
            for(int sub: mSubPointer[i]) {
                printf("%5d ", sub + 1);
            }
        }
        printf("\n");
    }
}


void ProgramReader::debugParser() {
    printf("ProgramReader DEBUG: TOTAL %d LINES\n", (int)mProgramLine.size());
    for(int line = 0; line < (int)mProgramLine.size(); line ++) {
        for(int i = 0; i < (int)mTokenTable[line].size(); i ++) {
            mTokenTable[line][i].debugOutput();
        }
        printf("\n");
    }
    printf("\n");
}


void ProgramReader::parseNewline() {
    TokenList tokenListTmp;
    int lineId = mProgramLine.size() - 1;
    Parser ps(mProgramLine[lineId]);
    mMainPointer.push_back(-1); // no main pointer
    mSubPointer.push_back({});  // no sub pointer
    do {
        ps.jumpSpace();
        char ch  = ps.getChar();
        int  col = ps.getColumn();
        if(ch == 0 || ch == ';') {
            tokenListTmp.push_back(Token(TOKEN_ENDOFLINE, col, "<END_OF_LINE>"));
            break;                                  // every line has a token EOL on it
        }
        if(Utils::isLetter(ch)) {                   // read in identifier or keyword
            std::string iden = ps.getIdentifier();
            int keywordId = KeywordMap::getInstance().getKeywordId(iden);
            if(keywordId > 0) {                     // this is a keyword 
                tokenListTmp.push_back(Token(keywordId, col, iden));
            }else {                                 // this is an iden
                tokenListTmp.push_back(Token(TOKEN_IDENTIFIER, col, iden));
            }
        }else
        if(Utils::isDigit(ch)) {                    // this is a number 
            std::string num = ps.getNumber();
            tokenListTmp.push_back(Token(TOKEN_NUMBER, col, num));
        }else 
        if(ch == ':') {                             // the only reason is ':='
            ps.nextChar();
            ps.testChar('=', lineId);
            tokenListTmp.push_back(Token(TOKEN_ASS, col, ":="));
        }else
        if(ch == 34 || ch == 39) {                  // get a string
            std::string str = ps.getString(lineId);
            tokenListTmp.push_back(Token(TOKEN_STRING, col, str));
        }else
        if(Utils::isSingleOpe(ch)) {
            ps.nextChar();
            int tokenId = Utils::getSingleOpeId(ch);
            tokenListTmp.push_back(Token(tokenId, col, std::string("") + ch));
        }else
        if(Utils::isCompare(ch) && ch != '=') {     // <, <=, >, >=, <>
            ps.nextChar();
            if(ch == '<') {
                if(ps.getChar() == '=') {
                    ps.nextChar();
                    tokenListTmp.push_back(Token(TOKEN_LEQ, col, "<="));
                }else
                if(ps.getChar() == '>') {
                    ps.nextChar();
                    tokenListTmp.push_back(Token(TOKEN_NEQ, col, "<>"));
                }else {
                    tokenListTmp.push_back(Token(TOKEN_LSS, col, "<"));
                }
            }else { // ch == '>'
                if(ps.getChar() == '=') {
                    ps.nextChar();
                    tokenListTmp.push_back(Token(TOKEN_GEQ, col, ">="));
                }else {
                    tokenListTmp.push_back(Token(TOKEN_GTR, col, ">"));
                }
            }
        }else
        {
            ErrorReport::getInstance().send(
                true,
                "Lexical Error",
                "Unknow char ASCII " + std::to_string(ch) + " appear in program",
                lineId,
                col
            );
        }
    } while(true);
    mTokenTable.push_back(tokenListTmp);
}


void ProgramReader::openLine(int lineId) {
    mLineId  = lineId;
    mTokenId = 0;
    if(!(0 <= lineId && lineId < mTokenTable.size())) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "ProgramReader::openLine lineId " + std::to_string(lineId) + " does not exist"
        );
    }
}


const Token& ProgramReader::getToken() {
    return mTokenTable[mLineId][mTokenId];
}


void ProgramReader::nextToken() {
    //fprintf(stderr, "JUMP %s\n", getToken().raw.c_str());
    if(mTokenId < mTokenTable[mLineId].size()) {
        mTokenId ++;
    }
}


void ProgramReader::backToken() {
    if(mTokenId > 0) {
        mTokenId --;
    }
}


void ProgramReader::compileImport(int lineNow) {
    mLineNow = lineNow;
    openLine(mLineNow);
    const Token& tokenImport = match(TOKEN_IMPORT, "IMPORT"); 
    const Token& tokenStr = match(TOKEN_STRING, "STRING");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    std::string fileName = Utils::getRealString(tokenStr.raw, mLineNow, tokenStr.col);
    if(!sFileCompiled[fileName]) {
        // if the file has been compiled, ignore this import
        if(!fileInStack(fileName)) { // check the file is in stack
            ProgramReader& pr = newInstance();
            pr.open(fileName);
            pr.compile();
            clearInstance();
        }else {
            ErrorReport::getInstance().send(
                true,
                "Import Error",
                "file '" +fileName+ "' is Imported again during another import.",
                mLineNow,
                tokenImport.col
            );
        }
    }
    mLineNow ++;
}


void ProgramReader::compile() {
    mLineNow = 0;
    while(mLineNow < mProgramLine.size()) {
        Token& firstToken = mTokenTable[mLineNow][0];
        if(firstToken.type == TOKEN_ENDOFLINE) { // jump empty line
            mLineNow ++;
        }else
        if(firstToken.type == TOKEN_VAR) {
            compileVar(mLineNow);
        }else 
        if(firstToken.type == TOKEN_FUNC) {
            compileFunction(mLineNow);
        }else 
        if(firstToken.type == TOKEN_IMPORT) {
            compileImport(mLineNow);
        }else
        {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "Expected 'FUNC' or 'VAR' or 'IMPORT' get '" + firstToken.raw + "'",
                mLineNow,
                firstToken.col
            );
        }
    }
    // the compile of this file is finished
    sFileCompiled[mFileName] = true;
}


int ProgramReader::matchConstInt() {
    int negflag = 0;
    if(getToken().type == TOKEN_SUB) {
        negflag = 1;
        match(TOKEN_SUB, "-");
    }
    const Token& tokenNum = match(TOKEN_NUMBER, "NUMBER");
    int ans = atoi(tokenNum.raw.c_str());
    if(negflag) {
        ans = - ans;
    }
    return ans;
}


void ProgramReader::compileVar(int lineFrom) {
    openLine(lineFrom);
    match(TOKEN_VAR, "VAR"); // default jump this token
    const Token* token = nullptr;
    const Token* value = nullptr;
    int length = 1;
take_on_vars:
    token = &match(TOKEN_IDENTIFIER, "IDENTIFIER");
    if(getToken().type == TOKEN_INDEXOPEN) {
        match(TOKEN_INDEXOPEN, "[");
        value = &match(TOKEN_NUMBER, "NUMBER");
        match(TOKEN_INDEXCLOSE, "]");
        if(VarMgr::getInstance().existFunc(token -> raw)) {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "Name '" + token -> raw + "' is already used as a function",
                lineFrom,
                token -> col
            );
        }
        VarMgr::getInstance().addVar( // get var token[value]
            mFunctionName, 
            token -> raw,
            length = atoi(value -> raw.c_str()),
            lineFrom,
            token -> col
        );
    }else {
        VarMgr::getInstance().addVar( // get var token (length = 1)
            mFunctionName, 
            token -> raw,
            length = 1,
            lineFrom,
            token -> col
        );
    }
    if(mFunctionName == "") { // register a global var in stack segment
        if(getToken().type == TOKEN_ASS) {
            match(TOKEN_ASS, ":=");
            int* arr = new int[length];
            int val;
            for(int i = 0; i < length; i ++) {
                arr[i] = 0;
            }
            int arguCnt = 0;
            if(length == 1) {
                val = matchConstInt();
                arr[0] = val;
                arguCnt = 1;
            }else { // length > 1
                
                match(TOKEN_INDEXOPEN, "[");
global_var_string_next_argu:
                val = matchConstInt();
                if(arguCnt >= length) {
                    ErrorReport::getInstance().send(
                        true,
                        "Semantic Error",
                        "Initialize list too long for string '" + token -> raw + "' ",
                        mLineNow,
                        getToken().col
                    );
                }
                arr[arguCnt ++] = val;
                if(getToken().type == TOKEN_COMMA) {
                    nextToken();
                    goto global_var_string_next_argu;
                }
                match(TOKEN_INDEXCLOSE, "]");
            }
            CodeMgr::getInstance().setGlobalVar(token -> raw, length, arr, arguCnt);
            delete[] arr;
        }else {
            CodeMgr::getInstance().setGlobalVar(token -> raw, length, nullptr, 0);
        }
    }
    if(getToken().type == TOKEN_ASS && mFunctionName != "") {
        int offset = VarMgr::getInstance().getVarOffset(mFunctionName, 
            token -> raw,
            mLineNow,
            token -> col
        );
        match(TOKEN_ASS, ":=");
        if(length == 1) {
            matchExpression();
            CodeMgr::getInstance().popToLocalVar(mFunctionName, offset);
        }else {
            CodeMgr::getInstance().pushLocalVarOffset(mFunctionName, offset + length - 1);
            CodeMgr::getInstance().pushConstant(mFunctionName, length);
            CodeMgr::getInstance().memset(mFunctionName, 0);
            int arguCnt = 0;
            match(TOKEN_INDEXOPEN, "[");
            if(getToken().type != TOKEN_INDEXCLOSE) {
get_array_member_init:
                CodeMgr::getInstance().pushLocalVarOffset(mFunctionName, offset + length - 1);
                CodeMgr::getInstance().pushConstant(mFunctionName, arguCnt);
                matchExpression();
                CodeMgr::getInstance().popToArrayVar(mFunctionName);
                arguCnt ++;   
                if(getToken().type == TOKEN_COMMA) {
                    match(TOKEN_COMMA, ",");
                    goto get_array_member_init;
                }
            }
            const Token& tokenClose = match(TOKEN_INDEXCLOSE, "]");
            if(arguCnt > length) {
                ErrorReport::getInstance().send(
                    true,
                    "Syntax Error",
                    "Too many initial value for array '" + token -> raw + "' ",
                    mLineNow,
                    tokenClose.col
                );
            }
        }
    }
    if(getToken().type == TOKEN_COMMA) {
        match(TOKEN_COMMA, ",");
        goto take_on_vars;
    }else 
    if(getToken().type == TOKEN_ENDOFLINE) {
        goto take_on_finish;
    }else {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "Expected ',' or 'END_OF_LINE' but get '" + getToken().raw + "'",
            mLineId,
            getToken().col
        );
    }
take_on_finish:
    mLineNow ++;
}


const Token& ProgramReader::match(int type, std::string strName, bool jump) {
    if(getToken().type == type) {
        const Token& token = getToken();
        if(jump) nextToken();
        return token;
    }else {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "Expected '" + strName + "' get '" + getToken().raw + "'",
            mLineId,
            getToken().col
        );
    }
}


void ProgramReader::compileFunction(int lineFrom) {
    openLine(lineFrom);
    match(TOKEN_FUNC, "FUNC");
    const Token* funcIden = &match(TOKEN_IDENTIFIER, "IDENTIFIER"); // get function name
    mFunctionName = funcIden -> raw;
    VarMgr::getInstance().addFunc(
        mFunctionName, lineFrom, funcIden -> col);                  // add function into list
    FuncGraph::getInstance().addNewFunc(mFunctionName);             // add function into funcGraph
    CodeMgr::getInstance().clearFunc(mFunctionName);                // clear do not check name twice
    
    match(TOKEN_OPEN, "(");
    int arguCnt = 0;
    if(getToken().type != TOKEN_CLOSE) {
        const Token* arguIden = nullptr;
get_new_argu:
        arguIden = &match(TOKEN_IDENTIFIER, "IDENTIFIER");
        VarMgr::getInstance().addVar(
            mFunctionName, arguIden -> raw, 1, lineFrom, arguIden -> col
        );
        arguCnt ++;
        if(getToken().type == TOKEN_COMMA) {
            match(TOKEN_COMMA, ",");
            goto get_new_argu;
        }
    }
    VarMgr::getInstance().setFuncArguCnt(mFunctionName, arguCnt);
    match(TOKEN_CLOSE, ")");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    int flag = false, endpos;
    for(endpos = lineFrom + 1; endpos < mProgramLine.size(); endpos ++) {
        if(mTokenTable[endpos][0].type == TOKEN_ENDFUNC) {
            // mLineNow = endpos + 1;
            flag = true;      // find the position that function end
            break;
        }
    }
    if(flag) {
        // printf("lineFrom = %d, End = %d\n", lineFrom, j); // delete this printf
        mLineNow = lineFrom + 1;
        while(mLineNow < endpos) {
            compileLine(mLineNow); // compile a basic bock or a line
        }
        mLineNow = endpos + 1;
        mFunctionName = "";        // remember to set this to ""
    }else {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "'FUNC' without 'ENDFUNC'",
            lineFrom,
            0
        );
    }
}


void ProgramReader::compileReturn(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    match(TOKEN_RETURN, "RETURN");
    if(getToken().type != TOKEN_ENDOFLINE) { // there is still something after RETURN
        matchExpression();
        match(TOKEN_ENDOFLINE, "END_OF_LINE");
        CodeMgr::getInstance().sendTopToAx(mFunctionName); // use AX to store the return value
    }else {                                                // RETURN 0
        CodeMgr::getInstance().MOV(mFunctionName, "AX", "0");
    }
    CodeMgr::getInstance().jumpReturn(mFunctionName);
    mLineNow ++;
}


void ProgramReader::compilePutchar(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom); // remember this
    match(TOKEN_PUTCHAR, "PUTCHAR");
    match(TOKEN_OPEN, "(");
    matchExpression();
    CodeMgr::getInstance().outputStackTop(mFunctionName);
    match(TOKEN_CLOSE, ")");
    mLineNow ++;
}


void ProgramReader::compileIf(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    int ifIdNow = ++ ifCnt;
    int siz = mSubPointer[lineFrom].size(); // siz = 2 means with ELSE, siz = 1 means no ELSE
    match(TOKEN_IF, "IF");
    matchExpression();                                   // important, push expression into CodeMgr
    CodeMgr::getInstance().testStackTop(mFunctionName);  // POP AX; AND AX, AX
    match(TOKEN_THEN, "THEN");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    mLineNow = lineFrom + 1;
    if(siz == 1) {
        int endIfLine = mSubPointer[lineFrom][0];
        CodeMgr::getInstance().jzEndIf(mFunctionName, ifIdNow);
        while(mLineNow < endIfLine) {
            compileLine(mLineNow);
        }
        CodeMgr::getInstance().setEndIf(mFunctionName, ifIdNow);
        openLine(mLineNow);
        match(TOKEN_ENDIF, "ENDIF");
        match(TOKEN_ENDOFLINE, "END_OF_LINE");
    }else {
        int elseLine  = mSubPointer[lineFrom][0];
        int endIfLine = mSubPointer[lineFrom][1];
        CodeMgr::getInstance().jzElse(mFunctionName, ifIdNow);
        while(mLineNow < elseLine) {
            compileLine(mLineNow);
        }
        openLine(mLineNow);
        match(TOKEN_ELSE, "ELSE");
        match(TOKEN_ENDOFLINE, "END_OF_LINE");
        CodeMgr::getInstance().jumpEndIf(mFunctionName, ifIdNow);
        CodeMgr::getInstance().setElse(mFunctionName, ifIdNow);
        mLineNow ++;
        while(mLineNow < endIfLine) {
            compileLine(mLineNow);
        }
        openLine(mLineNow);
        match(TOKEN_ENDIF, "ENDIF");
        match(TOKEN_ENDOFLINE, "END_OF_LINE");
        CodeMgr::getInstance().setEndIf(mFunctionName, ifIdNow);
    }
    mLineNow = mSubPointer[lineFrom][siz - 1] + 1; // endif
}


void ProgramReader::compileAssign(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    if(getToken().type == TOKEN_VARAT) {
        match(TOKEN_VARAT, "VARAT");
        match(TOKEN_OPEN, "(");
        matchExpression();
        match(TOKEN_CLOSE, ")");
        match(TOKEN_ASS, ":=");
        matchExpression();
        match(TOKEN_ENDOFLINE, "END_OF_LINE");
        CodeMgr::getInstance().PopToVarAt(mFunctionName);
        mLineNow ++;
    }else {
        const Token& tokenIden = match(TOKEN_IDENTIFIER, "IDENTIFIER");
        if(VarMgr::getInstance().existFunc(tokenIden.raw)) {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "You Can not Assign value to function '" + tokenIden.raw + "' use keyword 'CALL' to call it",
                mLineNow,
                tokenIden.col
            );
        }
        bool isLocal;
        int offset; // global var or local var is different
        int length;
        if(VarMgr::getInstance().existLocalVar(mFunctionName, tokenIden.raw)) {
            isLocal = true;
            offset  = VarMgr::getInstance().getVarOffset(
                mFunctionName, tokenIden.raw, mLineNow, tokenIden.col
            );
            length  = VarMgr::getInstance().getVarLength(
                mFunctionName, tokenIden.raw
            );
        }else {
            if(!VarMgr::getInstance().existGlobalVar(tokenIden.raw)) {
                ErrorReport::getInstance().send(
                    true,
                    "Semantic Error",
                    "Var '" + tokenIden.raw + "' not exist",
                    mLineNow, 
                    tokenIden.col
                );
            }
            isLocal = false; // global var, funcName = ""
            offset  = VarMgr::getInstance().getVarOffset(
                "", tokenIden.raw, mLineNow, tokenIden.col
            );
            length  = VarMgr::getInstance().getVarLength("", tokenIden.raw);
        }
        if(length == 1) {
            match(TOKEN_ASS, ":=");
            matchExpression();
            if(isLocal) {
                CodeMgr::getInstance().popToLocalVar(mFunctionName, offset);
            }else {
                CodeMgr::getInstance().popToGlobalVar(mFunctionName, offset);
            }
        }else {
            if(isLocal) {
                CodeMgr::getInstance().pushLocalVarOffset(mFunctionName, offset + length - 1);
            }else {
                CodeMgr::getInstance().pushGlobalVarOffset(mFunctionName, offset);
            }
            match(TOKEN_INDEXOPEN, "[");
            matchExpression();
            match(TOKEN_INDEXCLOSE, "]");
            match(TOKEN_ASS, ":=");
            matchExpression();
            CodeMgr::getInstance().popToArrayVar(mFunctionName);
        }
        mLineNow ++;
    }
}


void ProgramReader::compileCall(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    match(TOKEN_CALL, "CALL");
    const Token& tokenFunc = getToken(); nextToken();
    if(!VarMgr::getInstance().existFunc(tokenFunc.raw)) {
        ErrorReport::getInstance().send(
            true,
            "Semantic Error",
            "Function '" + tokenFunc.raw + "' is not defined",
            mLineNow,
            tokenFunc.col
        );
    }
    FuncGraph::getInstance().addEdge(mFunctionName, tokenFunc.raw); // add edge between functions
    
    int arguCnt = VarMgr::getInstance().getFuncArguCnt(tokenFunc.raw);
    int nowCnt = 0;
    match(TOKEN_OPEN, "(");
    if(getToken().type == TOKEN_CLOSE) {
        goto match_close;
    }
get_one_argu:
    matchExpression();
    nowCnt ++;
    if(getToken().type == TOKEN_COMMA) {
        nextToken();
        goto get_one_argu;
    }
match_close:
    const Token& tokenClose = match(TOKEN_CLOSE, ")");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    if(arguCnt != nowCnt) {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "function '' need " + std::to_string(arguCnt) + " paras but " + std::to_string(nowCnt) + " given",
            mLineNow,
            tokenClose.col
        );
    }
    for(int i = 0, j = arguCnt - 1; i < j; i ++, j --) {
        CodeMgr::getInstance().swapStack(mFunctionName, i, j);
    }
    CodeMgr::getInstance().callFunction(mFunctionName, tokenFunc.raw);
    CodeMgr::getInstance().clearStack(mFunctionName, nowCnt);
    mLineNow ++;
}


void ProgramReader::compileWhile(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    int whileId = ++ whileCnt;
    CodeMgr::getInstance().setWhileBegin(mFunctionName, whileId);
    match(TOKEN_WHILE, "WHILE");
    matchExpression();
    CodeMgr::getInstance().checkAndJumpEndWhile(mFunctionName, whileId);
    match(TOKEN_LOOP, "LOOP");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    int end = mSubPointer[lineFrom][mSubPointer[lineFrom].size() - 1];
    mLineNow ++;
    whileStack.push(whileId);
    mGetLineIdByWhileId[whileId] = lineFrom;
    while(mLineNow < end) {
        compileLine(mLineNow);
    }
    CodeMgr::getInstance().backToWhileBegin(mFunctionName, whileId);
    CodeMgr::getInstance().setEndWhile(mFunctionName, whileId);
    whileStack.pop(); 
    mLineNow = end + 1;
}


void ProgramReader::compileFor(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    int whileId = ++ whileCnt;
    match(TOKEN_FOR, "FOR");
    const Token& tokenIden = getToken(); nextToken();
    if(!VarMgr::getInstance().existLocalVar(mFunctionName, tokenIden.raw)) {
        VarMgr::getInstance().addVar(mFunctionName, tokenIden.raw, 1,
            mLineNow, tokenIden.col
        );
    }
    int offset = VarMgr::getInstance().getVarOffset( // loop var must be local var
        mFunctionName, tokenIden.raw, mLineNow, tokenIden.col
    );
    int length = VarMgr::getInstance().getVarLength(mFunctionName, tokenIden.raw);
    if(length != 1) {
        ErrorReport::getInstance().send(
            true,
            "Semantic Error",
            "Length of Loop Var '" + tokenIden.raw + "' must be 1, but now '" + std::to_string(length) + "'",
            mLineNow,
            tokenIden.col
        );
    }
    match(TOKEN_ASS, ":=");
    matchExpression();
    CodeMgr::getInstance().popToLocalVar(mFunctionName, offset);
    CodeMgr::getInstance().setWhileBegin(mFunctionName, whileId);
    match(TOKEN_TO, "TO");
    matchExpression();
    CodeMgr::getInstance().pushLocalVarValue(mFunctionName, offset);
    int step = 1, colTmp = 0;
    if(getToken().type == TOKEN_BY) { // the number after BY must be a constant 
        int negflag = 0;
        match(TOKEN_BY, "BY");
        if(getToken().type == TOKEN_SUB) {
            match(TOKEN_SUB, "-");
            negflag = 1;
        }
        const Token& tokenNum = match(TOKEN_NUMBER, "NUMBER");
        colTmp = tokenNum.col;
        step = atoi(tokenNum.raw.c_str());
        if(negflag) {
            step = - step;
        }
    }
    if(step == 0) {
        ErrorReport::getInstance().send(
            false,
            "Syntax Warning",
            "The Step of FOR is zero (meaningless)",
            mLineNow,
            colTmp
        );
    }
    if(step > 0) {
        CodeMgr::getInstance().geqStackTop(mFunctionName);
    }else {
        CodeMgr::getInstance().leqStackTop(mFunctionName);
    }
    CodeMgr::getInstance().checkAndJumpEndWhile(mFunctionName, whileId);
    match(TOKEN_DO, "DO");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    int end = mSubPointer[lineFrom][mSubPointer[lineFrom].size() - 1];
    mLineNow ++;
    whileStack.push(whileId);
    mGetLineIdByWhileId[whileId] = lineFrom;
    while(mLineNow < end) {
        compileLine(mLineNow);
    }
    openLine(mLineNow);
    match(TOKEN_ENDFOR, "ENDFOR");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    whileStack.pop();
    CodeMgr::getInstance().pushLocalVarValue(mFunctionName, offset); // set new value for loop var
    CodeMgr::getInstance().pushConstant(mFunctionName, step);
    CodeMgr::getInstance().addStackTop(mFunctionName);
    CodeMgr::getInstance().popToLocalVar(mFunctionName, offset); 
    CodeMgr::getInstance().backToWhileBegin(mFunctionName, whileId);
    CodeMgr::getInstance().setEndWhile(mFunctionName, whileId);
    mLineNow = end + 1;
}


void ProgramReader::compileAsm(int lineFrom) {
    mLineNow = lineFrom;
    openLine(lineFrom);
    match(TOKEN_ASM, "ASM");
    match(TOKEN_OPEN, "(");
    const Token& tokenStr = match(TOKEN_STRING, "STRING");
    match(TOKEN_CLOSE, ")");
    match(TOKEN_ENDOFLINE, "END_OF_LINE");
    CodeMgr::getInstance().appendFuncAsm(mFunctionName, Utils::getRealString(tokenStr.raw,
        mLineNow,
        tokenStr.col
    ));
    mLineNow = lineFrom + 1;
}


void ProgramReader::compileLine(int lineFrom) { // total eight form
    //printf("lineFrom = %d\n", lineFrom);
    if(mTokenTable[lineFrom][0].type == TOKEN_ASM) {
        compileAsm(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_VAR) {
        compileVar(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_ENDOFLINE) {
        mLineNow = lineFrom + 1;
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_RETURN) {
        compileReturn(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_PUTCHAR) {
        compilePutchar(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_IF) {
        compileIf(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_WHILE) {
        compileWhile(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_IDENTIFIER || 
        mTokenTable[lineFrom][0].type == TOKEN_VARAT
    ) {
        compileAssign(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_CALL) {
        compileCall(lineFrom);
    }else
    if(mTokenTable[lineFrom][0].type == TOKEN_FOR) {
        //fprintf(stderr, "compileFor lineFrom = %d\n", lineFrom);
        compileFor(lineFrom);
    }else
    if(!whileStack.empty() && mMainPointer[lineFrom] == mGetLineIdByWhileId[whileStack.top()]) {
        int whileId = whileStack.top();
        openLine(mLineNow);
        if(getToken().type == TOKEN_BREAK) {
            match(TOKEN_BREAK, "BREAK");
            match(TOKEN_ENDOFLINE, "END_OF_LINE");
            CodeMgr::getInstance().jumpEndWhile(mFunctionName, whileId);
        }else
        if(getToken().type == TOKEN_CONTINUE) {
            match(TOKEN_CONTINUE, "CONTINUE");
            match(TOKEN_ENDOFLINE, "END_OF_LINE");
            CodeMgr::getInstance().backToWhileBegin(mFunctionName, whileId);
        }else {
            ErrorReport::getInstance().send(
                true,
                "Inner Error",
                "Slaves of WHILE is not BREAK or CONTINUE but '" + getToken().raw + "'",
                mLineNow,
                getToken().col
            );
        }
        mLineNow ++;
    }else
    // ----------------------------------------------------------------------
    // TODO: Add your compile code here
    {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "the first item is '" + mTokenTable[lineFrom][0].raw + "' which is not allowed",
            lineFrom,
            mTokenTable[lineFrom][0].col
        );
    }
}


void ProgramReader::matchIdentifierExpression(int& varCnt) {
    if(VarMgr::getInstance().existFunc(getToken().raw)) { // this is a function
        const Token& tokenFunc = getToken(); nextToken();
        FuncGraph::getInstance().addEdge(mFunctionName, tokenFunc.raw); // add edge between functions
        
        int arguCnt = VarMgr::getInstance().getFuncArguCnt(tokenFunc.raw);
        int nowCnt = 0;
        match(TOKEN_OPEN, "(");
        if(getToken().type == TOKEN_CLOSE) {
            goto match_close_here;
        }
get_one_argu_here:
        matchExpression();
        nowCnt ++;
        if(getToken().type == TOKEN_COMMA) {
            nextToken();
            goto get_one_argu_here;
        }
match_close_here:
        const Token& tokenClose = match(TOKEN_CLOSE, ")");
        if(arguCnt != nowCnt) {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "function '' need " + std::to_string(arguCnt) + " paras but " + std::to_string(nowCnt) + " given",
                mLineNow,
                tokenClose.col
            );
        }
        for(int i = 0, j = arguCnt - 1; i < j; i ++, j --) {
            CodeMgr::getInstance().swapStack(mFunctionName, i, j);
        }
        CodeMgr::getInstance().callFunction(mFunctionName, tokenFunc.raw);
        CodeMgr::getInstance().clearStack(mFunctionName, nowCnt);
        CodeMgr::getInstance().pushAX(mFunctionName);
        varCnt ++;
    }else {
        const Token& tokenIden = getToken(); nextToken();
        bool isLocal;
        int  offset ; // global var or local var is different
        int  length ;
        if(VarMgr::getInstance().existLocalVar(mFunctionName, tokenIden.raw)) {
            isLocal = true;
            offset  = VarMgr::getInstance().getVarOffset(
                mFunctionName, tokenIden.raw, mLineNow, tokenIden.col
            );
            length  = VarMgr::getInstance().getVarLength(
                mFunctionName, tokenIden.raw
            );
        }else {
            if(!VarMgr::getInstance().existGlobalVar(tokenIden.raw)) {
                ErrorReport::getInstance().send(
                    true,
                    "Semantic Error",
                    "Var '" + tokenIden.raw + "' not exist",
                    mLineNow, 
                    tokenIden.col
                );
            }
            isLocal = false; // global var, funcName = ""
            offset  = VarMgr::getInstance().getVarOffset(
                "", tokenIden.raw, mLineNow, tokenIden.col
            );
            length  = VarMgr::getInstance().getVarLength("", tokenIden.raw);
        }
        if(length <= 1) {
            if(isLocal) {
                CodeMgr::getInstance().pushLocalVarValue(mFunctionName, offset);
            }else {
                CodeMgr::getInstance().pushGlobalVarValue(mFunctionName, offset);
            }
        }else {
            if(isLocal) {
                CodeMgr::getInstance().pushLocalVarOffset(mFunctionName, offset + length - 1);
            }else {
                CodeMgr::getInstance().pushGlobalVarOffset(mFunctionName, offset);
            }
            match(TOKEN_INDEXOPEN, "[");
            matchExpression();
            match(TOKEN_INDEXCLOSE, "]");
            CodeMgr::getInstance().getArrayElementValue(mFunctionName);
        }
        varCnt ++;
    }
}


void ProgramReader::matchExpression() {
    std::stack <int> opeStack;             // operator stack
    // now you must have opened a line by openLine
    // TODO: Expression to Code
    int varCnt = 0;
    int openCnt = 0; // count open brace
get_new_var:
    if(getToken().type == TOKEN_VARAT) {
        match(TOKEN_VARAT, "VARAT");
        match(TOKEN_OPEN, "(");
        matchExpression();
        match(TOKEN_CLOSE, ")");
        CodeMgr::getInstance().getVarAtStackTop(mFunctionName);
        varCnt ++;
    }else
    if(getToken().type == TOKEN_NOT) {
        match(TOKEN_NOT, "NOT");
        match(TOKEN_OPEN, "(");
        matchExpression();
        match(TOKEN_CLOSE, ")");
        CodeMgr::getInstance().getNotStackTop(mFunctionName);
        varCnt ++;
    }else
    if(getToken().type == TOKEN_SUB) { // -1, -x, -arr[3], -f(), -(expression)
        match(TOKEN_SUB, "-");
        if(getToken().type == TOKEN_NUMBER) {
            const Token& tokenNum = match(TOKEN_NUMBER, "NUMBER");
            CodeMgr::getInstance().pushConstant(mFunctionName, -atoi(tokenNum.raw.c_str()));
            varCnt ++;
        }else
        if(getToken().type == TOKEN_IDENTIFIER) {
            matchIdentifierExpression(varCnt);
            CodeMgr::getInstance().negStackTop(mFunctionName);
        }else
        if(getToken().type == TOKEN_OPEN) {
            match(TOKEN_OPEN, "(");
            matchExpression();
            match(TOKEN_CLOSE, ")");
            CodeMgr::getInstance().negStackTop(mFunctionName);
            varCnt ++;
        }else
        {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "'-' can not be followed by '" + getToken().raw + "'",
                mLineNow,
                getToken().col
            );
        }
    }else
    if(getToken().type == TOKEN_OFFSET) {
        match(TOKEN_OFFSET, "OFFSET");
        match(TOKEN_OPEN, "(");
        const Token& tokenIden = match(TOKEN_IDENTIFIER, "IDENTIFIER");
        match(TOKEN_CLOSE, ")");
        bool isLocal;
        int offset; // global var or local var is different
        int length;
        if(VarMgr::getInstance().existLocalVar(mFunctionName, tokenIden.raw)) {
            isLocal = true;
            offset  = VarMgr::getInstance().getVarOffset(
                mFunctionName, tokenIden.raw, mLineNow, tokenIden.col
            );
            length  = VarMgr::getInstance().getVarLength(
                mFunctionName, tokenIden.raw
            );
        }else {
            if(!VarMgr::getInstance().existGlobalVar(tokenIden.raw)) {
                ErrorReport::getInstance().send(
                    true,
                    "Semantic Error",
                    "Var '" + tokenIden.raw + "' not exist",
                    mLineNow, 
                    tokenIden.col
                );
            }
            isLocal = false; // global var, funcName = ""
            offset  = VarMgr::getInstance().getVarOffset(
                "", tokenIden.raw, mLineNow, tokenIden.col
            );
            length  = VarMgr::getInstance().getVarLength("", tokenIden.raw);
        }
        if(isLocal) {
            CodeMgr::getInstance().pushLocalVarOffset(mFunctionName, offset + length - 1);
        }else {
            CodeMgr::getInstance().pushGlobalVarOffset(mFunctionName, offset);
        }
        varCnt ++;
    }else
    if(getToken().type == TOKEN_STRING) {
        const Token& tokenStr = getToken(); nextToken();
        std::string realString = Utils::getRealString(tokenStr.raw, mLineNow, tokenStr.col);
        if(tokenStr.raw[0] == '\'') {
            if(realString.length() != 1) {
                ErrorReport::getInstance().send(
                    true,
                    "Syntax Error",
                    "There must be at most 1 character in string begin with <'> ",
                    mLineNow,
                    tokenStr.col
                );
            }
            CodeMgr::getInstance().pushConstant(mFunctionName, (unsigned)realString[0]);
            varCnt ++;
        }else {
            int strId = ++ stringCnt;
            std::string tmpName = "#STR_" + std::to_string(strId);
            VarMgr::getInstance().addVar("", tmpName, realString.length() + 1, 
                mLineNow, tokenStr.col
            );
            int offset  = VarMgr::getInstance().getVarOffset(
                "", tmpName, mLineNow, tokenStr.col
            );
            CodeMgr::getInstance().setGlobalString(mFunctionName, offset, realString);
            CodeMgr::getInstance().pushGlobalVarOffset(mFunctionName, offset);
            varCnt ++;
        }
    }else
    if(getToken().type == TOKEN_OPEN) {
        match(TOKEN_OPEN, "(");
        opeStack.push(TOKEN_OPEN);
        openCnt ++;
        goto get_new_var;
    }else
    if(getToken().type == TOKEN_GETCHAR) {
        match(TOKEN_GETCHAR, "GETCHAR");
        match(TOKEN_OPEN, "(");
        match(TOKEN_CLOSE, ")");
        varCnt ++;
        CodeMgr::getInstance().pushGetchar(mFunctionName);
    }else
    if(getToken().type == TOKEN_NUMBER) {
        const Token& tokenNum = getToken(); nextToken();
        CodeMgr::getInstance().pushConstant(mFunctionName, atoi(tokenNum.raw.c_str()));
        varCnt ++;
    }else 
    if(getToken().type == TOKEN_IDENTIFIER) {   // function local/global, var/array
        matchIdentifierExpression(varCnt);
    }else {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "expected an expression but get a '" + getToken().raw + "'",
            mLineId,
            getToken().col
        );
    }
    if(openCnt > 0 && getToken().type == TOKEN_CLOSE) {
        const Token& tokenClose = match(TOKEN_CLOSE, ")");
        while(!opeStack.empty() && opeStack.top() != TOKEN_OPEN) {
            compute(opeStack);
            varCnt --;
        }
        if(opeStack.empty()) {
            ErrorReport::getInstance().send(
                true,
                "Syntax Error",
                "Close Brace ')' can not find '(' to match with it",
                mLineNow,
                tokenClose.col
            );
        }else {
            opeStack.pop();
        }
        openCnt --;
    }
    if(Utils::isIntegerOperator(getToken().type)) { // + - * / %
        const Token& tokenOpe = getToken(); nextToken();      
        // printf("tokenOpe.raw = %s\n", tokenOpe.raw.c_str());
        while(!opeStack.empty() && Utils::priority(opeStack.top()) >= Utils::priority(tokenOpe.type)) {
            compute(opeStack);
            varCnt --;
        }
        opeStack.push(tokenOpe.type);
        goto get_new_var;
    }else {
        
        goto finish_argu_intput;
    }
finish_argu_intput:
    // fprintf(stderr, "end expression by %s\n", getToken().raw.c_str());
    while(!opeStack.empty()) {
        compute(opeStack); // compute a operation
        varCnt --;
    }
    // printf("varCnt = %d, openCnt = %d\n", varCnt, openCnt);
    if(varCnt < 1) {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "expression form error: to many operator",
            mLineId,
            getToken().col
        );
    }
    if(varCnt > 1) {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "expression form error: to many var Value",
            mLineId,
            getToken().col
        );
    }
}


void ProgramReader::compute(std::stack<int>& opeStack) {
    if(opeStack.empty()) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "ProgramReader::compute compute when opeStack is empty"
        );
    }
    int opeType = opeStack.top(); opeStack.pop();
    switch(opeType) {
        case TOKEN_ADD:
            CodeMgr::getInstance().addStackTop(mFunctionName);
            break;
        case TOKEN_SUB:
            CodeMgr::getInstance().subStackTop(mFunctionName);
            break;
        case TOKEN_MUL:
            CodeMgr::getInstance().mulStackTop(mFunctionName);
            break;
        case TOKEN_DIV:
            CodeMgr::getInstance().divStackTop(mFunctionName);
            break;
        case TOKEN_MOD:
            CodeMgr::getInstance().modStackTop(mFunctionName);
            break;
        case TOKEN_LSS:
            CodeMgr::getInstance().lssStackTop(mFunctionName);
            break;
        case TOKEN_LEQ:
            CodeMgr::getInstance().leqStackTop(mFunctionName);
            break;
        case TOKEN_GTR:
            CodeMgr::getInstance().gtrStackTop(mFunctionName);
            break;
        case TOKEN_GEQ:
            CodeMgr::getInstance().geqStackTop(mFunctionName);
            break;
        case TOKEN_EQU:
            CodeMgr::getInstance().equStackTop(mFunctionName);
            break;
        case TOKEN_NEQ:
            CodeMgr::getInstance().neqStackTop(mFunctionName);
            break;
        case TOKEN_AND:
            CodeMgr::getInstance().andStackTop(mFunctionName);
            break;
        case TOKEN_OR:
            CodeMgr::getInstance().orStackTop(mFunctionName);
            break;
        default:
            ErrorReport::getInstance().send(
                true,
                "Inner Error",
                "ProgramReader::compute opeType '" + std::to_string(opeType) + "' is not allowed"
            );
    }
}

