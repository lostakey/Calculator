#include "ast.h"
#include "pluginmanager.h"
#include <stdexcept>
#include <cmath>

double BinaryOpNode::evaluate(PluginManager* pluginManager) const {
    double leftVal = m_left->evaluate(pluginManager);
    double rightVal = m_right->evaluate(pluginManager);

    if (m_op == "+") return leftVal + rightVal;
    if (m_op == "-") return leftVal - rightVal;
    if (m_op == "*") return leftVal * rightVal;
    if (m_op == "/") {
        if (rightVal == 0) throw std::runtime_error("Division by zero");
        return leftVal / rightVal;
    }
    if (m_op == "^") {
        if (!pluginManager) {
            throw std::runtime_error("Plugin manager not initialized");
        }
        auto* powPlugin = pluginManager->getPlugin("pow");
        if (powPlugin) {
            return powPlugin->execute({ leftVal, rightVal });
        }
        else {
            return std::pow(leftVal, rightVal);
        }
    }
    if (m_op == "%") {
        if (rightVal == 0) throw std::runtime_error("Modulo by zero");
        return fmod(leftVal, rightVal);
    }

    throw std::runtime_error("Unknown operator: " + m_op);
}

double FunctionCallNode::evaluate(PluginManager* pluginManager) const {
    if (!pluginManager) {
        throw std::runtime_error("Plugin manager not initialized");
    }

    auto* plugin = pluginManager->getPlugin(m_funcName);
    if (!plugin) {
        throw std::runtime_error("Unknown function: " + m_funcName);
    }

    std::vector<double> args;
    for (const auto& arg : m_args) {
        args.push_back(arg->evaluate(pluginManager));
    }

    try {
        return plugin->execute(args);
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Function error: ") + e.what());
    }
}