#include "parser.h"
#include <iostream>
#include <queue>
#include <string>
#include "lexer.h"
#include "tree.h"

#define SIMPLEOPERATION(ptype)                                          \
if (checklast({EXPR, ptype, EXPR}) && lesspriority(ptype, ptk.type)) {  \
	ParserToken tok = { EXPR, new BiOperation(OperationType::ptype,       \
		scope[scope.size() - 3].expr, scope[scope.size() - 1].expr) };      \
	deletelast(3);                                                        \
	scope.push_back(tok);                                                 \
	continue;                                                             \
}

int Parser::getpriority(ParserType type) {
	if (type == MUL || type == DIV) return 3;
	if (type == ADD || type == SUB) return 2;
	if (type == EQUAL || type == LESS ||
			type == GREATER || type == LEQUAL ||
			type == GEQUAL) return 1;
	if (type == ASSIGN) return 0;
	return -1;
}

int Parser::lesspriority(ParserType left, ParserType right) {
	if (getpriority(left) < getpriority(right)) return 0;
	return 1;
}

int Parser::checklast(std::vector<ParserType> types) {
	int sz = types.size();
	if (scope.size() < sz) return 0;
	int start = scope.size() - sz;
	for (int i = 0; i < sz; i++) {
		if (scope[start + i].type != types[i]) return 0;
	}
	return 1;
}

void Parser::deletelast(int count) {
	for (int i = 0; i < count; i++) scope.pop_back();
}

Parser::ParserToken Parser::gettoken(Token token) {
	switch (token.type) {
	case TokenType::INTEGER:
		return {INT, new Number(token.value)};
	case TokenType::VARIABLE:
		return {VAR, new Var(token.value)};
	case TokenType::OPERATION:
		if (token.value == "(") return {LPRNT, nullptr};
		if (token.value == ")") return {RPRNT, nullptr};
		if (token.value == "+") return {ADD, nullptr};
		if (token.value == "-") return {SUB, nullptr};
		if (token.value == "*") return {MUL, nullptr};
		if (token.value == "/") return {DIV, nullptr};
		if (token.value == "=") return {ASSIGN, nullptr};
		if (token.value == ";") return {SEMI, nullptr};
		if (token.value == "!") return {NOT, nullptr};
		if (token.value == "==") return {EQUAL, nullptr};
		if (token.value == "<") return {LESS, nullptr};
		if (token.value == ">") return {GREATER, nullptr};
		if (token.value == "<=") return {LEQUAL, nullptr};
		if (token.value == ">=") return {GEQUAL, nullptr};
		if (token.value == "{") return {LBRC, nullptr};
		if (token.value == "}") return {RBRC, nullptr};
	case TokenType::KEYWORD:
		if (token.value == "EOI") return {EOI, nullptr };
		if (token.value == "while") return {WHILE, nullptr };
		if (token.value == "if") return {IF, nullptr};
		if (token.value == "else") return {ELSE, nullptr};
		if (token.value == "print") return {PRINT, nullptr};
	}
	throw -1;
}

Expr* Parser::getTreeFromString(std::string str) {
	Avt avt;
	std::queue<Token> tokens = avt.fromString(str);
	tokens.push({ TokenType::KEYWORD, "EOI" });
	
	while (!tokens.empty()) {
		ParserToken ptk = gettoken(tokens.front());
		tokens.pop();
		while (1) {
			if (checklast({IF, EXPR, PROG, ELSE, PROG})) {
				ParserToken tok = {PROG, new TriOperation(OperationType::IF,
						scope[scope.size() - 4].expr, scope[scope.size() - 3].expr, scope[scope.size() - 1].expr)};
				deletelast(5);
				scope.push_back(tok);
				continue;
			}

			if (checklast({LPRNT, EXPR, RPRNT})) {
				ParserToken tok = {EXPR, scope[scope.size() - 2].expr};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({LBRC, PROG, RBRC})) {
				ParserToken tok = {PROG, scope[scope.size() - 2].expr};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			SIMPLEOPERATION(ADD)
			SIMPLEOPERATION(SUB)
			SIMPLEOPERATION(DIV)
			SIMPLEOPERATION(MUL)
			SIMPLEOPERATION(EQUAL)
			SIMPLEOPERATION(LESS)
			SIMPLEOPERATION(GREATER)
			SIMPLEOPERATION(LEQUAL)
			SIMPLEOPERATION(GEQUAL)

			if (checklast({PRINT, EXPR, SEMI})) {
				ParserToken tok = {PROG, new UnOperation(OperationType::PRINT,
						scope[scope.size() - 2].expr)};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({LVAR, ASSIGN, EXPR}) && lesspriority(ASSIGN, ptk.type)) {
				ParserToken tok = {EXPR, new BiOperation(OperationType::ASSIGN,
						scope[scope.size() - 3].expr, scope[scope.size() - 1].expr)};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({IF, EXPR, PROG}) && ptk.type != ELSE) {
				ParserToken tok = {PROG, new BiOperation(OperationType::IF,
						scope[scope.size() - 2].expr, scope[scope.size() - 1].expr)};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({WHILE, EXPR, PROG})) {
				ParserToken tok = {PROG, new BiOperation(OperationType::WHILE,
						scope[scope.size() - 2].expr, scope[scope.size() - 1].expr)};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({PROG, PROG})) {
				ParserToken tok = {PROG, new BiOperation(OperationType::SEMI,
						scope[scope.size() - 2].expr, scope[scope.size() - 1].expr)};
				deletelast(2);
				scope.push_back(tok);
				continue;
			}

			if (checklast({EXPR, SEMI})) {
				ParserToken tok = {PROG, scope[scope.size() - 2].expr};
				deletelast(2);
				scope.push_back(tok);
				continue;
			}

			if (checklast({VAR})) {
				ParserToken tok;
				if (ptk.type == ASSIGN)
					tok = {LVAR, scope[scope.size() - 1].expr};
				else
					tok = {EXPR, scope[scope.size() - 1].expr};
				deletelast(1);
				scope.push_back(tok);
				continue;
			}

			if (checklast({INT})) {
				ParserToken tok = {EXPR, scope[scope.size() - 1].expr};
				deletelast(1);
				scope.push_back(tok);
				continue;
			}

			break;
		}
		scope.push_back(ptk);
	}

	if (checklast({ PROG, EOI })) {
		ParserToken tok = { PROG, scope[scope.size() - 2].expr };
		deletelast(2);
		scope.push_back(tok);
	}
	
	if (scope.size() > 1) std::cout << "Smth wrong\n";
	return scope[0].expr;
}
