#pragma once

#include "tree.h"
#include "table.h"
#include "memnode.h"
#include <stack>
#include <string>
#include <iostream>

#define MAX_EXPECTED_VALUES 2

struct ExprFrame {
	Expr* expr;
	char state;

	ExprFrame* target;
	int slot;

	Type values[MAX_EXPECTED_VALUES];
	int completed = 0;

	ExprFrame(Expr* _expr = nullptr, char _state = 0, ExprFrame* _trg = nullptr, int _slot = 0): expr(_expr), state(_state), target(_trg), slot(_slot) {}
};

class FrameFabric {
	std::stack<ExprFrame*> scope;
	std::stack<ExprFrame*> ready;

	void add() {
		ExprFrame* frms = new ExprFrame[100];
		scope.push(frms);
		for (int i = 99; i >= 0; --i) ready.push(frms + i);
	}
public:
	FrameFabric() {
		add();
	}
	~FrameFabric() {
		while (!scope.empty()) {
			delete[] scope.top();
			scope.pop();
		}
	}
	ExprFrame* get(Expr* _expr = nullptr, char _state = 0, ExprFrame* _trg = nullptr, int _slot = 0) {
		if (ready.empty()) add();
		ExprFrame* ret = ready.top();
		ready.pop();

		ret->expr = _expr;
		ret->state = _state;
		ret->target = _trg;
		ret->slot = _slot;

		ret->completed = 0;
		
		return ret;
	}
	void free(ExprFrame* frm) {
		ready.push(frm);
	}
};

class IterRun {
	std::stack<ExprFrame* > estack;
	MemNodeT* vars;
	FrameFabric fbr;
	std::stack<ExprFrame* > ret;
	std::stack<MemNodeT*> retvars;
	int lastsemi = 0;

	ExprFrame* next_frame(ExprFrame* curr, char state);
public:
	IterRun(): vars(new MemNodeT()), fbr() {}
	void process(Var* expr, ExprFrame* frame);
	void process(RawVal* expr, ExprFrame* frame);
	void process(UnOperation* expr, ExprFrame* frame);
	void process(BiOperation* expr, ExprFrame* frame);
	void process(TriOperation* expr, ExprFrame* frame);
	void process(FunctionParam* expr, ExprFrame* frame);
	void process(FunctionDef* expr, ExprFrame* frame);
	void process(FunctionCall* expr, ExprFrame* frame);
	void run(Expr* expr);
};
