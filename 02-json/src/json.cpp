#include "json.h"
#include "CommandLine.h"
#include "InputProcessor.h"
#include "JsonParser.h"

int main(int argc, char** argv)
{
    copyright();

    // parse the command-line parameters
    CommandLine c(argc, argv);

    // prepare the appropriate input for processing
    InputProcessor i(c);

    // process the input and generate the output
    JsonParser jp(i.getInput());
    
    if (jp.isValid())
    {
        std::cerr << "   Input file IS valid JSON" << std::endl;
        return RETURN_SUCCESS;
    }
    else
    {
        std::cerr << "   Input file IS NOT valid JSON" << std::endl;
        return RETURN_FAIL;
    }
}


void copyright()
{
    std::cout
        << std::endl
        << "json - JSON Parser - Coding Challenge version" << std::endl
        << "Copyright (c) 2025 by John A. Gonzalez" << std::endl
        << std::endl;
}
