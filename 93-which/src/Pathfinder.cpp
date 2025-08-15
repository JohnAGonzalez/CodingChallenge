#include "PathFinder.h"

#include <iostream>
#include <sys/stat.h>

PathFinder::PathFinder(CommandLine & commandLine, Environment & environment) :
    _commandLine(commandLine), _environment(environment)
{
    findExecutables();
}

void PathFinder::findExecutables()
{
    std::vector<std::string> params = _commandLine.getParams();
    std::vector<std::string> paths = _environment.getPaths();

    for (auto path : paths)
    {
        for (auto param : params)
        {
            // make the file to find
            std::string fileToFind = path + "/" + param;
            struct stat buffer;

            // search for the existence of the file
            if (stat(fileToFind.c_str(), &buffer) == 0)
            {
                // if it is an executable binary
                if (buffer.st_mode & S_IXUSR)
                {
                    // then add it to the found list
                    _executables.push_back(fileToFind);
                }
            }
        }
    }
}

void PathFinder::showExecutables()
{
    for (auto executable : _executables)
    {
        std::cout << "  " << executable << std::endl;
    }
}
