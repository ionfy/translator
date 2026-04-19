#include "iter.h"
#include "table.h"
#include "tree.h"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>

#define SIMPLEOPERATION(type, op)                    \
case OperationType::type:                            \
	if (state == 0) {                                \
		estack.push(ExprState(expr, 1));             \
		estack.push(ExprState(expr->getRight()));    \
		estack.push(ExprState(expr->getLeft()));     \
	}                                                \
	else {                                           \
		if (intstack.empty()) throw -1;              \
		right = intstack.top();                      \
		intstack.pop();                              \
		if (intstack.empty()) throw -1;              \
		left = intstack.top();                       \
		intstack.pop();                              \
		intstack.push(left op right);                \
	}                                                \
	break;

void IterRun::process(Var* expr, char state) {
	if (state == 0) {
		if (vars.contain(expr->getVal()))
			intstack.push(vars.get(expr->getVal()));
		else {
			std::cerr << "Used undeclared var: " << expr->getVal() << std::endl;
			throw -1;
		}
	}
	else strstack.push(expr->getVal());
}

void IterRun::process(RawVal* expr, char state) {
	intstack.push(expr->getVal());
}

void IterRun::process(UnOperation* expr, char state) {
	int next;
	switch (expr->getOp()) {
		case OperationType::PRINT:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getNext()));
			}
			else {
				if (intstack.empty()) throw - 1;
				next = intstack.top();
				intstack.pop();
				std::cout << next << std::endl;
			}
			break;

		case OperationType::BLOCK:
			if (state == 0) {
				varscope.push(std::stack<std::string>());
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getNext()));
			}
			else {
				while (!varscope.top().empty()) {
					vars.remove(varscope.top().top());
					varscope.top().pop();
				}
			}
			break;
		
		case OperationType::RETURN:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getNext()));
			}
			else {
				while (&(estack.top()) != ret.top()) {
					estack.pop();
				}
			}
			break;

		default: throw -1;
	}
}

void IterRun::process(BiOperation* expr, char state) {
	int left, right;
	std::string var;

	switch (expr->getOp()) {
		SIMPLEOPERATION(ADD, +)
		SIMPLEOPERATION(SUB, -)
		SIMPLEOPERATION(MUL, *)
		SIMPLEOPERATION(DIV, /)
		SIMPLEOPERATION(EQUAL, ==)
		SIMPLEOPERATION(LESS, <)
		SIMPLEOPERATION(GREATER, >)
		SIMPLEOPERATION(LEQUAL, <=)
		SIMPLEOPERATION(GEQUAL, >=)

		case OperationType::ASSIGN:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getRight()));
				estack.push(ExprState(expr->getLeft(), 1));
			}
			else {
				var = strstack.top();
				strstack.pop();
				if (intstack.empty()) throw - 1;
				right = intstack.top();
				intstack.pop();
				if (!vars.contain(var)) varscope.top().push(var);
				vars.insert(var, right);
				intstack.push(right);
				// std::cout << var << " = " << right << '\n';
			}
			break;

		case OperationType::WHILE:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
			}
			else if (state == 1) {
				if (intstack.empty()) throw - 1;
				left = intstack.top();
				intstack.pop();
				if (left) {
					estack.push(ExprState(expr, 0));
					estack.push(ExprState(expr->getRight()));
				}
			}
			break;

		case OperationType::IF:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
			}
			else if (state == 1) {
				if (intstack.empty()) throw - 1;
				left = intstack.top();
				intstack.pop();
				if (left) {
					estack.push(ExprState(expr->getRight()));
				}
			}
			break;

		case OperationType::SEMI:
			if (state == 0) {
				estack.push(ExprState(expr->getRight()));
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
				estack.push(ExprState(expr, 1));
			}
			else {
				while (!intstack.empty()) intstack.pop();
			}
			break;

		case OperationType::COMMA:
			if (state == 0) {
				estack.push(ExprState(expr->getRight()));
				estack.push(ExprState(expr->getLeft()));
			}
			else if (state == 1) {
				estack.push(ExprState(expr->getRight(), 1));
				estack.push(ExprState(expr->getLeft(), 1));
			}
			else {
				estack.push(ExprState(expr->getLeft(), 2));
				estack.push(ExprState(expr->getRight(), 2));
			}
			break;

		default: throw -1;
	}
}

void IterRun::process(TriOperation* expr, char state) {
	int left, midle, right;

	switch (expr->getOp()) {
		case OperationType::IF:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
			}
			else if (state == 1) {
				if (intstack.empty()) throw - 1;
				left = intstack.top();
				intstack.pop();
				if (left) {
					estack.push(ExprState(expr->getMidle()));
				}
				else {
					estack.push(ExprState(expr->getRight()));
				}
			}
			break;
		default: throw -1;
	}
}

void IterRun::process(FunctionParam* expr, char state) {
	if (state == 0) { // число
		if (intstack.empty()) throw - 1;
		int ccount = intstack.top();
		intstack.pop();
		intstack.push(ccount + 1);
	}
	else if (state == 1) { // знач
		estack.push(ExprState(expr->getExpr()));
	}
	else if (state == 2){ // имя
		estack.push(ExprState(expr, 3));
		estack.push(ExprState(expr->getExpr(), 1));
	}
	else { // = параметр
		if (intstack.empty()) throw - 1;
		int val = intstack.top();
		intstack.pop();
		std::string name = strstack.top();
		strstack.pop();
		vars.insert(name, val);
	}
}

void IterRun::process(FunctionDef* expr, char state) {
	if (state == 0) {
		expr->getScreen() = vars;
		estack.push(ExprState(expr, 1));
		estack.push(ExprState(expr->getName(), 1));
		intstack.push(0);
		estack.push(ExprState(expr->getParam()));
	}
	else if (state == 1) {
		std::string name = strstack.top();
		strstack.pop();
		char count = intstack.top();
		intstack.pop();
		functions.insert({name, count}, expr);
	}
	else if (state == 2) {
		vars = expr->getScreen();
	}
	else if (state == 3) {
		estack.push(ExprState(expr, 4));
		ret.push(&(estack.top()));
		estack.push(ExprState(expr->getBody()));
		estack.push(ExprState(expr->getParam(), 2));
	}
	else {
		expr->getScreen().refresh(vars);
		vars = expr->getScreen();
	}
}

void IterRun::process(FunctionCall* expr, char state) {
	if (state == 0) {
		estack.push(ExprState(expr, 1));
		estack.push(ExprState(expr->getName(), 1));
		intstack.push(0);
		estack.push(ExprState(expr->getParam()));
	}
	else if (state == 1) {
		std::string name = strstack.top();
		strstack.pop();
		char count = intstack.top();
		intstack.pop();
		if (!functions.contain({name, count})) {
			std::cout << "Used undeclared function " << name << " with " << (int)count << " arguments" << std::endl;
			throw -1;
		}
		expr->getDesc() = {name, count};
		estack.push(ExprState(expr, 2));
		estack.push(ExprState(expr->getParam(), 1));
	}
	else if (state == 2) {
		expr->getTemp().swap(vars);
		estack.push(ExprState(expr, 3));
		estack.push(ExprState(functions.get(expr->getDesc()), 2));
	}
	else if (state == 3) {
		vars.refresh(expr->getTemp());
		estack.push(ExprState(expr, 4));
		estack.push(ExprState(functions.get(expr->getDesc()), 3));
	}
	else {
		vars.swap(expr->getTemp());
		vars.refresh(expr->getTemp());
		ret.pop();
	}
}

void IterRun::run(Expr* expr) {
	varscope.push(std::stack<std::string>());
	estack.push(ExprState(expr, 0));
	while (!estack.empty()) {
		ExprState curr = estack.top();
		estack.pop();
		curr.expr->process(this, curr.state);
	}
}
