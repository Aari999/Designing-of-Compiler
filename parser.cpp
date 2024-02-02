#include "parser.h"
#include <iostream>

std::unordered_map<std::string, int> variableTable;

SimpleParser::SimpleParser(SimpleLexer& lexer) : lexer(lexer) {}

int SimpleParser::parse() {
    Token token = lexer.getNextToken();
    if (token.type == TokenType::EndOfFile) {
        return 0;
    }
    int result = parseStatement(token);
    return result;
}

int SimpleParser::parseStatement(Token& token) {
    if (token.type == TokenType::Print) {
        token = lexer.getNextToken();
        int result = parseExpression(token);
        std::cout << result << std::endl;
        return result;
    } else if (token.type == TokenType::Var) {
        token = lexer.getNextToken();
        if (token.type == TokenType::Identifier) {
            std::string variableName = token.value;
            token = lexer.getNextToken();

            if (token.type == TokenType::Declare) {
                token = lexer.getNextToken();
                if (variableTable.find(variableName) != variableTable.end()) {
                    std::cerr << "Error: Variable '" << variableName << "' already declared" << std::endl;
                    return 0;
                }

                int value = parseExpression(token);
                variableTable[variableName] = value;
            } else {
                std::cerr << "Error: Invalid variable declaration" << std::endl;
                return 0;
            }
        } else {
            std::cerr << "Error: Invalid variable declaration" << std::endl;
            return 0;
        }
    } else if (token.type == TokenType::Identifier) {
        std::string variableName = token.value;
        token = lexer.getNextToken();

        if (token.type == TokenType::Declare) {
            token = lexer.getNextToken();
            if (variableTable.find(variableName) == variableTable.end()) {
                std::cerr << "Error: Variable '" << variableName << "' not declared" << std::endl;
                return 0;
            }

            int value = parseExpression(token);
            variableTable[variableName] = value;
        } else if (token.type == TokenType::Plus || token.type == TokenType::Minus || token.type == TokenType::Multiply || token.type == TokenType::Divide || token.type == TokenType::Modulo) {
            token = lexer.getNextToken();
            int value = parseExpression(token);
            return value + variableTable[variableName];
        } else {
            std::cerr << "Error: Invalid variable assignment" << std::endl;
            return 0;
        }
    } else {
        return parseExpression(token);
    }

    return 0;
}

int SimpleParser::parseExpression(Token& token) {
    int result = parseTerm(token);

    while (token.type == TokenType::Plus || token.type == TokenType::Minus) {
        if (token.type == TokenType::Plus) {
            token = lexer.getNextToken();
            result += parseTerm(token);
        } else if (token.type == TokenType::Minus) {
            token = lexer.getNextToken();
            result -= parseTerm(token);
        }
    }
    return result;
}

int SimpleParser::parseTerm(Token& token) {
    int result = parseFactor(token);

    while (token.type == TokenType::Multiply || token.type == TokenType::Divide || token.type == TokenType::Modulo) {
        if (token.type == TokenType::Multiply) {
            token = lexer.getNextToken();
            result *= parseFactor(token);
        } else if (token.type == TokenType::Divide) {
            token = lexer.getNextToken();
            int divisor = parseFactor(token);
            if (divisor != 0) {
                result /= divisor;
            } else {
                std::cerr << "Error: Division by zero" << std::endl;
                return 0;
            }
        } else if (token.type == TokenType::Modulo) {
            token = lexer.getNextToken();
            int divisor = parseFactor(token);
            if (divisor != 0) {
                result %= divisor;
            } else {
                std::cerr << "Error: Modulo by zero" << std::endl;
                return 0;
            }
        }
    }

    return result;
}

int SimpleParser::parseFactor(Token& token) {
    if (token.type == TokenType::Integer) {
        int value = std::stoi(token.value);
        token = lexer.getNextToken();
        return value;
    } else if (token.type == TokenType::Identifier) {
        std::string variableName = token.value;
        token = lexer.getNextToken();

        if (variableTable.find(variableName) != variableTable.end()) {
            return variableTable[variableName];
        } else {
            std::cerr << "Error: Variable '" << variableName << "' not found" << std::endl;
            return 0;
        }
    } else if (token.type == TokenType::LeftParenthesis) {
        token = lexer.getNextToken();
        int result = parseExpression(token);
        if (token.type == TokenType::RightParenthesis) {
            token = lexer.getNextToken();
            return result;
        } else {
            std::cerr << "Error: The parentheses don't match" << std::endl;
            return 0;
        }
    }

    std::cerr << "Error: Invalid factor" << token.value << std::endl;
    return 0;
}
