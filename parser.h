#ifndef PARSER_H
#define PARSER_H

#include "lexical_analyzer.h"

class SimpleParser {
public:
    SimpleParser(SimpleLexer& lexer);
    int parse();

private:
    int parseStatement(Token& token);
    int parseExpression(Token& token);
    int parseTerm(Token& token);
    int parseFactor(Token& token);

    SimpleLexer& lexer;
};

#endif PARSER_H
