#include <iostream>
#include <fstream>
#include <unordered_map>
#include "lexical_analyzer.h"
#include "parser.h"

std::unordered_map<std::string, int> variableTable;

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

    std::string line;
    while (std::getline(file, line)) {
        SimpleLexer lexer(line);
        SimpleParser parser(lexer);
        int result = parser.parse();
    }

    return 0;
}
