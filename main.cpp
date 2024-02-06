#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

enum class Tokentypes {
    Integer,
    Identifier,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Assignment,
    Semicolon,
    LeftParenthesis,
    RightParenthesis,
    Unknown
};

struct Token {
    Tokentypes type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& src) : source(src), current_position(0) {}

    Token getNextToken() {
        if (current_position >= source.length()) {
            return {Tokentypes::Unknown, ""};
        }

        while (isspace(source[current_position])) {
            current_position++;
        }

        if (isalpha(source[current_position]) || source[current_position] == '_') {
            return getIdentifierToken();
        } else if (isdigit(source[current_position])) {
            return getIntegerToken();
        } else {
            return getOperatorToken();
        }
    }

private:
    Token getIdentifierToken() {
        std::string val;
        while (isalnum(source[current_position]) || source[current_position] == '_') {
            val += source[current_position];
            current_position++;
        }
        return {Tokentypes::Identifier, val};
    }

    Token getIntegerToken() {
        std::string val;
        if (source[current_position] == '-') {
            val += '-';
            current_position++;
        }
        while (isdigit(source[current_position])) {
            val += source[current_position];
            current_position++;
        }
        return {Tokentypes::Integer, val};
    }

    Token getOperatorToken() {
        switch (source[current_position]) {
            case '+':
                current_position++;
                return {Tokentypes::Plus, "+"};
            case '-':
                current_position++;
                return {Tokentypes::Minus, "-"};
            case '*':
                current_position++;
                return {Tokentypes::Multiply, "*"};
            case '/':
                current_position++;
                return {Tokentypes::Divide, "/"};
            case '%':
                current_position++;
                return {Tokentypes::Modulo, "%"};
            case '=':
                current_position++;
                return {Tokentypes::Assignment, "="};
            case '(':
            	current_position++;
            	return {Tokentypes::LeftParenthesis, "("};
            case ')':
            	current_position++;
            	return {Tokentypes::RightParenthesis, ")"};
            case ';':
                current_position++;
                return {Tokentypes::Semicolon, ";"};
            default:
                current_position++;
                return {Tokentypes::Unknown, ""};
        }
    }
    std::string source;
    size_t current_position;
};

class Parser {
public:
    Parser(std::string src) : lexer(src) {
        curToken = lexer.getNextToken();
    }

    void parse() {
        while (curToken.type != Tokentypes::Unknown) {
            parseStatement();
        }
    }

private:
    void parseStatement() {
        if (curToken.type == Tokentypes::Identifier) {
            std::string id = curToken.value;
            curToken = lexer.getNextToken();

            if (curToken.type == Tokentypes::Assignment) {
                curToken = lexer.getNextToken();
                int res = parseExpression();
                vars[id] = res;
                std::cout << id << " = " << res << std::endl;
            } else if (curToken.type == Tokentypes::Semicolon) {
                if (vars.count(id) > 0) {
                    std::cout << id << " = " << vars[id] << std::endl;
                } else {
                    std::cerr << "Error: Variable '" << id << "' is not defined." << std::endl;
                }
            } else {
                std::cerr << "Error: Invalid statement." << std::endl;
            }
        } else {
            std::cerr << "Error: Invalid statement." << std::endl;
        }

        if (curToken.type == Tokentypes::Semicolon) {
            curToken = lexer.getNextToken();
        } else {
            std::cerr << "Error: Expected ';'." << std::endl;
        }
    }

    int parsePrimary() {
    if (curToken.type == Tokentypes::Integer) {
        int val = std::stoi(curToken.value);
        curToken = lexer.getNextToken();
        return val;
    } else if (curToken.type == Tokentypes::Identifier) {
        std::string id = curToken.value;
        curToken = lexer.getNextToken();

        if (vars.count(id) > 0) {
            return vars[id];
        } else {
            std::cerr << "Error: Variable '" << id << "' is not defined." << std::endl;
            return 0;
        }
    } else if (curToken.type == Tokentypes::LeftParenthesis) {
        curToken = lexer.getNextToken(); // get '('
        int result = parseExpression();
        if (curToken.type == Tokentypes::RightParenthesis) {
            curToken = lexer.getNextToken(); // get ')'
            return result;
        } else {
            std::cerr << "Error: Expected ')'." << std::endl;
            return 0;
        }
    } else {
        std::cerr << "Error: Invalid factor." << std::endl;
        return 0;
    }
}

    int parseTerm() {
    int res = parsePrimary();

    while (curToken.type == Tokentypes::Multiply || curToken.type == Tokentypes::Divide || curToken.type == Tokentypes::Modulo) {
        Tokentypes op = curToken.type;
        curToken = lexer.getNextToken();

        int primary = parsePrimary();
        if (op == Tokentypes::Multiply) {
            res *= primary;
        } else if (op == Tokentypes::Divide) {
            res /= primary;
        } else {
            res %= primary;
        }
    }

    return res;
  }

    int parseExpression() {

    int res = parseTerm();
    while (curToken.type == Tokentypes::Plus || curToken.type == Tokentypes::Minus) {
        Tokentypes op = curToken.type;
        curToken = lexer.getNextToken();

        int term = parseTerm();
        if (op == Tokentypes::Plus) {
            res += term;
        } else {
            res -= term;
        }
    }

    return res;
  }
    int parseFactor() {

        if (curToken.type == Tokentypes::Integer) {
            int val = std::stoi(curToken.value);
            curToken = lexer.getNextToken();
            return val;
        } else if (curToken.type == Tokentypes::Identifier) {
            std::string id = curToken.value;
            curToken = lexer.getNextToken();

            if (vars.count(id) > 0) {
                return vars[id];
            } else {
                std::cerr << "Error: The variable '" << id << "' is not defined." << std::endl;
                return 0;
            }
        } else {
            std::cerr << "Error: Invalid factor." << std::endl;
            return 0;
        }
    }

    Lexer lexer;
    Token curToken;
    std::unordered_map<std::string, int> vars;
};

int main() {
    std::string source;
    std::cout << "Press Enter and Space to quit" << std::endl;
    std::cout << "Enter Statement: " << std::endl;

    while (true) {
        std::getline(std::cin, source);
        if (source.empty() && std::cin.peek() == ' ') {
            break;
        }
        Parser parser(source);
        parser.parse();
    }

    return 0;
}
