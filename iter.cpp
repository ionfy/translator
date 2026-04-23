#include "iter.h"
#include "stypes.h"
#include "table.h"
#include "tree.h"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>

#define SIMPLEOPERATION(optype, op)              \
case OperationType::optype:                      \
	if (state == 0) {                              \
		estack.push(ExprState(expr, 1));             \
		estack.push(ExprState(expr->getRight()));    \
		estack.push(ExprState(expr->getLeft()));     \
	}                                              \
	else {                                         \
		if (valstack.empty()) throw -1;              \
		right = valstack.top();                      \
		valstack.pop();                              \
		if (valstack.empty()) throw -1;              \
		left = valstack.top();                       \
		valstack.pop();                              \
		valstack.push(left op right);                \
	}                                              \
	break;

void IterRun::process(Var* expr, char state) {
	if (state == 0) {
		if (vars->contain(expr->getVal()))
			valstack.push(vars->get(expr->getVal()));
		else {
			std::cerr << "Used undeclared var: " << expr->getVal() << std::endl;
			throw -1;
		}
	}
	else strstack.push(expr->getVal());
}

void IterRun::process(RawVal* expr, char state) {
	valstack.push(expr->getVal());
}

void IterRun::process(UnOperation* expr, char state) {
	Type next;
	switch (expr->getOp()) {
		case OperationType::PRINT:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getNext()));
			}
			else {
				if (valstack.empty()) throw - 1;
				next = valstack.top();
				valstack.pop();
				std::cout << next.to_string() << std::endl;
			}
			break;

		case OperationType::BLOCK:
			if (state == 0) {
				vars = new MemNodeT(vars);
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getNext()));
			}
			else {
				MemNodeT* node = vars;
				vars = vars->get_parent();
				delete node;
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

		case OperationType::NOT:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getNext()));
			}
			else {
				if (valstack.empty()) throw - 1;
				next = valstack.top();
				valstack.pop();
				valstack.push(!next);
			}
			break;

		default: throw -1;
	}
}

void IterRun::process(BiOperation* expr, char state) {
	Type left, right;
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
				if (valstack.empty()) throw - 1;
				right = valstack.top();
				valstack.pop();

				vars->insert(var, right);
				valstack.push(right);
				// std::cout << var << " = " << right << '\n';
			}
			break;

		case OperationType::WHILE:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
			}
			else if (state == 1) {
				if (valstack.empty()) throw - 1;
				left = valstack.top();
				valstack.pop();
				if (left.to_bool()) {
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
				if (valstack.empty()) throw - 1;
				left = valstack.top();
				valstack.pop();
				if (left.to_bool()) {
					estack.push(ExprState(expr->getRight()));
				}
			}
			break;

		case OperationType::SEMI:
			if (state == 0) {
				lastsemi = valstack.size();
				estack.push(ExprState(expr->getRight()));
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
				estack.push(ExprState(expr, 1));
			}
			else {
				while (valstack.size() > lastsemi) valstack.pop();
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
	Type left, midle, right;

	switch (expr->getOp()) {
		case OperationType::IF:
			if (state == 0) {
				estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
			}
			else if (state == 1) {
				if (valstack.empty()) throw - 1;
				left = valstack.top();
				valstack.pop();
				if (left.to_bool()) {
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
		if (valstack.empty()) throw - 1;
		int ccount = valstack.top().to_int();
		valstack.pop();
		valstack.push(Type(ccount + 1));
	}
	else if (state == 1) { // знач
		estack.push(ExprState(expr->getExpr()));
	}
	else if (state == 2){ // имя
		estack.push(ExprState(expr, 3));
		estack.push(ExprState(expr->getExpr(), 1));
	}
	else { // = параметр
		if (valstack.empty()) throw - 1;

		Type val = valstack.top();
		valstack.pop();
		std::string name = strstack.top();
		strstack.pop();

		vars->insert_last(name, val);
	}
}

void IterRun::process(FunctionDef* expr, char state) {
	if (state == 0) {
		expr->getScreen() = vars;
		estack.push(ExprState(expr, 1));
		estack.push(ExprState(expr->getName(), 1));
		valstack.push(Type(0));
		estack.push(ExprState(expr->getParam()));
	}
	else if (state == 1) {
		std::string name = strstack.top();
		strstack.pop();
		char count = valstack.top().to_int();
		valstack.pop();
		vars->insert(name, Type(expr, count));
	}
	else if (state == 2) {
		retvars.push(vars);

		vars = new MemNodeT(expr->getScreen());

		estack.push(ExprState(expr, 3));
		ret.push(&(estack.top()));

		estack.push(ExprState(expr->getBody()));
		estack.push(ExprState(expr->getParam(), 2));
	}
	else {
		while (vars != expr->getScreen()) {
			MemNodeT* node = vars;
			vars = vars->get_parent();
			delete node;
		}
		vars = retvars.top();
		retvars.pop();

		ret.pop();
	}
}

void IterRun::process(FunctionCall* expr, char state) {
	if (state == 0) {
		estack.push(ExprState(expr, 1));
		estack.push(ExprState(expr->getName(), 1));
		valstack.push(Type(0));
		estack.push(ExprState(expr->getParam()));
	}
	else {
		std::string name = strstack.top();
		strstack.pop();
		char count = valstack.top().to_int();
		valstack.pop();
		if (!(vars->contain(name) && vars->get(name).get_type() == Types::FUNCTION && vars->get(name).get_args_count() == count)) {
			std::cout << "Used undeclared function " << name << " with " << (int)count << " arguments" << std::endl;
			throw -1;
		}

		estack.push(ExprState(vars->get(name).get_function(), 2));
		estack.push(ExprState(expr->getParam(), 1));
	}
}

void IterRun::run(Expr* expr) {
	estack.push(ExprState(expr, 0));
	while (!estack.empty()) {
		ExprState curr = estack.top();
		estack.pop();
		curr.expr->process(this, curr.state);
	}
}
