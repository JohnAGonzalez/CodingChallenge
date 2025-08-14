#include "Environment.h"
#include <cstdlib>
#include <iostream>

Environment::Environment()
{
    fetchPathFromEnv();
    parsePaths();
}

std::vector<std::string> Environment::getPaths()
{
    return _paths;
}

void Environment::fetchPathFromEnv()
{
    _path = std::getenv("PATH");
    std::cout << "  PATH=[" << _path << "]" << std::endl;
}

void Environment::parsePaths()
{
}
