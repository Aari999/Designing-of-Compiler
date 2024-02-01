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
                current_pos++;
                return { token_type::PLUS, "+" };
            case '-':
                current_pos++;
                return { token_type::MINUS, "-" };
            case '*':
                current_pos++;
                return { token_type::MULTIPLY, "*" };
            case '/':
                current_pos++;
                return { token_type::DIVIDE, "/" };
            case '%':
                current_pos++;
                return { token_type::MODULO, "%" };
            case '(':
                current_pos++;
                return { token_type::LPAREN, "(" };
            case ')':
                current_pos++;
                return { token_type::RPAREN, ")" };
            case '=':
                current_pos++;
                return { token_type::DECLARE, "=" };
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
         if (token.type == token_type::PRINT) {
            token = lexer.get_next_token();  // Consume "print"
            int result = parse_expression(tok);
            cout << result << endl;
            return result;
        } else if (token.type == token_type::VAR) {
            tok = lexer.get_next_token();  // Consume a variable name
            if (token.type == token_type::IDENTIFIER) {
                string variable_name = token.value;
                token = lexer.get_next_token();  // Consume "="

                if (token.type == token_type::DECLARE) {
                    token = lexer.get_next_token();  // Consume a value

                    // check if the variable is already declared
                    if (variables.find(variable_name) != variables.end()) {
                        cerr << "Error: Variable '" << variable_name << "' already declared" << endl;
                        return 0;
                    }

                    int value = parse_expression(tok);
                    variables[variable_name] = value;
                } else {
                    cerr << "Error: Invalid variable declaration" << endl;
                    return 0;
                }
            } else {
                cerr << "Error: Invalid variable declaration" << endl;
                return 0;
            }
        } else if (token.type == token_type::IDENTIFIER) {
            // Variable assignment
            string variable_name = tok.value;
            token = lexer.get_next_token();  // Consume '='

            if (token.type == token_type::DECLARE) {
                tok = lexer.get_next_token();

                // check if the variable is already declared
                if (variables.find(variable_name) == variables.end()) {
                    cerr << "Error: Variable '" << variable_name << "' not declared" << endl;
                    return 0;
                }

                int value = parse_expression(tok);
                variables[variable_name] = value;

            } else if (token.type == token_type::PLUS || token.type == token_type::MINUS || token.type == token_type::MULTIPLY || token.type == token_type::DIVIDE || token.type == token_type::MODULO) {
                token = lexer.get_next_token();
                int value = parse_expression(token);
                return value + variables[variable_name];

            }
            else {
                cerr << "Error: Invalid variable assignment" << endl;
                return 0;
            }

        } else {
            // Regular expression evaluation
            return parse_expression(token);
        }

        return 0;
    }

    int parseExpression(Token& token) {
      int result = parse_term(token);

        while (token.type == token_type::PLUS || token.type == token_type::MINUS) {
            if (token.type == token_type::PLUS) {
                token = lexer.get_next_token();  // Consume '+'
                result += parse_term(token);
            } else if (token.type == token_type::MINUS) {
                token = lexer.get_next_token();  // Consume '-'
                result -= parse_term(token);
            }
        }
        return result;
    }

    int parseTerm(Token& token) {
        int result = parse_factor(token);

        while (token.type == token_type::MULTIPLY || token.type == token_type::DIVIDE || token.type == token_type::MODULO) {
            if (token.type == token_type::MULTIPLY) {
                token = lexer.get_next_token();  // Consume '*'
                result *= parse_factor(token);
            } else if (token.type == token_type::DIVIDE) {
                token = lexer.get_next_token();  // Consume '/'
                int divisor = parse_factor(token);
                if (divisor != 0) {
                    result /= divisor;
                } else {
                    cerr << "Error: Division by zero" << endl;
                    return 0;
                }
            } else if (token.type == token_type::MODULO) {
                token = lexer.get_next_token();  // Consume '%'
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
        if (token.type == token_type::INTEGER) {
            int value = stoi(token.value);
            token = lexer.get_next_token();  // Consume variable
            return value;
        } else if (token.type == token_type::IDENTIFIER) {
            string variable_name = token.value;
            token = lexer.get_next_token();  // Consume variable

            if (variables.find(variable_name) != variables.end()) {
                return variables[variable_name];
            } else {
                cerr << "Error: Variable '" << variable_name << "' not found" << endl;
                return 0;
            }

        } else if (token.type == token_type::LPAREN) {
            token = lexer.get_next_token();  // Consume '('
            int result = parse_expression(tok);
            if (token.type == token_type::RPAREN) {
                token = lexer.get_next_token();  // Consume ')'
                return result;
            } else {
                cerr << "Error: The parenthesis don't match" << endl;
                return 0;
            }
        }

        // Invalid factor
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

