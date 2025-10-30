#include "parser.h"
#include <stdexcept>
#include <cctype>

std::unique_ptr<ASTNode> Parser::parse() {
    auto node = parseExpression();
    if (m_position < m_tokens.size()) {
        throw std::runtime_error("Unexpected token: " + currentToken());
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (m_position < m_tokens.size() && (currentToken() == "+" || currentToken() == "-")) {
        std::string op = currentToken();
        consumeToken();
        auto right = parseTerm();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (m_position < m_tokens.size() && (currentToken() == "*" || currentToken() == "/")) {
        std::string op = currentToken();
        consumeToken();
        auto right = parseFactor();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if (isFunctionCall()) {
        return parseFunctionCall();
    }

    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parseFunctionCall() {
    std::string funcName = currentToken();
    consumeToken(); // consume function name

    if (!match("(")) {
        throw std::runtime_error("Expected '(' after function name");
    }

    std::vector<std::unique_ptr<ASTNode>> args;
    if (currentToken() != ")") {
        do {
            args.push_back(parseExpression());
        } while (match(","));
    }

    if (!match(")")) {
        throw std::runtime_error("Expected ')' after function arguments");
    }

    return std::make_unique<FunctionCallNode>(funcName, std::move(args));
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match("(")) {
        auto node = parseExpression();
        if (!match(")")) {
            throw std::runtime_error("Expected ')'");
        }
        return node;
    }

    // Number
    if (std::isdigit(currentToken()[0]) || (currentToken()[0] == '-' && currentToken().length() > 1)) {
        double value = std::stod(currentToken());
        consumeToken();
        return std::make_unique<NumberNode>(value);
    }

    throw std::runtime_error("Unexpected token: " + currentToken());
}

const std::string& Parser::currentToken() const {
    if (m_position >= m_tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return m_tokens[m_position];
}

void Parser::consumeToken() {
    m_position++;
}

bool Parser::match(const std::string& expected) {
    if (m_position < m_tokens.size() && m_tokens[m_position] == expected) {
        consumeToken();
        return true;
    }
    return false;
}

bool Parser::isFunctionCall() const {
    if (m_position >= m_tokens.size()) return false;

    const std::string& token = m_tokens[m_position];
    if (!std::isalpha(token[0])) return false;

    // Check if next token is "("
    return (m_position + 1 < m_tokens.size() && m_tokens[m_position + 1] == "(");
}