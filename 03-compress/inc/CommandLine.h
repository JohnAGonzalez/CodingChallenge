#ifndef ___COMPRESS__COMMANDLINE_H
#define ___COMPRESS__COMMANDLINE_H

#include <string>

class CommandLine
{
    public:
        CommandLine(int argc, char** argv);
        bool fileIsValid();
        std::string getFileToCompress() { return _fileToCompress; };

    protected:
        CommandLine();

    private:
        std::string _fileToCompress;
};

#endif // ___COMPRESS__COMMANDLINE_H
