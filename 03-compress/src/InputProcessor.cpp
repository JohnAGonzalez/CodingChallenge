#include "InputProcessor.h"

#include <iostream>

InputProcessor::~InputProcessor()
{
    if (_fileToCompress.is_open())
        _fileToCompress.close();
}

void InputProcessor::init()
{
    _fileToCompress.open(_commandLine.getFileToCompress());
}


int InputProcessor::processInputStream()
{
    // check to make sure that the file is open and ready
    if (!_fileToCompress.good() || !_fileToCompress.is_open())
        return INPUT_READ_FAIL;

    // read in the contents of the file
    std::string contents(
        (std::istreambuf_iterator<char>(_fileToCompress)),
        std::istreambuf_iterator<char>());

    for (int i = 0; i < contents.length(); ++i)
    {
        char letter = contents[i];
        _frequencyMap[letter]++;
    }

    return INPUT_READ_SUCCESS;
}

void InputProcessor::showFrequencyMap()
{
    int colNo = 0;
    std::cout << "  ";

    for (const auto& [letter, frequency] : _frequencyMap)
    {
        std::cout << (letter < ' ' || letter > '~' ? '?' : letter) << "[" << frequency << "]\t";
        
        if (++colNo > 4)
        {
            std::cout << std::endl << "  ";
            colNo = 0;
        }
    }
}
