#include "ast.h"
#include "pluginmanager.h"
#include <stdexcept>
#include <cmath>

// ¬нешн€€ ссылка на PluginManager (будет передаватьс€ через контекст)
extern class PluginManager* g_pluginManager;

double BinaryOpNode::evaluate() const {
    double leftVal = m_left->evaluate();
    double rightVal = m_right->evaluate();

    if (m_op == "+") return leftVal + rightVal;
    if (m_op == "-") return leftVal - rightVal;
    if (m_op == "*") return leftVal * rightVal;
    if (m_op == "/") {
        if (rightVal == 0) throw std::runtime_error("Division by zero");
        return leftVal / rightVal;
    }
    if (m_op == "^") return std::pow(leftVal, rightVal);

    throw std::runtime_error("Unknown operator: " + m_op);
}

double FunctionCallNode::evaluate() const {
    if (!g_pluginManager) {
        throw std::runtime_error("Plugin manager not initialized");
    }

    auto* plugin = g_pluginManager->getPlugin(m_funcName);
    if (!plugin) {
        throw std::runtime_error("Unknown function: " + m_funcName);
    }

    std::vector<double> args;
    for (const auto& arg : m_args) {
        args.push_back(arg->evaluate());
    }

    return plugin->execute(args);
}