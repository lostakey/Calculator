#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <memory>

class Parser {
public:
    Parser(const std::vector<std::string>& tokens) : m_tokens(tokens), m_position(0) {}

    std::unique_ptr<ASTNode> parse();

private:
    std::vector<std::string> m_tokens;
    size_t m_position;

    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseExponent();
    std::unique_ptr<ASTNode> parseFunctionCall();
    std::unique_ptr<ASTNode> parsePrimary();

    const std::string& currentToken() const;
    void consumeToken();
    bool match(const std::string& expected);
    bool isFunctionCall() const;
};