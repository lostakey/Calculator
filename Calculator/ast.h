#pragma once
#include <memory>
#include <vector>
#include <string>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual double evaluate() const = 0;
};

class NumberNode : public ASTNode {
    double m_value;
public:
    NumberNode(double value) : m_value(value) {}
    double evaluate() const override { return m_value; }
};

class BinaryOpNode : public ASTNode {
    std::string m_op;
    std::unique_ptr<ASTNode> m_left;
    std::unique_ptr<ASTNode> m_right;
public:
    BinaryOpNode(const std::string& op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : m_op(op), m_left(std::move(left)), m_right(std::move(right)) {}
    double evaluate() const override;
};

class FunctionCallNode : public ASTNode {
    std::string m_funcName;
    std::vector<std::unique_ptr<ASTNode>> m_args;
public:
    FunctionCallNode(const std::string& funcName, std::vector<std::unique_ptr<ASTNode>> args)
        : m_funcName(funcName), m_args(std::move(args)) {}
    double evaluate() const override;
};