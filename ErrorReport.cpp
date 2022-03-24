#include <cstdio>
#include <iostream>


#include "ErrorReport.h"
#include "ProgramReader.h"


ErrorReport& ErrorReport::getInstance() {
    static ErrorReport er;
    return er;
}


void ErrorReport::send
    (
        bool fatal, 
        std::string errorType, 
        std::string errorPrompt,   // you do not need to set an '\n' in prompt
        int line,
        int col
    ) {
    using std::cerr;
    using std::endl;
    if(fatal) {
        cerr << "FATAL: ";   // if fatal, you need to stop compile
    }else {
        cerr << "WARNING: "; // if warning, compile can continue
    }
    cerr << errorType << ": ";
    cerr << errorPrompt << endl;
    while(!ProgramReader::objStack.empty()) {
        if(line != -1) {
            fprintf(stderr, "    FILE       | %s\n", ProgramReader::getInstance().getFileName().c_str());
            fprintf(stderr, "    LINE %5d | ", line);
            cerr << ProgramReader::getInstance().getLine(line) << endl;
            fprintf(stderr, "  COLUMN %5d | ", col);
            while(col --) {
                cerr << " ";
            }
            cerr << "^" << endl << endl;
        }
        ProgramReader::clearInstance();
        if(!ProgramReader::objStack.empty()) {
            line = ProgramReader::getInstance().getLineNow();
            col  = ProgramReader::getInstance().getColumnNow();
        }
    }
    if(fatal) endProgram();
}


void ErrorReport::endProgram(int errorlevel) {
    exit(errorlevel);
}

