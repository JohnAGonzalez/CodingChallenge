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
        std::string InputFilenamePath() { return _inputFilenamePath; };
        bool UsingFile() { return _usingFile; };
        bool OptionEmpty() { return _optionEmpty; };
        bool OptionBytes() { return _optionBytes; };
        bool OptionLines() { return _optionLines; };
        bool OptionWords() { return _optionWords; };
        bool OptionMultiBytes() { return _optionMultiBytes; };

    protected:

    private:
        CommandLine(); // do not allow use of the default contructor

        // member functions
        void init();
        void processInput();

        // private data members
        int _argc;
        char** _argv;
        std::string _inputFilenamePath;
        bool _usingFile;
        bool _optionEmpty;
        bool _optionBytes;
        bool _optionLines;
        bool _optionWords;
        bool _optionMultiBytes;

};

#endif
