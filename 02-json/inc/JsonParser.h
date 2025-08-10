#ifndef __JSONPARSER_H
#define __JSONPARSER_H

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>

class JsonParser
{
    public:
        // preferred constructor
        JsonParser(std::istream & input);

        // public member functions
        void parse();
        bool isValid();
        
    private:
        std::istream & _input;
        std::string _inputText;
        bool _isValid;

        enum class TokenType {
           LBrace, RBrace,
           LBracket, RBracket,
           Colon, Comma,
           String, Number, True, False, Null,
           End, Invalid 
        };

        struct Token {
            TokenType type;
            std::string lexeme;
            size_t position;    // character offset from start of input
            size_t line;        // optional
            size_t column;      // optional
        };

        enum Symbol {
            LBRACE, RBRACE,
            LBRACKET, RBRACKET,
            COLON, COMMA,
            STRING, NUMBER, TRUE_SYM, FALSE_SYM, NULL_SYM,
            END_SYM,
            S, PAIRS, PAIR, VALUE 
        };

        enum ActionType { SHIFT, REDUCE, ACCEPT, ERROR_ACT };

        struct Rule {
            Symbol lhs;
            std::vector<Symbol> rhs;
        };

        struct Action {
            ActionType type;
            int target;
        };

        std::vector<Rule> rules;
        std::map<int, std::map<Symbol, Action>> actionTable;
        std::map<int, std::map<Symbol, int>> gotoTable;

        void initRules();
        void initParseTables();
        std::vector<Token> tokenize();
        std::string expectedTokens(int state) const;
        std::string symbolName(Symbol s) const;
};

#endif
