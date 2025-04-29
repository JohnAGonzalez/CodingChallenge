#include "InputProcessor.h"
#include <iostream>

InputProcessor::InputProcessor(CommandLine& cmdLine)
{
	_cmdLine = &cmdLine;

    if (_cmdLine->UsingFile() && !_cmdLine->InputFilenamePath().empty())
        _inputFile.open(_cmdLine->InputFilenamePath(), std::ios::in);
}

InputProcessor::~InputProcessor()
{
	_cmdLine = NULL;
    _inputFile.close();
}

std::istream& InputProcessor::getInputStream()
{
    if (_cmdLine->UsingFile())
        return _inputFile;
    else
	    return std::cin;
}

CommandLine * InputProcessor::getCommandLine()
{
	return _cmdLine;
}

