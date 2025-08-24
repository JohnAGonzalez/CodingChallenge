#include "main.h"

#include <iostream>

int main(int argc, char** argv)
{
    copyright();

    CommandLine cl(argc, argv);
    if (!cl.fileIsValid())
    {
        std::cerr << "  Error: input file does not exist." << std::endl;
        return RETURN_FILE_NOT_EXIST;
    }

    InputProcessor fp(cl);
    if (fp.processInputStream() == INPUT_READ_FAIL)
    {
        std::cerr << "  Error: failed to read input file." << std::endl;
        return RETURN_INPUT_READ_FAIL;
    }

    fp.showFrequencyMap();

    return RETURN_SUCCESS;
}

void copyright()
{
    std::cout
        << std::endl
        << "compress - Huffman Compression - Coding Challenge version" << std::endl
        << "Copyright (c) 2025 by John A. Gonzalez" << std::endl
        << std::endl;
}
