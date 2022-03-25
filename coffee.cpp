// ---------------------------------------------------------------------- //
// coffee.cpp - Version 0.0.1
//
// Author: GGN_2015 (JLU #21200612)
// Date  : 2022-03-11


#include <string>


#include "CodeMgr.h"
#include "ErrorReport.h"
#include "ProgramReader.h"
#include "VarMgr.h"


// ---------------------------------------------------------------------- //
// Compile parameters
#define DEBUG_MODE (true) // DEBUG_MODE means with output even compile succeeded


int main(int argc, char** argv) {
    ErrorReport  & er = ErrorReport   :: getInstance();
    ProgramReader& pr = ProgramReader :: getInstance(); // if there is no instance, auto create one
    VarMgr       & vm = VarMgr        :: getInstance();
    CodeMgr      & cm = CodeMgr       :: getInstance();
    std::string fileName = "";
    for(int i = 1; i < argc; i ++) {
        if(std::string(argv[i]).substr(0, 2) == "-O") {
            cm.setOptimizeGrade(atoi(std::string(argv[i]).substr(2).c_str()));
        }else {
            fileName = argv[i];
        }
    }
    if(fileName == "") {
        er.send(
            true,
            "File Error",
            "No Input File"
        );
    }else {
        pr.open(fileName);
        // pr.debugOutput(); // show  full text of the input program
        // pr.debugParser(); // show  all tokens of the input program
        // pr.debugBrace();
        pr.compile();
        cm.outputCode(16384);
        // vm.debugShow();
    }
    ProgramReader::clearAllInstance();
    return 0;
}

