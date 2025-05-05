#ifndef __COMMANDLINE_H
#define __COMMANDLINE_H

// library includes
#include <string>

// class definition
class CommandLine
{
    public:
        // Constructors and destructor
        CommandLine(int argc, char** argv);
        ~CommandLine();

        // accessors
        std::string getFilename() { return _filename; };

    protected:

    private:
        CommandLine(); // do not allow use of the default contructor

        // member functions
        void init();
        void processInput();

        // private data members
        int _argc;
        char** _argv;

        std::string _filename;

};

#endif
