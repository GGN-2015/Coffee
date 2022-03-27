#include "ErrorReport.h"
#include "FuncGraph.h"


FuncGraph& FuncGraph::getInstance() { // static singleton
    static FuncGraph fg;
    return fg;
}


bool FuncGraph::checkFuncExist(std::string funcName) {
    return mFuncId.count(funcName);    
}


void FuncGraph::addNewFunc(std::string funcName) {
    int idNow = ++ mFuncCnt; // begin with 1
    mFuncName [idNow] = funcName;
    mFuncId[funcName] = idNow;
    G[idNow] = {};
    mVis.push_back(0);
}


int FuncGraph::getFuncIdByName(std::string funcName) {
    if(!checkFuncExist(funcName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "FuncGraph::getFuncIdByName funcName = " + funcName + " not found"
        );
    }
    return mFuncId[funcName];
}


std::string FuncGraph::getFuncNameById(int funcId) {
    return mFuncName[funcId];
}


void FuncGraph::addEdge(std::string funcMain, std::string funcSub) { // funcMain can reach funcSub
    int idMain = getFuncIdByName(funcMain);
    int idSub  = getFuncIdByName(funcSub );
    G[idMain].push_back(idSub);
}


void FuncGraph::searchAll(std::string startPoint) {
    for(int i = 0; i < mVis.size(); i ++) {
        mVis[i] = 0;
    }
    int idMain = getFuncIdByName(startPoint);
    dfs(idMain);
}


void FuncGraph::dfs(int nodeNowId) { // flood fill
    if(mVis[nodeNowId]) return;
    mVis[nodeNowId] = true;
    for(auto& nxtNode: G[nodeNowId]) {
        if(!mVis[nxtNode]) {
            dfs(nxtNode);
        }
    }
}


bool FuncGraph::checkFuncVisited(std::string funcName) {
    int funcId = getFuncIdByName(funcName);
    return mVis[funcId];
}
