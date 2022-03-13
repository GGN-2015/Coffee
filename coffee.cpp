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
    ProgramReader& pr = ProgramReader :: getInstance();
    VarMgr       & vm = VarMgr        :: getInstance();
    CodeMgr      & cm = CodeMgr       :: getInstance();
    if(argc <= 1) {
        er.send(
            true,
            "File Error",
            "No Input File"
        );
    }else {
        std::string fileName = std::string(argv[1]);
        pr.open(fileName);
        // pr.debugOutput(); // show  full text of the input program
        // pr.debugParser(); // show  all tokens of the input program
        // pr.debugBrace();
        pr.compile();
        cm.outputCode(16384);
        // vm.debugShow();
    }
    return 0;
}

