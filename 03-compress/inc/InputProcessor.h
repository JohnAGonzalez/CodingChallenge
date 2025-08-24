#ifndef ___COMPRESS__INPUTPROCESSOR_H
#define ___COMPRESS__INPUTPROCESSOR_H

#include "CommandLine.h"

#include <fstream>
#include <map>
#include <string>

#define INPUT_READ_SUCCESS  0
#define INPUT_READ_FAIL     1

class InputProcessor
{
    public:
        // constructor
        InputProcessor(CommandLine & commandLine) : _commandLine(commandLine) { init(); };

        // destructor
        ~InputProcessor();

        // public member functions
        int processInputStream();
        void showFrequencyMap();

    protected:
        // don't allow the default constructor
        InputProcessor();

    private:
        // private data members
        CommandLine  _commandLine;
        std::ifstream _fileToCompress;
        std::map<char, int> _frequencyMap;

        // private member functions
        void init();

};

#endif // ___COMPRESS__INPUTPROCESSOR_H
