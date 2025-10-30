#include "calculator.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <cctype>

Calculator::Calculator(PluginManager* pm) : m_pluginManager(pm) {}

double Calculator::calculate(const std::string& expression) {
    try {
        auto tokens = tokenize(expression);
        std::cout << "Tokens: ";
        for (const auto& t : tokens) std::cout << "[" << t << "] ";
        std::cout << "\n";
        return evaluate(tokens);
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

        if (std::isdigit(c) || c == '.') {
            currentToken += c;
        }
        else if (std::isalpha(c)) {
            currentToken += c;
        }
        else if (isOperator(std::string(1, c))) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        }
        else if (c == '(' || c == ')') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        }
        else if (c == ',') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}

double Calculator::evaluate(const std::vector<std::string>& tokens) {
    std::stack<double> values;
    std::stack<std::string> operators;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];
        if (token.empty()) continue;

        if (std::isdigit(token[0]) || (token.length() > 1 && std::isdigit(token[1]))) {
            // Число
            values.push(std::stod(token));
        }
        else if (std::isalpha(token[0])) {
            // Функция
            if (i + 1 < tokens.size() && tokens[i + 1] == "(") {
                operators.push(token);
                operators.push("(");
                i++;
            }
            else {
                throw std::runtime_error("Function call without parentheses: " + token);
            }
        }
        else if (token == "(") {
            operators.push(token);
        }
        else if (token == ")") {
            // Обрабатываем закрывающую скобку
            while (!operators.empty() && operators.top() != "(") {
                if (operators.top() == "func_marker") {
                    operators.pop();
                    continue;
                }

                if (isOperator(operators.top())) {
                    // Бинарный оператор
                    if (values.size() < 2) {
                        throw std::runtime_error("Not enough operands for operator: " + operators.top());
                    }
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    std::string op = operators.top(); operators.pop();

                    if (op == "+") values.push(a + b);
                    else if (op == "-") values.push(a - b);
                    else if (op == "*") values.push(a * b);
                    else if (op == "/") {
                        if (b == 0) throw std::runtime_error("Division by zero");
                        values.push(a / b);
                    }
                }
            }

            if (!operators.empty() && operators.top() == "(") {
                operators.pop();

                // Проверяем, была ли перед скобкой функция
                if (!operators.empty() && std::isalpha(operators.top()[0])) {
                    std::string funcName = operators.top(); operators.pop();
                    IPlugin* plugin = m_pluginManager->getPlugin(funcName);
                    if (!plugin) {
                        throw std::runtime_error("Unknown function: " + funcName);
                    }

                    if (values.empty()) {
                        throw std::runtime_error("No argument for function: " + funcName);
                    }

                    double arg = values.top(); values.pop();
                    double result = plugin->execute({ arg });
                    values.push(result);
                }
            }
        }
        else if (isOperator(token)) {
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(token)) {
                if (values.size() < 2) {
                    throw std::runtime_error("Not enough operands for operator: " + operators.top());
                }
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                std::string op = operators.top(); operators.pop();

                if (op == "+") values.push(a + b);
                else if (op == "-") values.push(a - b);
                else if (op == "*") values.push(a * b);
                else if (op == "/") {
                    if (b == 0) throw std::runtime_error("Division by zero");
                    values.push(a / b);
                }
            }
            operators.push(token);
        }
    }

    // Обрабатываем оставшиеся операторы
    while (!operators.empty()) {
        std::string op = operators.top(); operators.pop();

        if (op == "(") {
            throw std::runtime_error("Mismatched parentheses");
        }

        if (isOperator(op)) {
            if (values.size() < 2) {
                throw std::runtime_error("Not enough operands for operator: " + op);
            }
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();

            if (op == "+") values.push(a + b);
            else if (op == "-") values.push(a - b);
            else if (op == "*") values.push(a * b);
            else if (op == "/") {
                if (b == 0) throw std::runtime_error("Division by zero");
                values.push(a / b);
            }
        }
    }

    if (values.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return values.top();
}

bool Calculator::isOperator(const std::string& token) const {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

int Calculator::getPrecedence(const std::string& op) const {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}