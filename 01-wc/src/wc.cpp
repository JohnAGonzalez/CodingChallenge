#include "wc.h"

int main(int argc, char** argv)
{
    copyright();

    // parse the command-line parameters
    CommandLine c(argc, argv);

    // prepare the appropriate input for processing
    InputProcessor i(c);

    // process the input and generate the output
    TextProcessor p(i);
    p.go();

    return ERROR_SUCCESS;
}


void copyright()
{
    std::cout
        << std::endl
        << "wc - word count - Coding Challenge version" << std::endl
        << "Copyright (c) 2025 by John A. Gonzalez" << std::endl
        << std::endl;
}
