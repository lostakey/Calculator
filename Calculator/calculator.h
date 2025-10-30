#pragma once
#include "pluginmanager.h"
#include <string>
#include <vector>
#include <stack>
#include <map>

class Calculator {
public:
    Calculator(PluginManager* pm);
    double calculate(const std::string& expression);

private:
    PluginManager* m_pluginManager;

    std::vector<std::string> tokenize(const std::string& expression);
    double evaluate(const std::vector<std::string>& tokens);
    bool isOperator(const std::string& token) const;
    int getPrecedence(const std::string& op) const;
};