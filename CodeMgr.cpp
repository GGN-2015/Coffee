#include "CodeMgr.h"
#include "ErrorReport.h"
#include "FuncGraph.h"
#include "Utils.h"
#include "VarMgr.h"


static int jumpCnt = 0;
#define OPTIMIZED_BY_O1 std::string(";optimized by O1\n")


CodeMgr& CodeMgr::getInstance() {
    static CodeMgr cm;
    return cm;
}


void CodeMgr::clearFunc(std::string funcName) { // clear and create
    mFuncCode[funcName] = {};
}


bool CodeMgr::optimizePushPop(std::string funcName) {
    std::vector<std::string> &funcCodeVec = getFuncCode(funcName);
    int lineNow = funcCodeVec.size() - 1;
    if(Utils::isJmpFlag(funcCodeVec[lineNow])) {
        return false; // do not optimize JMP flag
    }
    std::string ope = Utils::getOpe(funcCodeVec[lineNow]);
    std::string dst = Utils::getReg1(funcCodeVec[lineNow]);
    if(ope != "POP") {
        return false; // optimizePushPop only optimize PUSH - POP
    }
    int lastPush = lineNow - 1;
    while(lastPush > 0) {
        if(Utils::isJmpFlag(funcCodeVec[lastPush])) {
            return false; // can not optimize PUSH out of function but pop in the function
        }
        std::string opeFrom = Utils::getOpe(funcCodeVec[lastPush]);
        if(opeFrom == "PUSH") {
            break; // this line is the last PUSH that we find
        }
        if(Utils::checkAffect(funcCodeVec[lastPush], dst)) {
            return false; // can not optimize when affected
        }
        lastPush --;
    }
    if(lastPush < 0) return false; // can not find last PUSH
    std::string opeFrom = Utils::getOpe(funcCodeVec[lastPush]);
    std::string src     = Utils::getReg1(funcCodeVec[lastPush]);
    funcCodeVec.erase(funcCodeVec.end() - 1); // delete last statement
    if(src == dst) {
        funcCodeVec[lastPush] = OPTIMIZED_BY_O1;
    }else {
        funcCodeVec[lastPush] = "    MOV " + dst + ", " + src + " " + OPTIMIZED_BY_O1;
    }
    return true;
}


bool CodeMgr::optimizeMovBxAx(std::string funcName) {
    std::vector<std::string> &funcCodeVec = getFuncCode(funcName);
    int lineNow = funcCodeVec.size() - 1;
    if(Utils::isJmpFlag(funcCodeVec[lineNow])) {
        return false; // do not optimize JMP flag
    }
    if(lineNow == 0) return false;
    std::string opeMv1 = Utils::getOpe (funcCodeVec[lineNow]);
    std::string dstBx  = Utils::getReg1(funcCodeVec[lineNow]);
    std::string srcAx  = Utils::getReg2(funcCodeVec[lineNow]);
    if(opeMv1 != "MOV" || dstBx != "BX" || srcAx != "AX") {
        return false; // optimizePushPop only optimize PUSH - POP
    }
    std::string opeMv2 = Utils::getOpe (funcCodeVec[lineNow - 1]);
    std::string dstAx  = Utils::getReg1(funcCodeVec[lineNow - 1]);
    std::string srcCn  = Utils::getReg2(funcCodeVec[lineNow - 1]);
    if(opeMv2 != "MOV" || dstAx != "AX") {
        return false; // optimizePushPop only optimize PUSH - POP
    }
    funcCodeVec.erase(funcCodeVec.end()-1);
    funcCodeVec[lineNow - 1] = "MOV " + dstBx + ", " + srcCn + " " + OPTIMIZED_BY_O1;
    return true;
}


bool CodeMgr::optimize1LastLine(std::string funcName) {
    bool ans1 = optimizePushPop(funcName);
    bool ans2 = optimizeMovBxAx(funcName);
    return ans1 || ans2;
}


void CodeMgr::appendFunc(std::string funcName, std::string codeStr) {
    checkFuncExist(funcName);
    std::vector<std::string> &funcCodeVec = getFuncCode(funcName);
    std::string newLineCode = codeStr + "\n";
    funcCodeVec.push_back(newLineCode);
    
    // ------------------------------------------------------------ //
    if(CODE_OPTIMIZE_GRADE >= 1)
        while(optimize1LastLine(funcName)); // while ok to optimize last line, optimize
    // ------------------------------------------------------------ //
}


void CodeMgr::appendFuncAsm(std::string funcName, std::string codeStr) {
    checkFuncExist(funcName);
    getFuncCode(funcName).push_back("    " + Utils::fillStrTo(codeStr, 30) + "; code from inner asm\n");
}


void CodeMgr::testStackTop(std::string mFunctionName) {
    appendFunc(mFunctionName, "    POP AX");
    appendFunc(mFunctionName, "    AND AX, AX");
}


void CodeMgr::jumpEndIf(std::string mFunctionName, int ifIdNow) {
    appendFunc(mFunctionName, "    JMP ENDIF_" + std::to_string(ifIdNow));
}


void CodeMgr::jzEndIf(std::string mFunctionName, int ifIdNow) {
    int jumpId = ++ jumpCnt;
    std::string jumpName = "JUMP_" + std::to_string(jumpId);
    appendFunc(mFunctionName, "    JNZ " + jumpName);
    appendFunc(mFunctionName, "    JMP ENDIF_" + std::to_string(ifIdNow));
    appendFunc(mFunctionName, jumpName + ":");
}


void CodeMgr::setEndIf(std::string mFunctionName, int ifIdNow) {
    appendFunc(mFunctionName, "ENDIF_" + std::to_string(ifIdNow) + ":");
}


void CodeMgr::jzElse(std::string mFunctionName, int ifIdNow) {
    int jumpId = ++ jumpCnt;
    std::string jumpName = "JUMP_" + std::to_string(jumpId);
    appendFunc(mFunctionName, "    JNZ " + jumpName);
    appendFunc(mFunctionName, "    JMP ELSE_" + std::to_string(ifIdNow));
    appendFunc(mFunctionName, jumpName + ":");
}


void CodeMgr::setElse(std::string mFunctionName, int ifIdNow) {
    appendFunc(mFunctionName, "ELSE_" + std::to_string(ifIdNow) + ":");
}


void CodeMgr::checkFuncExist(std::string funcName) const {
    if(mFuncCode.count(funcName) == 0) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "CodeMgr::appendFunc funcName '" + funcName + "' not cleared"
        );
    }
}


void CodeMgr::getVarAtStackTop(std::string funcName) {
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    PUSH WORD PTR [BX]");
}


void CodeMgr::pushLocalVarOffset(std::string funcName, int innerOffset) {
    appendFunc(funcName, "    MOV AX, BP");
    appendFunc(funcName, "    SUB AX, " + std::to_string(2 * (innerOffset + 1)));
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::pushGlobalVarOffset(std::string funcName, int innerOffset) {
    appendFunc(funcName, "    MOV AX, " + std::to_string(innerOffset * 2));
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::pushLocalVarValue(std::string funcName, int offset) {
    appendFunc(funcName, "    PUSH WORD PTR [BP-" + std::to_string((offset + 1) * 2) + "]");
}


void CodeMgr::pushGlobalVarValue(std::string funcName, int offset) {
    appendFunc(funcName, "    MOV SI, " + std::to_string(offset * 2));
    appendFunc(funcName, "    PUSH WORD PTR [SI]");
}


void CodeMgr::popToLocalVar(std::string funcName, int offset) {
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV [BP-" + std::to_string((offset + 1) * 2) + "], AX");
}


void CodeMgr::popToGlobalVar(std::string funcName, int offset) {
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV SI, " + std::to_string(offset * 2));
    appendFunc(funcName, "    MOV [SI], AX");
}


void CodeMgr::addStackTop(std::string funcName) {
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    ADD AX, BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::subStackTop(std::string funcName) {
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::mulStackTop(std::string funcName) {
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MUL BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::divStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpName = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV DX, 0");
    appendFunc(funcName, "    CMP AX, 0");
    appendFunc(funcName, "    JGE " + jumpName);
    appendFunc(funcName, "    MOV DX, 0FFFFH");
    appendFunc(funcName, jumpName + ":");
    appendFunc(funcName, "    IDIV BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::modStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpName = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV DX, 0");
    appendFunc(funcName, "    CMP AX, 0");
    appendFunc(funcName, "    JGE " + jumpName);
    appendFunc(funcName, "    MOV DX, 0FFFFH");
    appendFunc(funcName, jumpName + ":");
    appendFunc(funcName, "    IDIV BX");
    appendFunc(funcName, "    PUSH DX");
}


#define ENTRANCE_FUNC_NAME std::string("main")


void CodeMgr::outputCode(int stackSegmentSizeWord) {
    if(mFuncCode.count(ENTRANCE_FUNC_NAME) == 0) {
        ErrorReport::getInstance().send(
            true,
            "Entry Error",
            "Can not found the function " + ENTRANCE_FUNC_NAME
        );
    }
    if(VarMgr::getInstance().getFuncArguCnt(ENTRANCE_FUNC_NAME) != 0) {
        ErrorReport::getInstance().send(
            true,
            "Entry Error",
            "function " + ENTRANCE_FUNC_NAME + " can not have arguments"
        );
    }
    if(stackSegmentSizeWord - stackSegmentLengthWord < 0) {
        ErrorReport::getInstance().send(
            true,
            "Semantic Error",
            "You have too many global var so that memory = " + 
                std::to_string(stackSegmentSizeWord) + " WORDS is not Enough"
        );
    }
    if(stackSegmentSizeWord >= MEMORY_WORD_MAX) {
        ErrorReport::getInstance().send(
            true,
            "Semantic Error",
            "stack segment size must be under " + std::to_string(MEMORY_WORD_MAX) + " words"
        );
    }
    FuncGraph::getInstance().searchAll(ENTRANCE_FUNC_NAME); // do not compile the function not reached
    
    printf("STACKSG SEGMENT STACK\n");
    printf("%s", stackSegment.c_str());
    printf("    DW %d DUP(0)\n", stackSegmentSizeWord - stackSegmentLengthWord);
    printf("STACKSG ENDS\n\n");
    printf("CODESG SEGMENT\n");
    printf("    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG\n\n");
    for(auto& funcString: mFuncCode) {
        if(!FuncGraph::getInstance().checkFuncVisited(funcString.first)) {
            continue; // do not compile function that has not been used
        }
        int localVarCnt = VarMgr::getInstance().getFuncLocalVarCnt(funcString.first);
        int arguCnt = VarMgr::getInstance().getFuncArguCnt(funcString.first);
        printf("FUNC_%s: ; localVar: %5d, arguCnt: %5d\n", 
            funcString.first.c_str(), localVarCnt, arguCnt);
        printf("    PUSH BP\n");
        printf("    MOV BP, SP\n");
        printf("    SUB SP, %d\n", 2 * localVarCnt); // create local VARs
        for(int i = 1; i <= arguCnt; i ++) {         // create argu list from stack
            printf("    MOV AX, [BP+%d]\n", 2 + 2 * i);
            printf("    MOV [BP-%d], AX\n", 2 * i);
        }
        // printf("%s", funcString.second.c_str());
        for(auto& funcLine: funcString.second) {
            if(funcLine == OPTIMIZED_BY_O1) continue; // JUMP optimized line
            printf("%s", funcLine.c_str());
        }
        printf("ENDFUNC_%s:\n", funcString.first.c_str());
        printf("    ADD SP, %d\n", 2 * localVarCnt); // destroy local VARs
        printf("    POP BP\n");
        printf("    RET\n\n");
    }
    printf("ENTER_POINT:\n");
    printf("    MOV AX, STACKSG\n");
    printf("    MOV DS, AX\n");
    printf("    MOV SS, AX\n"); // stack segment and data segment are the same
    printf("    MOV SP, %d\n", stackSegmentSizeWord * 2);
    printf(("    CALL FUNC_" + ENTRANCE_FUNC_NAME + "\n").c_str());
    printf("    MOV AH, 4CH\n");
    printf("    INT 21H\n");
    printf("CODESG ENDS\n");
    printf("END ENTER_POINT\n");
}

void CodeMgr::lssStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    JL " + jumpFlag);
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, jumpFlag + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::leqStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    JLE " + jumpFlag);
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, jumpFlag + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::gtrStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    JG " + jumpFlag);
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, jumpFlag + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::geqStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    JGE " + jumpFlag);
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, jumpFlag + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::equStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    JE " + jumpFlag);
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, jumpFlag + ":");
    appendFunc(funcName, "    PUSH CX");
}

void CodeMgr::neqStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, "    SUB AX, BX");
    appendFunc(funcName, "    JNE " + jumpFlag);
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, jumpFlag + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::andStackTop(std::string funcName) {
    int jumpId[2];
    std::string jumpStr[2];
    for(int i = 0; i < 2; i ++) {
        jumpId [i] = ++ jumpCnt;
        jumpStr[i] = "JUMP_" + std::to_string(jumpId[i]);
    }
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    AND BX, BX");
    appendFunc(funcName, "    JZ " + jumpStr[0]);
    appendFunc(funcName, "    MOV BX, 1");
    appendFunc(funcName, jumpStr[0] + ":");
    
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    AND AX, AX");
    appendFunc(funcName, "    JZ " + jumpStr[1]);
    appendFunc(funcName, "    MOV AX, 1");
    appendFunc(funcName, jumpStr[1] + ":");
    
    appendFunc(funcName, "    AND AX, BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::orStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpStr = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    OR AX, BX");
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, "    AND AX, AX");
    appendFunc(funcName, "    JZ " + jumpStr);
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, jumpStr + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::pushConstant(std::string funcName, int value) {
    appendFunc(funcName, "    MOV AX, " + std::to_string(value));
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::sendTopToAx(std::string funcName) {
    appendFunc(funcName, "    POP AX");
}


void CodeMgr::jumpReturn(std::string funcName) {
    appendFunc(funcName, "    JMP ENDFUNC_" + funcName);
}


void CodeMgr::outputStackTop(std::string funcName) {
    appendFunc(funcName, "    POP DX");
    appendFunc(funcName, "    MOV AH, 2");
    appendFunc(funcName, "    INT 21H");
}


void CodeMgr::MOV(std::string funcName, std::string regTo, std::string regFrom) {
    appendFunc(funcName, "    MOV " + regTo + ", " + regFrom);
}


void CodeMgr::pushGetchar(std::string funcName) {  // actually it is like getch, but show char
    appendFunc(funcName, "    MOV AH, 1");
    appendFunc(funcName, "    INT 21H");
    appendFunc(funcName, "    MOV AH, 0");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::getArrayElementValue(std::string funcName) {
    appendFunc(funcName, "    POP BX"); // index
    appendFunc(funcName, "    POP AX"); // array offset
    appendFunc(funcName, "    ADD BX, BX");
    // appendFunc(funcName, "    SUB AX, BX");
    // appendFunc(funcName, "    MOV BX, AX");
    appendFunc(funcName, "    ADD BX, AX");
    appendFunc(funcName, "    PUSH WORD PTR [BX]");
}


void CodeMgr::popToArrayVar(std::string funcName) {
    appendFunc(funcName, "    POP CX"); // value
    appendFunc(funcName, "    POP BX"); // index
    appendFunc(funcName, "    POP AX"); // offset
    appendFunc(funcName, "    ADD BX, BX");
    // appendFunc(funcName, "    SUB AX, BX");
    // appendFunc(funcName, "    MOV BX, AX");
    appendFunc(funcName, "    ADD BX, AX");
    appendFunc(funcName, "    MOV [BX], CX");
}


void CodeMgr::setGlobalString(std::string funcName, int offset, std::string realString) {
    stackSegment += "    DW ";
    for(int i = 0; i < realString.length(); i ++) {
        stackSegment += std::to_string(realString[i]) + ", ";
        // appendFunc(funcName, "    MOV AX, " + std::to_string((   unsigned)realString[i]));
        // appendFunc(funcName, "    MOV SI, " + std::to_string((offset + i) * 2));
        // appendFunc(funcName, "    MOV [SI], AX"); // offset
    }
    stackSegment += "0\n";
    stackSegmentLengthWord += realString.length() + 1;
}


void CodeMgr::swapStack(std::string mFunction, int i, int j) {
    appendFunc(mFunction, "    MOV SI, SP");
    appendFunc(mFunction, "    MOV AX, [SI+" + std::to_string(i * 2) + "]");
    appendFunc(mFunction, "    MOV BX, [SI+" + std::to_string(j * 2) + "]");
    appendFunc(mFunction, "    MOV [SI+" + std::to_string(i * 2) + "], BX");
    appendFunc(mFunction, "    MOV [SI+" + std::to_string(j * 2) + "], AX");
}


void CodeMgr::callFunction(std::string nowFuncName, std::string nextFuncName) {
    appendFunc(nowFuncName, "    CALL FUNC_" + nextFuncName);
}


void CodeMgr::clearStack(std::string nowFuncName, int nowCnt) {
    if(nowCnt) {
        appendFunc(nowFuncName, "    ADD SP, " + std::to_string(nowCnt * 2));
    }
}


void CodeMgr::setWhileBegin(std::string funcName, int whileId) {
    appendFunc(funcName, "WHILE_" + std::to_string(whileId) + ":");
}


void CodeMgr::checkAndJumpEndWhile(std::string funcName, int whileId) {
    std::string endWhileString = "ENDWHILE_" + std::to_string(whileId);
    int jumpId = ++ jumpCnt;
    std::string jumpFlag = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    AND AX, AX");
    appendFunc(funcName, "    JNZ " + jumpFlag);
    appendFunc(funcName, "    JMP " + endWhileString);
    appendFunc(funcName, jumpFlag + ":");
}


void CodeMgr::backToWhileBegin(std::string funcName, int whileId) {
    appendFunc(funcName, "    JMP WHILE_" + std::to_string(whileId));
}


void CodeMgr::setEndWhile(std::string funcName, int whileId) {
    std::string endWhileString = "ENDWHILE_" + std::to_string(whileId);
    appendFunc(funcName, endWhileString + ":");
}


void CodeMgr::pushAX(std::string funcName) {
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::jumpEndWhile(std::string funcName, int whileId) {
    std::string endWhileString = "ENDWHILE_" + std::to_string(whileId);
    appendFunc(funcName, "    JMP " + endWhileString);
}


void CodeMgr::PopToVarAt(std::string funcName) {
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    POP SI");
    appendFunc(funcName, "    MOV [SI], AX");
}


std::vector<std::string>& CodeMgr::getFuncCode(std::string funcName) { // speed up search
    static std::string  lastCallFuncName = "";
    static std::vector<std::string>* lastCallStringPos = nullptr;
    if(lastCallStringPos == nullptr || lastCallFuncName != funcName) {
        lastCallFuncName = funcName;
        lastCallStringPos = &mFuncCode[funcName];
    }
    return *lastCallStringPos;
}


void CodeMgr::getNotStackTop(std::string funcName) {
    int jumpId = ++ jumpCnt;
    std::string jumpStr = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV CX, 0");
    appendFunc(funcName, "    AND AX, AX");
    appendFunc(funcName, "    JNZ " + jumpStr);
    appendFunc(funcName, "    MOV CX, 1");
    appendFunc(funcName, jumpStr + ":");
    appendFunc(funcName, "    PUSH CX");
}


void CodeMgr::setGlobalVar(std::string varName, int length, const int* arr, int val_len) {
    if(arr == nullptr) {
        stackSegment += "    GLOBAL_" + varName + " DW " + std::to_string(length) + " DUP(0)\n";
    }else {
        stackSegment += "    GLOBAL_" + varName + " DW ";
        for(int i = 0; i < val_len; i ++) {
            if(i != 0) stackSegment += ", ";
            stackSegment += std::to_string(arr[i]);
        }
        if(val_len < length) {
            if(val_len != 0) stackSegment += ",";
            stackSegment += " " + std::to_string(length - val_len) + " DUP(0)";
        }
        stackSegment += "\n";
    }
    stackSegmentLengthWord += length;
}


void CodeMgr::negStackTop(std::string funcName) {
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    NEG AX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::memset(std::string funcName, int value) {
    int jumpId = ++ jumpCnt;
    std::string jumpName = "JUMP_" + std::to_string(jumpId);
    appendFunc(funcName, "    POP CX"); // length
    appendFunc(funcName, "    POP BX"); // offset
    appendFunc(funcName, jumpName+ ":");
    appendFunc(funcName, "    MOV WORD PTR [BX], " + std::to_string(value));
    appendFunc(funcName, "    ADD BX, 2");
    appendFunc(funcName, "    LOOP " + jumpName);
}


void CodeMgr::setOptimizeGrade(int newGrade) {
    CODE_OPTIMIZE_GRADE = newGrade;
}

