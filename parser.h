#pragma once

#include "tree.h"
#include "token.h"
#include <vector>



class Parser {
	enum ParserType: int {
		VAR, LVAR,
		INT,
		ASSIGN,
		SEMI,
		ADD, SUB, MUL, DIV,
		NOT, EQUAL, NEQUAL, LESS, GREATER, LEQUAL, GEQUAL,
		LPRNT, RPRNT,
		LBRC, RBRC,
		WHILE,
		IF, ELSE,
		PRINT,
		EXPR, PROG, EOI};

	struct ParserToken {
		ParserType type;
		Expr* expr;
	};

	std::vector<ParserToken> scope;

	int getpriority(ParserType type);
	int lesspriority(ParserType left, ParserType right);
	int checklast(std::vector<ParserType> types);
	void deletelast(int count);
	ParserToken gettoken(Token token);
public:
	Parser() {}
	Expr* getTreeFromString(std::string str);
};

