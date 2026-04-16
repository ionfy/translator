#pragma once

#include "tree.h"
#include "table.h"
#include <stack>

struct ExprState {
	Expr* expr;
	char state;
	ExprState(Expr* _expr, char _state = 0): expr(_expr), state(_state) {}
};

class IterRun {
	std::stack<ExprState> estack;
	TTable<std::string, int> vars;
	std::stack<int> intstack;
	std::stack<std::string> strstack;
public:
	void process(Var* expr, char state);
	void process(Number* expr, char state);
	void process(UnOperation* expr, char state);
	void process(BiOperation* expr, char state);
	void process(TriOperation* expr, char state);
	void run(Expr* expr);
};
