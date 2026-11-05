#include "JsonParser.h"

#include <sstream>

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
        { VALUE, { NULL_SYM } },
        { VALUE, { LBRACE, RBRACE } },
        { VALUE, { LBRACKET, RBRACKET } },
        { VALUE, { LBRACE, PAIRS, RBRACE } },
        { VALUE, { LBRACKET, VALUE, RBRACKET } }
    };
}

/*
  ──────────────────────────────────────────────────────────────────────────────
  Shift-Reduce Tables Cheat Sheet
  ──────────────────────────────────────────────────────────────────────────────
  Notation:
    actionTable[state][terminal] = { SHIFT, N }   // "sN"  shift to state N
    actionTable[state][terminal] = { REDUCE, R }  // "rR"  reduce by rule R
    actionTable[state][terminal] = { ACCEPT, 0 }  // "acc" accept
    gotoTable[state][nonterminal] = N             // "gN"  goto state N

  Subscripts:
    • First subscript (row) = current LR parser STATE (the DFA state on top of the
      state stack).
    • Second subscript (column) = lookahead symbol:
        - ACTION table uses TERMINALS (tokens).
        - GOTO table uses NON-TERMINALS (grammar symbols).

  Reduction semantics:
    On REDUCE by rule R: pop |RHS| symbols/states, then push LHS and
    transition via gotoTable[ stateStack.back() ][ LHS ].

  Grammar (rule numbers):
    0: S     → { PAIRS }
    1: S     → { }
    2: PAIRS → PAIR
    3: PAIRS → PAIR , PAIRS
    4: PAIR  → STRING : VALUE
    5: VALUE → STRING
    6: VALUE → NUMBER
    7: VALUE → TRUE
    8: VALUE → FALSE
    9: VALUE → NULL
   10: VALUE → { }
   11: VALUE → [ ]
   12: VALUE → { PAIRS }
   13: VALUE → [ VALUE ]

  State mnemonics:
    0  = start
    1  = after '{' of a top-level object
    2  = S recognized; expecting END -> accept
    3  = after STRING used as a key; expecting ':'
    4  = saw '{}' at top level; reduce S → { }
    5  = inside top-level object after computing PAIRS; expecting '}'
    6  = after first PAIR in object (no comma consumed yet)
    7  = after ':'; expecting a VALUE (string/number/true/false/null/'{'/'[')
    8  = saw top-level '}' -> accept on END
    9  = after comma between pairs; expecting next PAIR
    10 = VALUE pending: STRING (reduce on ',', '}', or ']')
    11 = VALUE pending: NUMBER (reduce on ',', '}', or ']')
    12 = VALUE pending: TRUE   (reduce on ',', '}', or ']')
    13 = VALUE pending: FALSE  (reduce on ',', '}', or ']')
    14 = VALUE pending: NULL   (reduce on ',', '}', or ']')
    15 = after VALUE (PAIR is complete) -> reduce PAIR on ',', '}'
    16 = in PAIRS after one PAIR in a comma context (either shift ',' or end PAIRS)
    17 = PAIRS tail recognized (PAIRS → PAIR , PAIRS)
    18 = VALUE starts with '{' (object-as-value), first key expected
    19 = saw '{}' in value context -> reduce VALUE → { }
    20 = VALUE starts with '[' (array-as-value)
    21 = saw '[]' in value context -> reduce VALUE → [ ]
    22 = inside object-as-value after computing PAIRS; expecting '}'
    23 = saw '}' closing object-as-value -> reduce VALUE → { PAIRS }
    24 = inside array-as-value after reading one VALUE; expecting ']'
    25 = saw ']' closing single-element array -> reduce VALUE → [ VALUE ]
*/
void JsonParser::initParseTables()
{
    // -- State 0: start --
    actionTable[0][LBRACE] = { SHIFT, 1 };
    gotoTable[0][S] = 2;

    // -- State 1: after top-level '{' --
    actionTable[1][STRING] = { SHIFT, 3 };  // key
    actionTable[1][RBRACE] = { SHIFT, 4 };  // {}
    gotoTable[1][PAIRS] = 5;                // when PAIRS recognized, expect '}'
    gotoTable[1][PAIR] = 6;                 // after first PAIR (no comma yet)

    // -- State 2: S recognized; accept on END --
    actionTable[2][END_SYM] = { ACCEPT, 0 };

    // -- State 3: after key STRING; need ':' --
    actionTable[3][COLON] = { SHIFT, 7 };

    // -- State 4: saw '{}' at top level -> reduce S -> {} --
    actionTable[4][END_SYM] = { REDUCE, 1 };

    // -- State 5: inside top-level object after PAIRS; expect '}' --
    actionTable[5][RBRACE] = { SHIFT, 8 };

    // -- State 6: after a PAIR (no comma consumed yet) --
    actionTable[6][COMMA] = { SHIFT, 9 };       // more pairs
    actionTable[6][RBRACE] = { REDUCE, 2 };     // PAIRS -> PAIR

    // -- State 7: after ":"; any VALUE starter --
    actionTable[7][STRING] = { SHIFT, 10 };
    actionTable[7][NUMBER] = { SHIFT, 11 };
    actionTable[7][TRUE_SYM] = { SHIFT, 12 };
    actionTable[7][FALSE_SYM] = { SHIFT, 13 };
    actionTable[7][NULL_SYM] = { SHIFT, 14 };
    actionTable[7][LBRACE] = { SHIFT, 18 };     // object-as-value
    actionTable[7][LBRACKET] = { SHIFT, 20 };   // array-as-value
    gotoTable[7][VALUE] = 15;

    // -- State 8: top-level '}' consumed -> accept on END --
    actionTable[8][END_SYM] = { ACCEPT, 0 };
    
    // -- State 9: after comma between pairs; parse next PAIR --
    actionTable[9][STRING] = { SHIFT, 3 };      // next key
    gotoTable[9][PAIR] = 16;                    // PAIR after comma
    gotoTable[9][PAIRS] = 17;                   // PAIRS tail

    // -- States 10..14: VALUE tokens; reduce on ',', '}', or ']' --
    actionTable[10][RBRACE] = { REDUCE, 5 };    // VALUE->STRING
    actionTable[10][COMMA]  = { REDUCE, 5 };
    actionTable[10][RBRACKET] = { REDUCE, 5 };

    actionTable[11][RBRACE] = { REDUCE, 6 };    // VALUE->NUMBER
    actionTable[11][COMMA]  = { REDUCE, 6 };
    actionTable[11][RBRACKET] = { REDUCE, 6 };

    actionTable[12][RBRACE] = { REDUCE, 7 };    // VALUE->TRUE
    actionTable[12][COMMA]  = { REDUCE, 7 };
    actionTable[12][RBRACKET] = { REDUCE, 7 };

    actionTable[13][RBRACE] = { REDUCE, 8 };    // VALUE->FALSE
    actionTable[13][COMMA]  = { REDUCE, 8 };
    actionTable[13][RBRACKET] = { REDUCE, 8 };

    actionTable[14][RBRACE] = { REDUCE, 9 };    // VALUE->NULL
    actionTable[14][COMMA]  = { REDUCE, 9 };
    actionTable[14][RBRACKET] = { REDUCE, 9 };

    // -- State 15: after VALUE; PAIR is complete --
    actionTable[15][COMMA] = { REDUCE, 4 };     // PAIR -> STRING : VALUE
    actionTable[15][RBRACE] = { REDUCE, 4 };

    // -- State 16: PAIR after comma; either another comma or end of PAIRS --
    actionTable[16][COMMA] = { SHIFT, 9 };
    actionTable[16][RBRACE] = { REDUCE, 2 };    // PAIRS -> PAIR

    // -- State 17: PAIRS tail recognized --
    actionTable[17][RBRACE] = { REDUCE, 3 };    // PAIRS -> PAIR, PAIRS

    // -- State 18: object-as-value; first key expected --
    actionTable[18][RBRACE] = { SHIFT, 19 };    // allow {} as a value
    actionTable[18][STRING] = { SHIFT, 3 };     // key (reuse key state) 
    gotoTable[18][PAIR] = 6;                    // save as first PAIR path at top-level
    gotoTable[18][PAIRS] = 22;                  // when PAIRS inside value are recognized

    // -- State 19: after '}' in value context: reduce VALUE -> {}
    actionTable[19][COMMA] = { REDUCE, 10 };
    actionTable[19][RBRACE] = { REDUCE, 10 };
    actionTable[19][RBRACKET] = { REDUCE, 10 };

    // -- State 20: array-as-value; either empty '[]' or one VALUE --
    actionTable[20][RBRACKET] = { SHIFT, 21 };  // [] empty
    actionTable[20][STRING]    = { SHIFT, 10 }; // one VALUE inside the array
    actionTable[20][NUMBER]    = { SHIFT, 11 };
    actionTable[20][TRUE_SYM]  = { SHIFT, 12 };
    actionTable[20][FALSE_SYM] = { SHIFT, 13 };
    actionTable[20][NULL_SYM]  = { SHIFT, 14 };
    actionTable[20][LBRACE]    = { SHIFT, 18 };
    actionTable[20][LBRACKET]  = { SHIFT, 20 };
    gotoTable[20][VALUE] = 24;

    // -- State 21: VALUE -> [] --
    actionTable[21][COMMA] = { REDUCE, 11 };
    actionTable[21][RBRACE] = { REDUCE, 11 };
    actionTable[21][RBRACKET] = { REDUCE, 11 };

    // -- State 22: object-as-value after PAIRS; expect closing '}' --
    actionTable[22][RBRACE] = { SHIFT, 23 };
    
    // -- State 23: close object-as-value -> reduce VALUE -> { PAIRS } --
    actionTable[23][COMMA] = { REDUCE, 12 };
    actionTable[23][RBRACE] = { REDUCE, 12 };
    actionTable[23][RBRACKET] = { REDUCE, 12 };

    // -- State 24: After reading one VALUE in the array, close with ']' and reduce --
    actionTable[24][RBRACKET] = { SHIFT, 25 };
    
    // -- State 25: VALUE -> [ VALUE ]
    actionTable[25][COMMA]    = { REDUCE, 13 };
    actionTable[25][RBRACE]   = { REDUCE, 13 };
    actionTable[25][RBRACKET] = { REDUCE, 13 };
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
            // There was a parsing error, let's give some hints on the issue
            // Let's start with the basic error information
            const Token &tok = tokens[pos];
            std::cerr <<
                "Syntax error at position " << tok.position <<
                " (line " << tok.line << ", col " << tok.column << "):" << std::endl <<
                "  Unexpected token " << symbolName(input) << std::endl <<
                "  Expected one of: " << expectedTokens(state) << std::endl << std::endl;

            // Now lets try to show where the parser thinks the error is located in the original JSON
            auto start = (tok.position > 20) ? tok.position - 20 : 0;
            auto end = std::min(_inputText.size(), tok.position + 20);
            std::string snippet = _inputText.substr(start, end - start);
            std::cerr << snippet << std::endl;
            std::cerr << std::string(tok.position - start, ' ') << "^" << std::endl;

            // Exit the parser
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
    size_t currentLine = 1;
    size_t currentCol = 0;
    std::string input((std::istreambuf_iterator<char>(_input)), std::istreambuf_iterator<char>());
    _inputText = input;

    auto consume_string = [&]() -> bool {
        // The caller has positioned i at the opening quote.
        ++i;
        ++currentCol;

        auto is_hex = [](char ch) {
            return (ch >= '0' && ch <= '9') ||
                   (ch >= 'a' && ch <= 'f') ||
                   (ch >= 'A' && ch <= 'F');
        };

        while (i < _inputText.size()) {
            const unsigned char ch =
                static_cast<unsigned char>(_inputText[i]);

            // Closing quotation mark.
            if (ch == '"') {
                ++i;
                ++currentCol;
                return true;
            }

            // JSON forbids unescaped control characters.
            if (ch < 0x20) {
                return false;
            }

            if (ch == '\\') {
                ++i;
                ++currentCol;

                if (i >= _inputText.size()) {
                    return false;
                }

                const char escaped = _inputText[i];

                switch (escaped) {
                    case '"':
                    case '\\':
                    case '/':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                        ++i;
                        ++currentCol;
                        break;

                    case 'u':
                        ++i;
                        ++currentCol;

                        // Unicode escape requires exactly four
                        // hexadecimal digits.
                        for (int digit = 0; digit < 4; ++digit) {
                            if (i >= _inputText.size() ||
                                !is_hex(_inputText[i])) {
                                return false;
                            }

                            ++i;
                            ++currentCol;
                        }
                        break;

                    default:
                        return false;
                }

                continue;
            }

            ++i;
            ++currentCol;
        }

        // Reached end of input without a closing quote.
        return false;
    };

    auto consume_number = [&]() -> std::string {
        const size_t start = i;

        // RFC 8259: number = [minus] int [frac] [exp]
        auto is_digit = [](char ch) {
            return ch >= '0' && ch <= '9';
        };

        auto advance = [&]() {
            ++i;
            ++currentCol;
        };

        // Optional leading minus.
        if (i < _inputText.size() && _inputText[i] == '-') {
            advance();
        }

        // Integer portion is mandatory.
        if (i >= _inputText.size()) {
            return {};
        }

        if (_inputText[i] == '0') {
            advance();

            // Leading zeros are forbidden.
            if (i < _inputText.size() && is_digit(_inputText[i])) {
                return {};
            }
        } else if (_inputText[i] >= '1' &&
                   _inputText[i] <= '9') {
            do {
                advance();
            } while (i < _inputText.size() &&
                     is_digit(_inputText[i]));
        } else {
            return {};
        }

        // Optional fractional portion.
        if (i < _inputText.size() && _inputText[i] == '.') {
            advance();

            // At least one digit must follow the decimal point.
            if (i >= _inputText.size() ||
                !is_digit(_inputText[i])) {
                return {};
            }

            while (i < _inputText.size() &&
                   is_digit(_inputText[i])) {
                advance();
            }
        }

        // Optional exponent portion.
        if (i < _inputText.size() &&
            (_inputText[i] == 'e' || _inputText[i] == 'E')) {
            advance();

            if (i < _inputText.size() &&
                (_inputText[i] == '+' || _inputText[i] == '-')) {
                advance();
            }

            // Exponent must contain at least one digit.
            if (i >= _inputText.size() ||
                !is_digit(_inputText[i])) {
                return {};
            }

            while (i < _inputText.size() &&
                   is_digit(_inputText[i])) {
                advance();
            }
        }

        return _inputText.substr(start, i - start);
    };

    while (i < _inputText.size()) {
        if (_inputText[i] == '\n') {
            ++currentLine;
            currentCol = 1;
        }
        if (isspace(_inputText[i])) {
            ++i;
            ++currentCol;
        } else if (_inputText[i] == '{') {
            tokens.push_back({ TokenType::LBrace, "{", i, currentLine, currentCol});
            ++i;
            ++currentCol;
        } else if (_inputText[i] == '}') {
            tokens.push_back({ TokenType::RBrace, "}", i, currentLine, currentCol});
            ++i;
            ++currentCol;
        } else if (_inputText[i] == '[') {
            tokens.push_back({ TokenType::LBracket, "[", i, currentLine, currentCol});
            ++i;
            ++currentCol;
        } else if (_inputText[i] == ']') {
            tokens.push_back({ TokenType::RBracket, "]", i, currentLine, currentCol});
            ++i;
            ++currentCol;
        } else if (_inputText[i] == ':') {
            tokens.push_back({ TokenType::Colon, ":", i, currentLine, currentCol});
            ++i;
            ++currentCol;
        } else if (_inputText[i] == ',') {
            tokens.push_back({ TokenType::Comma, ",", i, currentLine, currentCol});
            ++i;
            ++currentCol;
        } else if (_inputText[i] == '"') {
            const size_t start = i;
            const size_t startLine = currentLine;
            const size_t startCol = currentCol;

            const bool valid = consume_string();

            tokens.push_back({
                valid ? TokenType::String : TokenType::Invalid,
                _inputText.substr(start, i - start),
                start,
                startLine,
                startCol
            });
        } else if (isdigit(_inputText[i]) || _inputText[i] == '-') {
            const size_t start = i;
            const size_t startCol = currentCol;

            std::string num = consume_number();

            if (num.empty()) {
                tokens.push_back({
                    TokenType::Invalid,
                    _inputText.substr(start, i - start),
                    start,
                    currentLine,
                    startCol
                });
            } else {
                tokens.push_back({
                    TokenType::Number,
                    num,
                    start,
                    currentLine,
                    startCol
                });
            }
        } else if (_inputText.compare(i, 4, "true") == 0) {
            tokens.push_back({ TokenType::True, "true", i, currentLine, currentCol });
            i += 4;
            currentCol += 4;
        } else if (_inputText.compare(i, 5, "false") == 0) {
            tokens.push_back({ TokenType::False, "false", i, currentLine, currentCol });
            i += 5;
            currentCol += 5;
        } else if (_inputText.compare(i, 4, "null") == 0) {
            tokens.push_back({ TokenType::Null, "null", i, currentLine, currentCol });
            i += 4;
            currentCol += 4;
        } else {
            tokens.push_back({ TokenType::Invalid, std::string(1, _inputText[i++]), i, currentLine, currentCol });
        }
    }

    tokens.push_back({ TokenType::End, "", i, currentLine, currentCol});
    return tokens;
}
std::string JsonParser::expectedTokens(int state) const
{
    std::ostringstream oss;
    bool first = true;
    for(auto &kv : actionTable.at(state)) {
        Symbol sym = static_cast<Symbol>(kv.first);
        if (!first)
            oss << ", ";
        oss << symbolName(sym);
        first = false;
    }
    return oss.str();
}

std::string JsonParser::symbolName(Symbol s) const
{
    switch (s)
    {
        case LBRACE:    return "'{'";
        case RBRACE:    return "'}'";
        case COLON:     return "':'";
        case COMMA:     return "','";
        case STRING:    return "string";
        case NUMBER:    return "number";
        case TRUE_SYM:  return "'true'";
        case FALSE_SYM: return "'false'";
        case NULL_SYM:  return "'null'";
        case LBRACKET:  return "'['";
        case RBRACKET:  return "']'";
        case END_SYM:   return "end of input";
        default:        return "?";
    }
}

bool JsonParser::isValid()
{
    return _isValid;
}

