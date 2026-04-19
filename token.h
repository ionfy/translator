#pragma once

#include <string>

enum class TokenType: int {VARIABLE, INTEGER, OPERATION, KEYWORD, DOUBLE};

struct Token {
	TokenType type;
	std::string value;
};
