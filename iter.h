#pragma once

#include "tree.h"
#include "table.h"
#include <stack>
#include <string>

struct ExprState {
	Expr* expr;
	char state;
	ExprState(Expr* _expr, char _state = 0): expr(_expr), state(_state) {}
};

class IterRun {
	std::stack<ExprState> estack;
	TTable<std::string, int> vars;
	std::stack<std::stack<std::string>> varscope;
	std::stack<int> intstack;
	std::stack<std::string> strstack;
	TTable<FuncDesc, Expr*> functions;
	std::stack<ExprState*> ret;
public:
	void process(Var* expr, char state);
	void process(RawVal* expr, char state);
	void process(UnOperation* expr, char state);
	void process(BiOperation* expr, char state);
	void process(TriOperation* expr, char state);
	void process(FunctionParam* expr, char state);
	void process(FunctionDef* expr, char state);
	void process(FunctionCall* expr, char state);
	void run(Expr* expr);
};
