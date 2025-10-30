#include "calculator.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

// Глобальная ссылка на PluginManager для AST
PluginManager* g_pluginManager = nullptr;

Calculator::Calculator(PluginManager* pm) : m_pluginManager(pm) {
    g_pluginManager = pm; // Устанавливаем глобальную ссылку
}

double Calculator::calculate(const std::string& expression) {
    try {
        auto tokens = tokenize(expression);
        Parser parser(tokens);
        auto ast = parser.parse();
        return ast->evaluate();

    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Calculation error: ") + e.what());
    }
}

std::vector<std::string> Calculator::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::istringstream iss(expression);
    char c;
    std::string currentToken;

    while (iss.get(c)) {
        if (std::isspace(c)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            continue;
        }

        if (std::isdigit(c) || c == '.' || (c == '-' && currentToken.empty())) {
            currentToken += c;
        }
        else if (std::isalpha(c)) {
            currentToken += c;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        }
        else if (c == '(' || c == ')' || c == ',') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}