#include "CodeMgr.h"
#include "ErrorReport.h"
#include "VarMgr.h"


static int jumpCnt = 0;


CodeMgr& CodeMgr::getInstance() {
    static CodeMgr cm;
    return cm;
}


void CodeMgr::clearFunc(std::string funcName) {
    mFuncCode[funcName] = "";
}


void CodeMgr::appendFunc(std::string funcName, std::string codeStr) {
    checkFuncExist(funcName);
    mFuncCode[funcName] += codeStr + "\n";
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


void CodeMgr::PopToLocalVar(std::string funcName, int offset) {
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV [BP-" + std::to_string((offset + 1) * 2) + "], AX");
}


void CodeMgr::PopToGlobalVar(std::string funcName, int offset) {
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
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV DX, 0");
    appendFunc(funcName, "    DIV BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::modStackTop(std::string funcName) {
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    MOV DX, 0");
    appendFunc(funcName, "    DIV BX");
    appendFunc(funcName, "    PUSH DX");
}


void CodeMgr::outputCode(int stackSegmentSizeWord) {
    if(mFuncCode.count("main") == 0) {
        ErrorReport::getInstance().send(
            true,
            "Entry Error",
            "Can not found the function 'main'"
        );
    }
    if(VarMgr::getInstance().getFuncArguCnt("main") != 0) {
        ErrorReport::getInstance().send(
            true,
            "Entry Error",
            "function 'main' can not have arguments"
        );
    }
    printf("STACKSG SEGMENT STACK\n");
    printf("    DW %d DUP(0)\n", stackSegmentSizeWord);
    printf("STACKSG ENDS\n\n");
    printf("CODESG SEGMENT\n");
    printf("    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG\n\n");
    for(auto& funcString: mFuncCode) {
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
        printf("%s", funcString.second.c_str());
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
    printf("    CALL FUNC_main\n");
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
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    AND AX, BX");
    appendFunc(funcName, "    PUSH AX");
}


void CodeMgr::orStackTop(std::string funcName) {
    appendFunc(funcName, "    POP BX");
    appendFunc(funcName, "    POP AX");
    appendFunc(funcName, "    OR AX, BX");
    appendFunc(funcName, "    PUSH AX");
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
    for(int i = 0; i <= realString.length(); i ++) {
        appendFunc(funcName, "    MOV AX, " + std::to_string((   unsigned)realString[i]));
        appendFunc(funcName, "    MOV SI, " + std::to_string((offset + i) * 2));
        appendFunc(funcName, "    MOV [SI], AX"); // offset
    }
}


void CodeMgr::swapStack(std::string mFunction, int i, int j) {
    appendFunc(mFunction, "    MOV AX, [BP+" + std::to_string(i * 2) + "]");
    appendFunc(mFunction, "    MOV BX, [BP+" + std::to_string(j * 2) + "]");
    appendFunc(mFunction, "    MOV [BP+" + std::to_string(i * 2) + "], BX");
    appendFunc(mFunction, "    MOV [BP+" + std::to_string(j * 2) + "], AX");
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


