#include "iter.h"
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
		right = intstack.top();                      \
		intstack.pop();                              \
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

void IterRun::process(Number* expr, char state) {
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

void IterRun::run(Expr* expr) {
	varscope.push(std::stack<std::string>());
	estack.push(ExprState(expr, 0));
	while (!estack.empty()) {
		ExprState curr = estack.top();
		estack.pop();
		curr.expr->process(this, curr.state);
	}
}
