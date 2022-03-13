#include "ErrorReport.h"
#include "VarMgr.h"


VarMgr& VarMgr::getInstance() {
    static VarMgr vm;
    return vm;
}


void VarMgr::addVar(std::string funcName, std::string varName, int length, int line, int col) {
    if(existFunc(funcName)) {              // function exist
        FuncVar& funcVar = getFuncVar(funcName);
        if(!existVar(funcVar, varName)) { // can not find the var
            funcVar.varList.push_back(VarPair(varName, length));
        }else {                            // find the var
            ErrorReport::getInstance().send(
                true,
                "Semantic Error",
                "Var Name '" + varName + "' has been used in function '" + funcName + "'",
                line,
                col
            );
        }
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::addVar funcName = '" +  funcName + "' does not exist"
        );
    }
}


void VarMgr::addFunc(std::string funcName, int line, int col) {
    if(existFunc(funcName)) {
        ErrorReport::getInstance().send(
            true,
            "Semantic Error",
            "Functione Name '" + funcName + "' has been used in other function",
            line,
            col
        );
    }else {
        mAllFuncVar.push_back(
            FuncVar(funcName, {})
        );
    }
}


void VarMgr::setFuncArguCnt(std::string funcName, int arguCnt) {
    if(!existFunc(funcName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::setFuncArguCnt funcName '" +funcName+ "' does not exist"
        );
    }else {
        FuncVar& funcVar = getFuncVar(funcName);
        funcVar.arguCnt  = arguCnt;
    }
}


int VarMgr::getVarOffset(std::string funcName, std::string varName, int line, int col) {
    if(!existFunc(funcName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::getVarOffset funcName '" +funcName+ "' does not exist"
        );
    }else {
        FuncVar& funcVar = getFuncVar(funcName);
        if(!existVar(funcVar, varName)) {
            std::string prompt = "";
            if(funcName == "") {
                prompt = "GLOBAL VAR '" + varName + "' undefined";
            }else {
                prompt = "LOCAL VAR '" + varName + "' at function '" + funcName + "' undefined";
            }
            ErrorReport::getInstance().send(
                true,
                "Semantic Error",
                prompt,
                line,
                col
            );
        }
        int prelen = 0;
        for(auto& varPair: funcVar.varList) {
            if(varPair.varName == varName) {
                return prelen;
            }
            prelen += varPair.length;
        }
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::getVarOffset VAR '" + varName + "' got but can not find",
            line,
            col
        );
    }
}


bool VarMgr::existGlobalVar(std::string varName) const {
    const FuncVar& funcVar = getFuncVar("");
    return existVar(funcVar, varName);
}


bool VarMgr::existLocalVar(std::string funcName, std::string varName) const {
    const FuncVar& funcVar = getFuncVar(funcName);
    return existVar(funcVar, varName);
}


bool VarMgr::existFunc(std::string funcName) const {
    for(auto& funcVar: mAllFuncVar) {
        if(funcVar.funcName == funcName) {
            return true;
        }
    }
    return false;
}


FuncVar& VarMgr::getFuncVar(std::string funcName) {
    if(existFunc(funcName)) {
        for(auto& funcVar: mAllFuncVar) {
            if(funcVar.funcName == funcName) {
                return funcVar;
            }
        }
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::addVar funcName = '" +  funcName + "' does not exist"
        );
    }
}


const FuncVar& VarMgr::getFuncVar(std::string funcName) const {
    if(existFunc(funcName)) {
        for(const auto& funcVar: mAllFuncVar) {
            if(funcVar.funcName == funcName) {
                return funcVar;
            }
        }
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::addVar funcName = '" +  funcName + "' does not exist"
        );
    }
}


bool VarMgr::existVar(const FuncVar& funcVar, std::string  varName) const {
    for(const auto& varPair:funcVar.varList) {
        if(varPair.varName == varName) {
            return true;
        }
    }
    return false;
}


void VarMgr::debugShow() {
    for(auto& funcVar: mAllFuncVar) {
        std::string funcName = funcVar.funcName.c_str();
        if(funcName == "") {
            funcName = "GLOBAL";
        }
        printf("%10s: arguCnt = %5d\n", funcName.c_str(), funcVar.arguCnt);
        for(auto& varPair: funcVar.varList) {
            printf("    %5s: %5d\n", varPair.varName.c_str(), varPair.length);
        }
        printf("\n");
    }
}


int VarMgr::getVarLength(std::string funcName, std::string varName) {
    if(existFunc(funcName)) {
        FuncVar& funcVar = getFuncVar(funcName);
        for(auto& varPair: funcVar.varList) {
            if(varPair.varName == varName) {
                return varPair.length;
            }
        }
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "Function '" + funcName + "' does not have var called '" + varName + "'"
        );
        return -1;
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::getVarLength funcName = '" +  funcName + "' does not exist"
        );
    }
}


int VarMgr::getFuncLocalVarCnt(std::string funcName) const {
    if(!existFunc(funcName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::getFuncLocalVarCnt funcName = '" +  funcName + "' does not exist"
        );
    }else {
        const FuncVar& funcVar = getFuncVar(funcName);
        int ans = 0;
        for(auto& varPair: funcVar.varList) {
            ans += varPair.length;
        }
        return ans;
    }
}


int VarMgr::getFuncArguCnt(std::string funcName) const {
    if(!existFunc(funcName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "VarMgr::getFuncLocalVarCnt funcName = '" +  funcName + "' does not exist"
        );
    }else {
        const FuncVar& funcVar = getFuncVar(funcName);
        return funcVar.arguCnt;
    }
}
