#include "JsonParser.h"

JsonParser::JsonParser(std::istream & input) : _input(input)
{
    _isValid = false;
    initRules();
    initParseTables();
    parse();
}

void JsonParser::initRules()
{
    rules = {
        { S,     { LBRACE, PAIRS, RBRACE } },
        { S,     { LBRACE, RBRACE } },
        { PAIRS, { PAIR } },
        { PAIRS, { PAIR, COMMA, PAIRS } },
        { PAIR,  { STRING, COLON, VALUE } },
        { VALUE, { STRING } },
        { VALUE, { NUMBER } },
        { VALUE, { TRUE_SYM } },
        { VALUE, { FALSE_SYM } },
        { VALUE, { NULL_SYM } }
    };
}

void JsonParser::initParseTables()
{
    actionTable[0][LBRACE] = { SHIFT, 1 };
    gotoTable[0][S] = 2;

    actionTable[1][STRING] = { SHIFT, 3 };
    actionTable[1][RBRACE] = { SHIFT, 4 };
    gotoTable[1][PAIRS] = 5;
    gotoTable[1][PAIR] = 6;

    actionTable[2][END_SYM] = { ACCEPT, 0 };

    actionTable[3][COLON] = { SHIFT, 7 };

    actionTable[4][END_SYM] = { REDUCE, 1 };

    actionTable[5][RBRACE] = { SHIFT, 8 };

    actionTable[6][COMMA] = { SHIFT, 9 };
    actionTable[6][RBRACE] = { REDUCE, 2 };

    actionTable[7][STRING] = { SHIFT, 10 };
    actionTable[7][NUMBER] = { SHIFT, 11 };
    actionTable[7][TRUE_SYM] = { SHIFT, 12 };
    actionTable[7][FALSE_SYM] = { SHIFT, 13 };
    actionTable[7][NULL_SYM] = { SHIFT, 14 };
    gotoTable[7][VALUE] = 15;

    actionTable[8][END_SYM] = { ACCEPT, 0 };
    
    actionTable[9][STRING] = { SHIFT, 3 };
    gotoTable[9][PAIR] = 16;
    gotoTable[9][PAIRS] = 17;

    actionTable[10][RBRACE] = { REDUCE, 5 };
    actionTable[10][COMMA]  = { REDUCE, 5 };
    actionTable[11][RBRACE] = { REDUCE, 6 };
    actionTable[11][COMMA]  = { REDUCE, 6 };
    actionTable[12][RBRACE] = { REDUCE, 7 };
    actionTable[12][COMMA]  = { REDUCE, 7 };
    actionTable[13][RBRACE] = { REDUCE, 8 };
    actionTable[13][COMMA]  = { REDUCE, 8 };
    actionTable[14][RBRACE] = { REDUCE, 9 };
    actionTable[14][COMMA]  = { REDUCE, 9 };

    actionTable[15][COMMA] = { REDUCE, 4 };
    actionTable[15][RBRACE] = { REDUCE, 4 };

    actionTable[16][COMMA] = { SHIFT, 9 };
    actionTable[16][RBRACE] = { REDUCE, 2 };

    actionTable[17][RBRACE] = { REDUCE, 3 };
}

void JsonParser::parse()
{
    auto tokens = tokenize();

    std::vector<int> stateStack = { 0 };
    std::vector<Symbol> symbolStack;

    size_t pos = 0;
    while(true) {
        int state = stateStack.back();
        Symbol input = (pos < tokens.size()) ? static_cast<Symbol>(tokens[pos].type) : END_SYM;

        auto it = actionTable[state].find(input);
        if (it == actionTable[state].end()) {
            return;
        }

        Action action = it->second;

        if (action.type == SHIFT) {
            stateStack.push_back(action.target);
            symbolStack.push_back(input);
            ++pos;
        } else if (action.type == REDUCE) {
            const Rule& rule = rules[action.target];
            for (size_t i = 0; i < rule.rhs.size(); ++i) {
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            Symbol lhs = rule.lhs;
            int newState = gotoTable[stateStack.back()][lhs];
            symbolStack.push_back(lhs);
            stateStack.push_back(newState);
        } else if (action.type == ACCEPT) {
            _isValid = true;
            return;
        } else {
            return;
        }
    }
}

std::vector<JsonParser::Token> JsonParser::tokenize()
{
    std::vector<Token> tokens;
    size_t i = 0;

    std::string input((std::istreambuf_iterator<char>(_input)), std::istreambuf_iterator<char>());

    auto consume_string = [&]() -> std::string {
        size_t start = ++i;
        while (i < input.size() && input[i] != '"')
            ++i;
        std::string val = input.substr(start, i - start);
        ++i;
        return val;
    };

    auto consume_number = [&]() -> std::string {
        size_t start = i;
        if (input[i] == '-')
            ++i;
        while (i < input.size() && (isdigit(input[i]) || input[i] == '.'))
            ++i;
        return input.substr(start, i - start);
    };

    while (i < input.size()) {
        if (isspace(input[i])) {
            ++i;
        } else if (input[i] == '{') {
            tokens.push_back({ TokenType::LBrace, "{"});
            ++i;
        } else if (input[i] == '}') {
            tokens.push_back({ TokenType::RBrace, "}"});
            ++i;
        } else if (input[i] == ':') {
            tokens.push_back({ TokenType::Colon, ":"});
            ++i;
        } else if (input[i] == ',') {
            tokens.push_back({ TokenType::Comma, ","});
            ++i;
        } else if (input[i] == '"') {
            std::string str = consume_string();
            tokens.push_back({ TokenType::String, str});
        } else if (isdigit(input[i]) || input[i] == '-') {
            std::string num = consume_number();
            tokens.push_back({ TokenType::Number, num});
        } else if (input.compare(i, 4, "true") == 0) {
            tokens.push_back({ TokenType::True, "true" });
            i += 4;
        } else if (input.compare(i, 5, "false") == 0) {
            tokens.push_back({ TokenType::False, "false" });
            i += 5;
        } else if (input.compare(i, 4, "null") == 0) {
            tokens.push_back({ TokenType::Null, "null" });
            i += 4;
        } else {
            tokens.push_back({ TokenType::Invalid, std::string(1, input[i++]) });
        }
    }

    tokens.push_back({ TokenType::End, ""});
    return tokens;
}

bool JsonParser::isValid()
{
    return _isValid;
}

