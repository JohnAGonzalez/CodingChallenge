#ifndef __INPUTPROCESSOR_H
#define __INPUTPROCESSOR_H

#include "CommandLine.h"

class InputProcessor
{
    public:
        InputProcessor(CommandLine & cl) : _commandLine(cl);
        ~InputProcessor();

    private:
        // disallow the use of the default constructor
        InputProcessor();

        // private data members
        CommandLine & _commandLine;

};

#endif
