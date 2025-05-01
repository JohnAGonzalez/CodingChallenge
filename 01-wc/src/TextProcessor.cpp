#include "TextProcessor.h"
#include <iomanip>
#include <clocale>
#include <string_view>
#include <cctype>


TextProcessor::TextProcessor(InputProcessor& input)
{
    init();
	_input = &input;
}

TextProcessor::~TextProcessor()
{
	init();
}

void TextProcessor::init()
{
    _input = NULL;

    resetCounts();
}

void TextProcessor::resetCounts()
{
    _byteCount = 0;
    _lineCount = 0;
    _wordCount = 0;
    _multiByteCount = 0;
}

int TextProcessor::go()
{
    processInput();
    
    showResults();

    return 0;
}

void TextProcessor::processInput()
{
    resetCounts();

    std::istream & in = _input->getInputStream();
    std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    bool inWord = false;

    for (unsigned char byte: str)
    {
        // count up bytes 
        ++_byteCount;

        // count up lines
        if (byte == '\r')
            ++_lineCount;

        // count up words
        if (std::isspace(byte))
        {
            if (inWord)
                inWord = false;
        }
        else
        {
            if (!inWord)
            {
                inWord = true;
                ++_wordCount;
            }
        }

        // count up multi-byte characters
        if ((byte & 0b11000000) != 0b10000000)
        {
            ++_multiByteCount;
        }
    }
}

void TextProcessor::showResults()
{
    if (_input->getCommandLine()->OptionLines() || _input->getCommandLine()->OptionEmpty())
    {
        std::cout
            << std::setw(8)
            << _lineCount;
    }
    if (_input->getCommandLine()->OptionWords() || _input->getCommandLine()->OptionEmpty())
    {
        std::cout
            << std::setw(8)
            << _wordCount;
    }
    if (_input->getCommandLine()->OptionBytes() || _input->getCommandLine()->OptionEmpty())
    {
        std::cout
            << std::setw(8)
            << _byteCount;
    }
    if (_input->getCommandLine()->OptionMultiBytes())
    {
        std::cout
            << std::setw(8)
            << _multiByteCount;
    }
    
    std::cout
        << "  "
        << _input->getCommandLine()->InputFilenamePath()
        << std::endl;
}

void TextProcessor::processBytes()
{
    std::istream * in = &_input->getInputStream();

    while (!in->eof())
    {
        in->get();
        ++_byteCount;
    }
}

void TextProcessor::processLines()
{
    std::istream * in = &_input->getInputStream();
    std::string line;

    while (!in->eof())
    {
        std::getline(*in, line);
        ++_lineCount;
    }
}

void TextProcessor::processWords()
{
    std::istream * in = &_input->getInputStream();
    std::string line;

    while (!in->eof())
    {
        *in >> line;
        ++_wordCount;
    }
}

void TextProcessor::processMultiBytes()
{
    std::istream & in = _input->getInputStream();
    std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    for (unsigned char byte: str)
    {
        if ((byte & 0b11000000) != 0b10000000)
        {
            ++_multiByteCount;
        }
    }
}
