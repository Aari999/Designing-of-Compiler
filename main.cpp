#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <cmath>

using namespace std;

// Enumeration for different token types
enum TokenType
{
    Integer,
    Identifier,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Assign,
    Semicolon,
    LeftParenthesis,
    RightParenthesis,
    Unknown
};
unordered_map<string, unordered_map<string, int>> vars;
int lineNum = 1;
int inf = std::numeric_limits<int>::max();
// Struct to represent a token with its type and value
struct Token
{
    TokenType type;
    string value;
};

// Lexer class for tokenizing the input source code
class Lexer
{
public:
    Lexer(const string &src)
    {
        source = src;
        currentPosition = 0;
    }

    // Function to get the next token from the source code
    Token getNextToken()
    {
        if (currentPosition >= source.length())
        {
            return {Unknown, ""};
        }

        // Skip whitespace characters
        while (isspace(source[currentPosition]))
        {
            currentPosition++;
        }

        // Check if the current character represents an identifier, integer, or operator
        if (isalpha(source[currentPosition]) || source[currentPosition] == '_')
        {
            return getIdentifierToken();
        }
        else if (isdigit(source[currentPosition]))
        {
            return getIntegerToken();
        }
        else
        {
            return getOperatorToken();
        }
    }

private:
    // Function to get an identifier token
    Token getIdentifierToken()
    {
        string val;
        while (isalnum(source[currentPosition]) || source[currentPosition] == '_')
        {
            val += source[currentPosition];
            currentPosition++;
        }
        return {Identifier, val};
    }

    // Function to get an integer token
    Token getIntegerToken()
    {
        string val;
        if (source[currentPosition] == '-')
        {
            val += '-';
            currentPosition++;
        }
        while (isdigit(source[currentPosition]))
        {
            val += source[currentPosition];
            currentPosition++;
        }
        return {Integer, val};
    }

    // Function to get an operator token
    Token getOperatorToken()
    {
        switch (source[currentPosition])
        {
        case '+':
            currentPosition++;
            return {Plus, "+"};
        case '-':
            currentPosition++;
            return {Minus, "-"};
        case '*':
            currentPosition++;
            return {Multiply, "*"};
        case '/':
            currentPosition++;
            return {Divide, "/"};
        case '%':
            currentPosition++;
            return {Modulo, "%"};
        case '=':
            currentPosition++;
            return {Assign, "="};
        case '(':
            currentPosition++;
            return {LeftParenthesis, "("};
        case ')':
            currentPosition++;
            return {RightParenthesis, ")"};
        case ';':
            currentPosition++;
            return {Semicolon, ";"};
        default:
            currentPosition++;
            return {Unknown, ""};
        }
    }
    string source;
    size_t currentPosition;
};

// Parser class for parsing the tokenized source code
class Parser
{
public:
    // Constructor for the Parser class
    Parser(string src) : lexer(src)
    {
        curToken = lexer.getNextToken();
        std::cout << "curToken " << curToken.value << endl;
    }

    // Function to start parsing the source code
    void parse()
    {

        while (curToken.type != Unknown)
        {
            parseStatement();
        }
    }

private:
    // Function to parse a statement
    void parseStatement()
    {
        bool errorPrinted = false; // Flag to track if an error has been printed

        if (curToken.type == Identifier)
        {
            string id = curToken.value;
            curToken = lexer.getNextToken();

            // Check if the statement is an assignment or a variable output
            if (curToken.type == Assign)
            {
                curToken = lexer.getNextToken();
                int res = parseExpression();
                if (vars.count(id) == 0)
                {
                    // vars[id] = unordered_map<string, int>();
                    vars[id] = unordered_map<string, int>({pair<string, int>("line", lineNum)});
                    cout << "Creating variable in the SB " << id << " on line " << lineNum << endl;
                    vars[id]["line"] = lineNum;
                }
                vars[id]["value"] = res;

                cout << id << " = " << res << endl;
            }
            else if (curToken.type == Semicolon)
            {
                // Output variable value or error if not initialized
                if (vars.count(id) > 0)
                {
                    cout << id << " = " << vars[id]["value"] << endl;
                }
                else
                {
                    vars[id] = unordered_map<string, int>({pair<string, int>("line", lineNum),
                                                           pair("value", inf)});
                    cout << "Creating variable in the SB " << id << " on line" << lineNum << endl;
                    cerr << "Warning: Variable '" << id << "' is not initialized." << endl;
                    errorPrinted = true; // Set the flag to true after printing the error
                }
            }
            else
            {
                cerr << "Error: Invalid statement." << endl;
                errorPrinted = true; // Set the flag to true after printing the error
            }
        }
        else
        {
            cerr << "Error: Invalid statement." << endl;
            errorPrinted = true; // Set the flag to true after printing the error
        }

        // Check for the semicolon at the end of the statement
        if (curToken.type == Semicolon)
        {
            curToken = lexer.getNextToken();
        }
        else if (!errorPrinted)
        { // Check the flag before printing the semicolon error
            cerr << "Error: Expected ';'." << endl;
        }
    }

    // Function to parse a primary expression (integer, identifier, or subexpression)
    int parsePrimary()
    {
        if (curToken.type == Integer)
        {
            int val = stoi(curToken.value);
            curToken = lexer.getNextToken();
            return val;
        }
        else if (curToken.type == Identifier)
        {
            string id = curToken.value;
            curToken = lexer.getNextToken();

            // Return variable value or error if not defined
            cout << "searching for " << id << endl;

            if (vars.count(id) > 0)
            {
                if (vars[id]["value"] == inf)
                {
                    std::string message = "Use of uninitialized variable on line " + std::to_string(lineNum);

                    cerr << "Error: Variable '" << id << "' is not initialized." << endl;
                    throw std::runtime_error(message);

                    return 0;
                }
                return vars[id]["value"];
            }
            else
            {
                cerr << "Error: Variable '" << id << "' is not defined." << endl;
                std::string message = "Use of undefined variable on line " + std::to_string(lineNum);
                throw std::runtime_error(message);

                return 0;
            }
        }
        else if (curToken.type == LeftParenthesis)
        {
            curToken = lexer.getNextToken(); // get '('
            int result = parseExpression();
            if (curToken.type == RightParenthesis)
            {
                curToken = lexer.getNextToken(); // get ')'
                return result;
            }
            else
            {
                cerr << "Error: Expected ')'." << endl;
                return 0;
            }
        }
        else
        {
            cerr << "Error: Invalid factor." << endl;
            return 0;
        }
    }

    // Function to parse a term (multiplication, division, modulo)
    int parseTerm()
    {
        int res = parsePrimary();

        while (curToken.type == Multiply || curToken.type == Divide || curToken.type == Modulo)
        {
            TokenType op = curToken.type;
            curToken = lexer.getNextToken();

            int primary = parsePrimary();
            if (op == Multiply)
            {
                res *= primary;
            }
            else if (op == Divide)
            {
                res /= primary;
            }
            else
            {
                res %= primary;
            }
        }

        return res;
    }

    // Function to parse an expression (addition, subtraction)
    int parseExpression()
    {
        int res = parseTerm();
        while (curToken.type == Plus || curToken.type == Minus)
        {
            TokenType op = curToken.type;
            curToken = lexer.getNextToken();

            int term = parseTerm();
            if (op == Plus)
            {
                res += term;
            }
            else
            {
                res -= term;
            }
        }

        return res;
    }

    // Function to parse a factor (integer, identifier, or error)
    int parseFactor()
    {
        if (curToken.type == Integer)
        {
            int val = stoi(curToken.value);
            curToken = lexer.getNextToken();
            return val;
        }
        else if (curToken.type == Identifier)
        {
            string id = curToken.value;
            curToken = lexer.getNextToken();

            // Return variable value or error if not defined
            if (vars.count(id) > 0)
            {
                return vars[id]["value"];
            }
            else
            {
                cerr << "Error: Variable '" << id << "' is not defined." << endl;
                return 0;
            }
        }
        else
        {
            cerr << "Error: Invalid factor." << endl;
            return 0;
        }
    }

    Lexer lexer;
    Token curToken;
};

// Main function where parsing starts
int main()
{

    std::ifstream inputFile("input.txt");
    std::string line;
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::cout << "Starting:\n";

    while (std::getline(inputFile, line))
    {
        Parser parser(line);
        parser.parse();
        lineNum++;
        cout << endl;
    }
    inputFile.close();

    cout << "_________________" << endl;
    for (const auto &pair : vars)
    {
        unordered_map<string, int> varContent = pair.second;
        std::cout
            << "var  " << pair.first << " " << varContent["value"] << std::endl;
    }
    cout << "end" << endl;
    return 0;
}
