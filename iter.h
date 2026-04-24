#pragma once

#include "tree.h"
#include "table.h"
#include "memnode.h"
#include <stack>
#include <string>

#define MAX_EXPECTED_VALUES 3

struct ExprFrame {
	Expr* expr;
	char state;

	ExprFrame* target;
	int slot;

	Type values[MAX_EXPECTED_VALUES];
	int complited = 0;

	ExprFrame(Expr* _expr, char _state = 0, ExprFrame* _trg = nullptr, int _slot = 0): expr(_expr), state(_state), target(_trg), slot(_slot) {}
};

class IterRun {
	std::stack<ExprFrame> estack;
	MemNodeT* vars;
	std::stack<Type> valstack;
	std::stack<std::string> strstack;
	std::stack<ExprFrame*> ret;
	std::stack<MemNodeT*> retvars;
	int lastsemi = 0;
public:
	IterRun(): vars(new MemNodeT()) {}
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
