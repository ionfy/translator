#pragma once

#include <string>

enum class TokenType: int {VARIABLE, INTEGER, OPERATION, KEYWORD};

struct Token {
	TokenType type;
	std::string value;
};
