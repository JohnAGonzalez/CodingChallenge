#ifndef __JSONPARSER_H
#define __JSONPARSER_H

#include <iostream>
#include <string>
#include <stack>

class JsonParser
{
    public:
        // preferred constructor
        JsonParser(std::istream& input);

        // public member functions
        void parse();
        bool isValid();

    private:
        // disable the default constructor
        JsonParser();

        // private member functions
        void eatWhite();

        // private data members
        enum Token
        {
            Unrecognized,
            OpenBrace,      // {
            CloseBrace,     // }
            End
        };

        std::istream&   _input;
        Token           _token;
        bool            _isValid;
        int             _look;

        std::stack<Token> _tokens;
};

#endif
