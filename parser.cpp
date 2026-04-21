#include "parser.h"
#include <cstddef>
#include <exception>
#include <iostream>
#include <queue>
#include <string>
#include "lexer.h"
#include "stypes.h"
#include "tree.h"

#define SIMPLEOPERATION(ptype)                                          \
if (checklast({EXPR, ptype, EXPR}) && lesspriority(ptype, ptk.type)) {  \
	ParserToken tok = { EXPR, new BiOperation(OperationType::ptype,       \
		scope[scope.size() - 3].expr, scope[scope.size() - 1].expr) };      \
	deletelast(3);                                                        \
	scope.push_back(tok);                                                 \
	continue;                                                             \
}

int stringtoint(std::string str) {
	int ret = 0;
	for (char ch: str) {
		ret *= 10;
		ret += ch - '0';
	}
	return ret;
}

double stringtodouble(std::string str) {
	double ret = 0;
	long long point = 0;
	for (char ch: str) {
		if (ch == '.') {
			point = 1;
			continue;
		}
		if (point) point *= 10;
		ret *= 10;
		ret += ch - '0';
	}
	return ret / point;
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
		return {RAW, new RawVal(Type(stringtoint(token.value)))};
		break;
	case TokenType::DOUBLE:
		return {RAW, new RawVal(Type(stringtodouble(token.value)))};
		break;
	case TokenType::STRING:
		return {RAW, new RawVal(Type(token.value))};
		break;
	case TokenType::VARIABLE:
		return {VAR, new Var(token.value)};
		break;
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
		if (token.value == ",") return {COMMA, nullptr};
		break;
	case TokenType::KEYWORD:
		if (token.value == "EOI") return {EOI, nullptr };
		if (token.value == "while") return {WHILE, nullptr };
		if (token.value == "if") return {IF, nullptr};
		if (token.value == "else") return {ELSE, nullptr};
		if (token.value == "print") return {PRINT, nullptr};
		if (token.value == "fn") return {FUNCDEF, nullptr};
		if (token.value == "return") return {RETURN, nullptr};
		break;
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

			if (checklast({FUNCDEF, VAR, LPRNT, PARAM, RPRNT, PROG})) {
				ParserToken tok = {PROG, new FunctionDef(
						scope[scope.size() - 5].expr, scope[scope.size() - 3].expr, scope[scope.size() - 1].expr)};
				deletelast(6);
				scope.push_back(tok);
				continue;
			}

			if (checklast({IF, EXPR, PROG, ELSE, PROG})) {
				ParserToken tok = {PROG, new TriOperation(OperationType::IF,
						scope[scope.size() - 4].expr, new UnOperation(OperationType::BLOCK, scope[scope.size() - 3].expr),
					new UnOperation(OperationType::BLOCK, scope[scope.size() - 1].expr))};
				deletelast(5);
				scope.push_back(tok);
				continue;
			}

			if (checklast({FUNCDEF, VAR, LPRNT, PARAM, RPRNT })) {
				break;
			}

			if (checklast({VAR, LPRNT, PARAM, RPRNT})) {
				ParserToken tok = {EXPR, new FunctionCall(
						scope[scope.size() - 4].expr, scope[scope.size() - 2].expr)};
				deletelast(4);
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
				ParserToken tok = {PROG, new UnOperation(OperationType::BLOCK ,scope[scope.size() - 2].expr)};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({PARAM, COMMA, EXPR})) {
				ParserToken tok = {PARAM, new BiOperation(OperationType::COMMA, scope[scope.size() - 3].expr, new FunctionParam(scope[scope.size() - 1].expr))};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({VAR, LPRNT, EXPR}) && lesspriority(ASSIGN, ptk.type)) {
				ParserToken tok = {PARAM, new FunctionParam(scope[scope.size() - 1].expr)};
				deletelast(1);
				scope.push_back(tok);
				continue;
			}

			if (checklast({RETURN, EXPR, SEMI})) {
				ParserToken tok = {PROG, new UnOperation(OperationType::RETURN,
						scope[scope.size() - 2].expr)};
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

			if (checklast({VAR, ASSIGN, EXPR}) && lesspriority(ASSIGN, ptk.type)) {
				ParserToken tok = {EXPR, new BiOperation(OperationType::ASSIGN,
						scope[scope.size() - 3].expr, scope[scope.size() - 1].expr)};
				deletelast(3);
				scope.push_back(tok);
				continue;
			}

			if (checklast({IF, EXPR, PROG}) && ptk.type != ELSE) {
				ParserToken tok = {PROG, new BiOperation(OperationType::IF,
						scope[scope.size() - 2].expr, new UnOperation(OperationType::BLOCK, scope[scope.size() - 1].expr))};
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

			if (checklast({VAR}) && ptk.type == LPRNT) {
				break;
			}

			if (checklast({VAR}) && ptk.type == ASSIGN) {
				break;
			}

			if (checklast({VAR})) {
				ParserToken tok;
				tok = {EXPR, scope[scope.size() - 1].expr};
				deletelast(1);
				scope.push_back(tok);
				continue;
			}

			if (checklast({RAW})) {
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
