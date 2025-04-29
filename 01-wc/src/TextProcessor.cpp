#include "TextProcessor.h"
#include <iomanip>
#include <clocale>
#include <string_view>


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
    _byteCount = 0;
    _lineCount = 0;
    _wordCount = 0;
    _multiByteCount = 0;
}

int TextProcessor::go()
{
    if (_input->getCommandLine()->OptionBytes() || _input->getCommandLine()->OptionEmpty())
        processBytes();

    if (_input->getCommandLine()->OptionLines() || _input->getCommandLine()->OptionEmpty())
        processLines();

	if (_input->getCommandLine()->OptionWords() || _input->getCommandLine()->OptionEmpty())
        processWords();

    if (_input->getCommandLine()->OptionMultiBytes())
        processMultiBytes();
    
    showResults();

    return 0;
}

void TextProcessor::showResults()
{
    if (_input->getCommandLine()->OptionLines() || _input->getCommandLine()->OptionEmpty())
    {
        std::cout
            << std::setw(8)
            << _lineCount - 1;
    }
    if (_input->getCommandLine()->OptionBytes() || _input->getCommandLine()->OptionEmpty())
    {
        std::cout
            << std::setw(8)
            << _byteCount - 1;
    }
    if (_input->getCommandLine()->OptionWords() || _input->getCommandLine()->OptionEmpty())
    {
        std::cout
            << std::setw(8)
            << _wordCount - 1;
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
    _multiByteCount = 0;

    for (unsigned char byte: str)
    {
        if ((byte & 0b11000000) != 0b10000000)
        {
            ++_multiByteCount;
        }
    }
}
