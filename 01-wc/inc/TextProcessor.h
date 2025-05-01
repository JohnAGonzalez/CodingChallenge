#ifndef __TEXTPROCESSOR_H
#define __TEXTPROCESSOR_H

// library includes
#include <iostream>

// project includes
#include "InputProcessor.h"

// class definition
class TextProcessor
{
	public:
		// constructors & destructor
		TextProcessor(InputProcessor& input);
		~TextProcessor();

		// public member functions
		int go();

	private:
        // default constructor
        TextProcessor();

        // private data members
		InputProcessor * _input;
        unsigned int _byteCount;
        unsigned int _lineCount;
        unsigned int _wordCount;
        unsigned int _multiByteCount;

        // private member functions
        void init();
        void resetCounts();
        void processInput();
        void processBytes();
        void processLines();
        void processWords();
        void processMultiBytes();
        void showResults();

};
#endif
