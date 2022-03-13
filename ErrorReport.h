#ifndef ERRORREPORT_H
#define ERRORREPORT_H


#include <string>


class ErrorReport
{
public:
    static ErrorReport& getInstance();
    void send
    (
        bool fatal, 
        std::string errorType, 
        std::string errorPrompt, 
        int line = -1,                  // -1 means don't need to output the program text
        int col  = -1
    );
    
protected:
    ErrorReport() {}
    void endProgram(int errorlevel = 1);
};

#endif

