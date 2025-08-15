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

void Environment::showPaths()
{
    std::cout << "  Paths:" << std::endl;
    for (auto path : _paths)
    {
        std::cout << "    " << path << std::endl;
    }
}

void Environment::fetchPathFromEnv()
{
    _path = std::getenv("PATH");
}

void Environment::parsePaths()
{
    std::string path;
    size_t pos = 0;
    
    while (pos < _path.size())
    {
        // add the next character to the current path
        path += _path[pos++];
        
        // if we are at the end of a string, add it to the list
        if (_path[pos] == ':' || pos >= _path.size())
        {
            _paths.push_back(path);
            path.clear();
            ++pos;
        }
    }
}
