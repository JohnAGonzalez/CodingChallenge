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
    _filename.clear();
}

void CommandLine::processInput()
{
    // first, let's make sure that the file exists
    std::string filename = _argv[1];
    std::ifstream file(filename);

    // only keep it if the file exists
    if (file.good())      
        _filename = filename;
}
