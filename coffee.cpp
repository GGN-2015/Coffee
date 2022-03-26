// ---------------------------------------------------------------------- //
// coffee.cpp - Version
#define VERSION "0.0.1"


//
// Author: GGN_2015 (JLU #21200612)
// Date  : 2022-03-11


#include <iostream>
#include <string>


#include "CodeMgr.h"
#include "ErrorReport.h"
#include "ProgramReader.h"
#include "VarMgr.h"


// ---------------------------------------------------------------------- //
// Compile parameters
#define DEBUG_MODE         (true) // DEBUG_MODE means with output even compile succeeded
#define DEFAULT_STACK_SIZE (32768)


int main(int argc, char** argv) {
    ErrorReport  & er = ErrorReport   :: getInstance();
    ProgramReader& pr = ProgramReader :: getInstance(); // if there is no instance, auto create one
    VarMgr       & vm = VarMgr        :: getInstance();
    CodeMgr      & cm = CodeMgr       :: getInstance();
    std::string fileName = "";
    int stackSize = (DEFAULT_STACK_SIZE + 1) / 2;
    for(int i = 1; i < argc; i ++) {
        if(std::string(argv[i]).substr(0, 2) == "-O") {
            cm.setOptimizeGrade(atoi(std::string(argv[i]).substr(2).c_str()));
        }else 
        if(std::string(argv[i]).substr(0, 2) == "-S") {
            stackSize = (atoi(std::string(argv[i]).substr(2).c_str()) + 1) / 2;
            // set the size of the memory
            if(stackSize > 64 * 1024 || stackSize < 0) {
                er.send(
                    true,
                    "Parameter Error",
                    "-SstackSize: stackSize must under 64KiB and above 0 but get " + std::string(argv[i]).substr(2)
                );
            }
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
        if(DEBUG_MODE) {
            std::cerr << std::boolalpha; // output bool in ALPHA mode
            std::cerr << "-----DEBUG MODE: " << DEBUG_MODE << std::endl;
            std::cerr << "-COFFEE VERSION: " << VERSION    << std::endl;
            std::cerr << "-------FILENAME: " << fileName   << std::endl;
            std::cerr << std::endl;
        }
        pr.open(fileName);
        // pr.debugOutput(); // show  full text of the input program
        // pr.debugParser(); // show  all tokens of the input program
        // pr.debugBrace();
        pr.compile();
        cm.outputCode(stackSize);
        // vm.debugShow();
        if(DEBUG_MODE) {
            std::cerr << "---------STATUS: " << "FINISHED" << std::endl;
            std::cerr << std::endl;
        }
    }
    ProgramReader::clearAllInstance();
    return 0;
}

