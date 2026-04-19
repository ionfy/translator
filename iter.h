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

struct FuncDesc {
	std::string name;
	char pcount;

	int operator== (const FuncDesc& other) {
		return name == other.name && pcount == other.pcount;
	}

	int operator< (const FuncDesc& other) {
		if (name != other.name) return name < other.name;
		return pcount < other.pcount;
	}

	int operator> (const FuncDesc& other) {
		if (name != other.name) return name > other.name;
		return pcount > other.pcount;
	}
};

class IterRun {
	std::stack<ExprState> estack;
	TTable<std::string, int> vars;
	std::stack<std::stack<std::string>> varscope;
	std::stack<int> intstack;
	std::stack<std::string> strstack;
	TTable<FuncDesc, Expr*> functions;
public:
	void process(Var* expr, char state);
	void process(Number* expr, char state);
	void process(UnOperation* expr, char state);
	void process(BiOperation* expr, char state);
	void process(TriOperation* expr, char state);
	void process(FunctionParam* expr, char state);
	void process(FunctionDef* expr, char state);
	void run(Expr* expr);
};
