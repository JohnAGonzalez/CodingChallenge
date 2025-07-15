#include "InputProcessor.h"
#include <iostream>

InputProcessor::~InputProcessor()
{
}

void InputProcessor::init()
{
    if (_commandLine.getFilename().length() > 0)
        _inputFile.open(_commandLine.getFilename());
}

bool InputProcessor::isValid()
{
    return _inputFile.good() && _inputFile.is_open();
}

std::ifstream & InputProcessor::getInput()
{
    return _inputFile;
}
