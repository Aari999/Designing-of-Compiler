#ifndef SIMPLELEXER_H
#define SIMPLELEXER_H

#include <string>
#include <unordered_map>

enum class TokenType {
    Integer,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    EndOfFile,
    LeftParenthesis,
    RightParenthesis,
    Var,
    Const,
    Declare,
    Identifier,
    Print
};

struct Token {
    TokenType type;
    std::string value;
};

class SimpleLexer {
public:
    SimpleLexer(const std::string& sourceCode);
    Token getNextToken();

private:
    Token readIntegerToken();
    Token readIdentifierToken();

    std::string sourceCode;
    size_t currentPos;
};

#endif // SIMPLELEXER_H
