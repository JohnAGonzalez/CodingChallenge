#ifndef __INPUTPROCESSOR_H
#define __INPUTPROCESSOR_H

// library includes
#include <fstream>

// project includes
#include "CommandLine.h"

// class definition
class InputProcessor
{
	public:
		// Constructor(s) and destructor
		InputProcessor(CommandLine& cmdLine);
		~InputProcessor();

		// public member functions
		std::istream& getInputStream();
		CommandLine * getCommandLine();

	private:
		CommandLine * _cmdLine;
		std::ifstream _inputFile;
};
#endif
