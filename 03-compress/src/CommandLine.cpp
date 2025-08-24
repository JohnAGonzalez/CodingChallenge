#include "CommandLine.h"

#include <sys/stat.h>

CommandLine::CommandLine(int argc, char** argv)
{
    for (int n = 1; n < argc; ++n)
    {
        struct stat buff;
        if (stat(argv[n], &buff) == 0)
        {
            _fileToCompress = argv[n];
            break;
        }
    }
}

bool CommandLine::fileIsValid()
{
    return _fileToCompress.length() > 0;
}
