#ifndef FUNCGRAPH_H
#define FUNCGRAPH_H


#include <map>
#include <string>
#include <vector>


class FuncGraph
{
public:
	static FuncGraph& getInstance();
	
	bool        checkFuncExist (std::string funcName);
	void        addNewFunc     (std::string funcName);
	int         getFuncIdByName(std::string funcName);
	std::string getFuncNameById(int funcId);
	
	void addEdge  (std::string funcMain, std::string funcSub); // funcMain can reach funcSub
	void searchAll(std::string startPoint);
	
	bool checkFuncVisited(std::string funcName);
	
protected:
    FuncGraph(){mFuncCnt = 0; mVis.push_back(0);}
    void dfs(int nodeNowId);              // dfs from node now
    
    std::map<int, std::vector<int> > G;   // record the graph
    
    int mFuncCnt;
    std::vector<int>           mVis;      // check if a node is visited
    std::map<std::string, int> mFuncId;   // get function name from function id
    std::map<int, std::string> mFuncName; // get function id from function name
};

#endif
