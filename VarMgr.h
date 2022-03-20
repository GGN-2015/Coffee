#ifndef VARMGR_H
#define VARMGR_H


#include <vector>
#include <string>


struct VarPair {
    VarPair(std::string vn, int len): varName(vn), length(len){}
    std::string varName;
    int length;
};


typedef std::vector<VarPair> VarList;


struct FuncVar {               // record all the variable that a function has
    FuncVar(std::string name, const VarList& vl): funcName(name), varList(vl) {arguCnt = 0;}
    std::string funcName;      // funcName = "" means GLOBAL var
    VarList varList;
    int arguCnt;               // the first cnt var is argument
};


class VarMgr
{
public:
	static VarMgr& getInstance();
	void addVar(std::string funcName, std::string varName, int length, int line, int col);
	void addFunc(std::string funcName, int line, int col);
	
	void setFuncArguCnt(std::string funcName, int arguCnt);
	int  getVarOffset(std::string funcName, std::string varName, int line, int col);
	
	int  getFuncLocalVarCnt(std::string funcName) const;
	int  getFuncArguCnt(std::string funcName) const;
	
	bool existGlobalVar(std::string varName) const;
	bool existLocalVar(std::string funcName, std::string varName) const;
	int  getVarLength(std::string mFunctionName, std::string varName);
	
	bool existFunc(std::string funcName) const;
	bool existVar(const FuncVar& funcVar, std::string  varName) const;
	FuncVar& getFuncVar(std::string funcName);
	const FuncVar& getFuncVar(std::string funcName) const;
	
	void debugShow();
	
protected:
    VarMgr() {
        mAllFuncVar.push_back(FuncVar("", {}));
    }
    std::vector<FuncVar> mAllFuncVar;
};


#endif

