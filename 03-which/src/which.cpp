#include "which.h"

int main(int argc, char** argv)
{
    copyright();

    // capture the command-line parameters
    CommandLine cl(argc, argv);

    // show the command-line parameters
    cl.showParams();

    // get the path from the system environment
    Environment e;
    
    return 0;
}

void copyright()
{
    std::cout
        << std::endl
        << "which - Coding Challenge version" << std::endl
        << "Copyright (c) 2025 by John A. Gonzalez" << std::endl
        << std::endl;
}
