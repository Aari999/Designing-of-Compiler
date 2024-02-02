#include <iostream>
#include <fstream>
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
    SimpleLexer(const std::string& sourceCode) : sourceCode(sourceCode), currentPos(0) {}

    Token getNextToken() {
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
                return {  TokenType::Plus, "+" };
            case '-':
                currentPos++;
                return {  TokenType::Minus, "-" };
            case '*':
                currentPos++;
                return {  TokenType::Multiply, "*" };
            case '/':
                currentPos++;
                return {  TokenType::Divide, "/" };
            case '%':
                currentPos++;
                return {  TokenType::Modulo, "%" };
            case '(':
                currentPos++;
                return {  TokenType::LeftParenthesis, "(" };
            case ')':
                currentPos++;
                return {  TokenType::RightParenthesis, ")" };
            case '=':
                currentPos++;
                return {  TokenType::Declare, "=" };
        }

        if (isalpha(sourceCode[currentPos])) {
            return readIdentifierToken();
        }

        return {TokenType::EndOfFile, ""};
    }

private:
    Token readIntegerToken() {
        std::string value;
        while (currentPos < sourceCode.length() && isdigit(sourceCode[currentPos])) {
            value += sourceCode[currentPos];
            currentPos++;
        }
        return {TokenType::Integer, value};
    }

    Token readIdentifierToken() {
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

    std::string sourceCode;
    size_t currentPos;
};

std::unordered_map<std::string, int> variableTable;

class SimpleParser {
public:
    SimpleParser(SimpleLexer& lexer) : lexer(lexer) {}

    int parse() {
        Token token = lexer.getNextToken();
        if (token.type == TokenType::EndOfFile) {
            return 0;
        }
        int result = parseStatement(token);
        return result;
    }

private:
    int parseStatement(Token& token) {
         if (token.type ==  TokenType::Print) {
            token = lexer.getNextToken();  // get "print"
            int outcome = parseExpression(token);
            cout<<outcome <<endl;
            return outcome;
        } else if (token.type ==  TokenType::Var) {
            token = lexer.getNextToken();  // get z variable name
            if (token.type ==  TokenType::Identifier) {
                string name_of_variable = token.value;
                token = lexer.getNextToken();  // get "="

                if (token.type ==  TokenType::Declare) {
                    token = lexer.getNextToken();  // get a value

                    // check if the variable is already declared
                    if (variableTable.find(name_of_variable) != variableTable.end()) {
                        cerr << "Error: Variable '" << name_of_variable << "' already declared" << endl;
                        return 0;
                    }

                    int value = parseExpression(token);
                    variableTable[name_of_variable] = value;
                } else {
                    cerr << "Error: Invalid variable declaration" << endl;
                    return 0;
                }
            } else {
                cerr << "Error: Invalid variable declaration" << endl;
                return 0;
            }
        } else if (token.type ==  TokenType::Identifier) {
            // assigning variable
            string name_of_variable = tok.value;
            token = lexer.getNextToken();  // get '='

            if (token.type ==  TokenType::Declare) {
                token = lexer.getNextToken();

                // to check if z variable has been declared already
                if (variableTable.find(name_of_variable) == variableTable.end()) {
                    cerr << "Error: Variable '" << name_of_variable << "' not declared" << endl;
                    return 0;
                }

                int value = parseExpression(token);
                variableTable[name_of_variable] = value;

            } else if (token.type ==  TokenType::Plus || token.type ==  TokenType::Minus || token.type ==  TokenType::Multiply || token.type ==  TokenType::Divide || token.type ==  TokenType::Modulo) {
                token = lexer.getNextToken();
                int value = parseExpression(token);
                return value + variableTable[name_of_variable];

            }
            else {
                cerr << "Error: Invalid variable assignment" << endl;
                return 0;
            }

        } else {
            return parseExpression(token);
            // Regular expression 
        }

        return 0;
    }

    int parseExpression(Token& token) {
      int result = parse_term(token);

        while (token.type ==  TokenType::Plus || token.type ==  TokenType::Minus) {
            if (token.type ==  TokenType::Plus) {
                token = lexer.getNextToken();  // get '+'
                result += parse_term(token);
            } else if (token.type ==  TokenType::Minus) {
                token = lexer.getNextToken();  // zen get '-'
                result -= parse_term(token);
            }
        }
        return result;
    }

    int parseTerm(Token& token) {
        int result = parse_factor(token);

        while (token.type ==  TokenType::Multiply || token.type ==  TokenType::Divide || token.type ==  TokenType::Modulo) {
            if (token.type ==  TokenType::Multiply) {
                token = lexer.getNextToken();  // get '*' first
                result *= parse_factor(token);
            } else if (token.type ==  TokenType::Divide) {
                token = lexer.getNextToken();  // get '/'
                int divisor = parse_factor(token);
                if (divisor != 0) {
                    result /= divisor;
                } else {
                    cerr << "Error: Division by zero" << endl;
                    return 0;
                }
            } else if (token.type ==  TokenType::Modulo) {
                token = lexer.getNextToken();  // get '%'
                int divisor = parse_factor(token);
                if (divisor != 0) {
                    result %= divisor;
                } else {
                    cerr << "Error: Modulo by zero" << endl;
                    return 0;
                }
            }
        }

        return 0;
    }

    int parseFactor(Token& token) {
        if (token.type ==  TokenType::Integer) {
            int value = stoi(token.value);
            token = lexer.getNextToken();  // get variable
            return value;
        } else if (token.type ==  TokenType::Identifier) {
            string name_of_variable = token.value;
            token = lexer.getNextToken();  // get variable

            if (variableTable.find(name_of_variable) != variableTable.end()) {
                return variableTable[name_of_variable];
            } else {
                cerr << "Error: Variable '" << name_of_variable << "' not found" << endl;
                return 0;
            }

        } else if (token.type ==  TokenType::LeftParenthesis) {
            token = lexer.getNextToken();  // get '('
            int result = parseExpression(tok);
            if (token.type ==  TokenType::RightParenthesis) {
                token = lexer.getNextToken();  // get ')'
                return result;
            } else {
                cerr << "Error: The parenthesis in the given code doesn't match" << endl;
                return 0;
            }
        }
        cerr << "Error: Invalid factor" << token.value << endl;
        return 0;
    }

    SimpleLexer& lexer;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Couldn't open the file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    std::string sourceCode;
    std::string line;
    while (std::getline(file, line)) {
        SimpleLexer lexer(line);
        SimpleParser parser(lexer);
        int result = parser.parse();
    }

    return 0;
}

