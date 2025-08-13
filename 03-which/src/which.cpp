#include "which.h"

int main(int argc, char** argv)
{
    // capture the command-line parameters
    CommandLine cl(argc, argv);

    // show the command-line parameters
    std::vector<std::string> params = cl.getParams();
    for (auto param : params)
    {
        std::cout << "  " << param << std::endl;
    }

    return 0;
}
