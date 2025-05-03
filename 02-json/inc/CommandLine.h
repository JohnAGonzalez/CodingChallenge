#ifndef __COMMANDLINE_H
#define __COMMANDLINE_H

// library includes
#include <string>
#include <vector>

// file list typedefs
typedef std::vector<std::string> FileList;
typedef FileList::iterator FileList_iter;

// class definition
class CommandLine
{
    public:
        // Constructors and destructor
        CommandLine(int argc, char** argv);
        ~CommandLine();

        // accessors
        FileList getFileList() { return _fileList; };

    protected:

    private:
        CommandLine(); // do not allow use of the default contructor

        // member functions
        void init();
        void processInput();

        // private data members
        int _argc;
        char** _argv;

        FileList _fileList;

};

#endif
