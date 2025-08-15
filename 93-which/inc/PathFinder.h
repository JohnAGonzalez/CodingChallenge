#ifndef __PATHFINDER_H
#define __PATHFINDER_H

#include "CommandLine.h"
#include "Environment.h"

#include <string>
#include <vector>

class PathFinder
{
    public:
        PathFinder(CommandLine & commandLine, Environment & environment);
        void findExecutables();
        void showExecutables();

    protected:
        PathFinder();

    private:
        CommandLine & _commandLine;
        Environment & _environment;
        std::vector<std::string> _executables;

};

#endif // __PATHFINDER_H
