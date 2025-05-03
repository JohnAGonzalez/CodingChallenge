#include "CommandLine.h"
#include <fstream>

CommandLine::CommandLine(int argc, char** argv) : _argc(argc), _argv(argv)
{
    init();
    processInput();
}

CommandLine::~CommandLine()
{
    init();
}

void CommandLine::init()
{
    _fileList.clear();
}

void CommandLine::processInput()
{
    for(int argc = 1; argc < _argc; ++argc)
    {
        // first, let's make sure that the file exists
        std::string filename = _argv[argc];
        std::ifstream file(filename);
        if (file.good())      
            _fileList.push_back(filename);
    }
}
