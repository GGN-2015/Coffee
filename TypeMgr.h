#ifndef TYPEMGR_H
#define TYPEMGR_H

#include <map>
#include <string>
#include <vector>

struct Member {
    std::vector<std::string> typeName;
    std::string name;
};

struct StructMsg {
    int length;
    std::vector<Member> memberList;
};

class TypeMgr // struct Mgr
{
public:
	static TypeMgr& getInstance();
	int getTypeLength(const std::vector<std::string>& typeVector, int posNow = 0);
	int getStructLength(std::string structName);
	bool isStruct(std::string structName) const;
	void createStruct(std::string structName, int line, int col);
	void addMember(std::string structName, 
        const std::vector<std::string>& typeVector, 
        std::string memberName, int line, int col);
        
    bool isMemberNameExist(std::string structName, std::string memberName);
    
    std::vector<std::string> getBaseType(std::vector<std::string> typeVector);
	
protected:
    TypeMgr() {}
    std::map<std::string, StructMsg> structMembers;
};

#endif
