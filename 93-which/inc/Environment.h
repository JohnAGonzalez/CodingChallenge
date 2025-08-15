#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include <string>
#include <vector>

class Environment
{
    public:
        Environment();
        std::vector<std::string> getPaths();
        void showPaths();

    protected:

    private:
        std::string _path;
        std::vector<std::string> _paths;

        void fetchPathFromEnv();
        void parsePaths();
};
#endif // __ENVIRONMENT_H
