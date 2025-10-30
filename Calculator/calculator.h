#pragma once
#include "pluginmanager.h"
#include "ast.h"
#include "parser.h"
#include <string>

class Calculator {
public:
    Calculator(PluginManager* pm);
    double calculate(const std::string& expression);

private:
    PluginManager* m_pluginManager;

    std::vector<std::string> tokenize(const std::string& expression);
};