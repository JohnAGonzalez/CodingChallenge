#ifndef __COMMANDLINE_H
#define __COMMANDLINE_H

#include <string>
#include <vector>

class CommandLine
{
    public:
        CommandLine(int argc, char** argv);
        std::vector<std::string> getParams();
        void showParams();

    protected:
        CommandLine();

    private:
        // private member functions
        void init();
        
        // private data members
        int _argc;
        char** _argv;
        
        std::vector<std::string> _params;

};
#endif // __COMMANDLINE_H
