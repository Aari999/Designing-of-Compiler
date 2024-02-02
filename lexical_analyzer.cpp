#include "lexical_analyzer.h"
#include <cctype>

SimpleLexer::SimpleLexer(const std::string& sourceCode) : sourceCode(sourceCode), currentPos(0) {}

Token SimpleLexer::getNextToken() {
    while (currentPos < sourceCode.length() && isspace(sourceCode[currentPos])) {
        currentPos++;
    }

    if (currentPos >= sourceCode.length()) {
        return {TokenType::EndOfFile, ""};
    }

    if (isdigit(sourceCode[currentPos])) {
        return readIntegerToken();
    }

    switch (sourceCode[currentPos]) {
        case '+':
            currentPos++;
            return {TokenType::Plus, "+"};
        case '-':
            currentPos++;
            return {TokenType::Minus, "-"};
        case '*':
            currentPos++;
            return {TokenType::Multiply, "*"};
        case '/':
            currentPos++;
            return {TokenType::Divide, "/"};
        case '%':
            currentPos++;
            return {TokenType::Modulo, "%"};
        case '(':
            currentPos++;
            return {TokenType::LeftParenthesis, "("};
        case ')':
            currentPos++;
            return {TokenType::RightParenthesis, ")"};
        case '=':
            currentPos++;
            return {TokenType::Declare, "="};
    }

    if (isalpha(sourceCode[currentPos])) {
        return readIdentifierToken();
    }

    return {TokenType::EndOfFile, ""};
}

Token SimpleLexer::readIntegerToken() {
    std::string value;
    while (currentPos < sourceCode.length() && isdigit(sourceCode[currentPos])) {
        value += sourceCode[currentPos];
        currentPos++;
    }
    return {TokenType::Integer, value};
}

Token SimpleLexer::readIdentifierToken() {
    std::string value;
    while (currentPos < sourceCode.length() && (isalnum(sourceCode[currentPos]) || sourceCode[currentPos] == '_')) {
        value += sourceCode[currentPos];
        currentPos++;
    }

    if (value == "var") {
        return {TokenType::Var, value};
    } else if (value == "print") {
        return {TokenType::Print, value};
    }

    return {TokenType::Identifier, value};
}
