#include <cstdlib>


#include "ErrorReport.h"
#include "TypeMgr.h"


TypeMgr& TypeMgr::getInstance() {
    static TypeMgr tm;
    return tm;
}


int TypeMgr::getTypeLength(const std::vector<std::string>& typeVector, int posNow) { // Unit: Word
    // WE ASSUME THAT THE TYPE_VECTOR IS CORRECT
    if(typeVector[posNow] == "INT" || typeVector[posNow] == "POINTER") {
        return 1;
    }else if(typeVector[posNow] == "ARRAY") {
        return atoi(typeVector[posNow + 1].c_str()) * getTypeLength(typeVector, posNow + 2);
    }else if(typeVector[posNow] == "STRUCT") {
        return getStructLength(typeVector[posNow + 1]);
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "TypeMgr::getTypeLength typeVector front = '" +typeVector[posNow]+ "' unknown"
        );
        return 0;
    }
}


int TypeMgr::getStructLength(std::string structName) {
    if(!isStruct(structName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "TypeMgr::getStructLength string '" +structName+ "' is not a struct name"
        );
    }
    return structMembers[structName].length;
}


bool TypeMgr::isStruct(std::string structName) const {
    return structMembers.count(structName) >= 1;
}


void TypeMgr::createStruct(std::string structName, int line, int col) {
    if(isStruct(structName)) {
        ErrorReport::getInstance().send(
            true,
            "Semantic Error",
            "struct Name '" + structName + "' redefined",
            line,
            col
        );
    }
    structMembers[structName] = {0, {}};
}


void TypeMgr::addMember(std::string structName, 
    const std::vector<std::string>& typeVector, 
    std::string memberName, int line, int col
    ) {
    if(isMemberNameExist(structName, memberName)) {
        ErrorReport::getInstance().send(
            true,
            "Syntax Error",
            "member name '" +memberName+ "' for struct '" +structName+ "' redefined",
            line,
            col
        );
    }
    structMembers[structName].memberList.push_back(
        Member{typeVector, memberName}
    );
}


bool TypeMgr::isMemberNameExist(std::string structName, std::string memberName) {
    if(!isStruct(structName)) {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "TypeMgr::isMemberNameExist structName '" +structName+ "' not exist"
        );
    }
    for(Member& member: structMembers[structName].memberList) {
        if(member.name == memberName) {
            return true;
        }
    }
    return false;
}


std::vector<std::string> TypeMgr::getBaseType(std::vector<std::string> typeVector) {
    if(typeVector[0] == "ARRAY") {
        typeVector.erase(typeVector.begin(), typeVector.begin() + 2); // erase first 2 value
        return typeVector;
    }else if(typeVector[0] == "POINTER") {
        typeVector.erase(typeVector.begin(), typeVector.begin() + 1); // erase first value
        return typeVector;
    }else {
        ErrorReport::getInstance().send(
            true,
            "Inner Error",
            "Can not Get BaseType for TYPE begin with '" +typeVector[0]+ "'"
        );
    }
}


