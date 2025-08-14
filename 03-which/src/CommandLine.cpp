#include "CommandLine.h"
#include <iostream>

CommandLine::CommandLine(int argc, char** argv) : _argc(argc), _argv(argv)
{
    init();
}

void CommandLine::init()
{
    for (int param = 1; param < _argc; ++param)
    {
        _params.push_back(std::string(_argv[param]));
    }
}

std::vector<std::string> CommandLine::getParams()
{
    return _params;
}

void CommandLine::showParams()
{
    for (auto param : _params)
    {
        std::cout << "  " << param << std::endl;
    }
}
