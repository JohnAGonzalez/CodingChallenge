#include "CommandLine.h"
#include <iostream>

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
    _inputFilenamePath.clear();
    _usingFile = false;
    _optionEmpty = true;
    _optionBytes = false;
    _optionLines = false;
    _optionWords = false;
    _optionMultiBytes = false;
}

void CommandLine::processInput()
{
    for (int argc = 1; argc < _argc; ++argc)
    {
        if (_argv[argc][0] == '-')
        {
            switch (_argv[argc][1])
            {
                case 'c':
                    _optionBytes = true;
                    _optionEmpty = false;
                    break;

                case 'l':
                    _optionLines = true;
                    _optionEmpty = false;
                    break;

                case 'w':
                    _optionWords = true;
                    _optionEmpty = false;
                    break;

                case 'm':
                    _optionMultiBytes = true;
                    _optionEmpty = false;
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (_inputFilenamePath.empty())
            {
                _inputFilenamePath = _argv[argc];
                _usingFile = true;
            }
        }
    }
}
