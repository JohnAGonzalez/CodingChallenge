#ifndef __INPUTPROCESSOR_H
#define __INPUTPROCESSOR_H

#include "CommandLine.h"
#include <fstream>

class InputProcessor
{
    public:
        InputProcessor(CommandLine & cl) : _commandLine(cl) { init(); };
        ~InputProcessor();

        bool isValid();
        std::ifstream & getInput();

    private:
        // disallow the use of the default constructor
        InputProcessor();
        void init();

        // private data members
        CommandLine & _commandLine;
        std::ifstream _inputFile;

};

#endif
