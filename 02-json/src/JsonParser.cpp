#include "JsonParser.h"

JsonParser::JsonParser(std::istream& input) : _input(input)
{
    _isValid = false;
    _look = _input.get();
    parse();
}

bool JsonParser::isValid()
{
    return _isValid;
}

void JsonParser::parse()
{
    // initialize the parser
    _token = Unrecognized;
    eatWhite();

    // enable the parse loop
    while (_look != EOF)
    {
        switch(_look)
        {
        case '{':
            {
                _token = OpenBrace;
                _tokens.push(_token);
            }
            break;

        case '}':
            {
                _token = _tokens.top();
                _tokens.pop();
                if (_token == OpenBrace)
                    _isValid = true;
            }
            break;

        default:
            {
                _look = _input.get();
            }
            break;
        }

        _look = _input.get();
        eatWhite();
    }
}

void JsonParser::eatWhite()
{
    while(isspace(_look))
        _look = _input.get();
}
