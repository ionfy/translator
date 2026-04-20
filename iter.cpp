#include "iter.h"
#include "stypes.h"
#include "table.h"
#include "tree.h"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>

#define ASIMPLEOPERATION(optype, op)               \
case OperationType::optype:                        \
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
		if (left.type == Types::INT && right.type == Types::INT) \
			valstack.push(Type(Types::INT,\
						std::get<int>(left.val) op std::get<int>(right.val)));       \
		if (left.type == Types::INT && right.type == Types::DOUBLE) \
			valstack.push(Type(Types::DOUBLE,\
						std::get<int>(left.val) op std::get<double>(right.val)));       \
		if (left.type == Types::DOUBLE && right.type == Types::INT) \
			valstack.push(Type(Types::DOUBLE,\
						std::get<double>(left.val) op std::get<int>(right.val)));       \
		if (left.type == Types::DOUBLE && right.type == Types::DOUBLE) \
			valstack.push(Type(Types::DOUBLE,\
						std::get<double>(left.val) op std::get<double>(right.val)));       \
	}                                              \
	break;

#define BSIMPLEOPERATION(optype, op)               \
case OperationType::optype:                        \
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
		if (left.type == Types::INT && right.type == Types::INT) \
			valstack.push(Type(Types::INT,\
						std::get<int>(left.val) op std::get<int>(right.val)));       \
		if (left.type == Types::INT && right.type == Types::DOUBLE) \
			valstack.push(Type(Types::INT,\
						std::get<int>(left.val) op std::get<double>(right.val)));       \
		if (left.type == Types::DOUBLE && right.type == Types::INT) \
			valstack.push(Type(Types::INT,\
						std::get<double>(left.val) op std::get<int>(right.val)));       \
		if (left.type == Types::DOUBLE && right.type == Types::DOUBLE) \
			valstack.push(Type(Types::INT,\
						std::get<double>(left.val) op std::get<double>(right.val)));       \
	}                                              \
	break;

void IterRun::process(Var* expr, char state) {
	if (state == 0) {
		if (vars.contain(expr->getVal()))
			valstack.push(vars.get(expr->getVal()));
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
				switch (next.type) {
				case Types::NONE:
					std::cout << "None" << std::endl;
					break;
				case Types::INT:
					std::cout << std::get<int>(next.val) << std::endl;
					break;
				case Types::DOUBLE:
					std::cout << std::get<double>(next.val) << std::endl;
					break;
				case Types::STRING:
					std::cout << std::get<std::string>(next.val) << std::endl;
					break;
				default: throw -1;
				}
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
				varscope.pop();
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
	Type left, right;
	std::string var;

	switch (expr->getOp()) {
		ASIMPLEOPERATION(ADD, +)
		ASIMPLEOPERATION(SUB, -)
		ASIMPLEOPERATION(MUL, *)
		ASIMPLEOPERATION(DIV, /)
		BSIMPLEOPERATION(EQUAL, ==)
		BSIMPLEOPERATION(LESS, <)
		BSIMPLEOPERATION(GREATER, >)
		BSIMPLEOPERATION(LEQUAL, <=)
		BSIMPLEOPERATION(GEQUAL, >=)

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
				if (!vars.contain(var)) varscope.top().push(var);
				vars.insert(var, right);
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
				int flag = 0;
				if (left.type == Types::INT) flag = (0 != std::get<int>(left.val));
				if (left.type == Types::DOUBLE) flag = (0 != std::get<double>(left.val));
				if (flag) {
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
				int flag = 0;
				if (left.type == Types::INT) flag = (0 != std::get<int>(left.val));
				if (left.type == Types::DOUBLE) flag = (0 != std::get<double>(left.val));
				if (flag) {
					estack.push(ExprState(expr->getRight()));
				}
			}
			break;

		case OperationType::SEMI:
			if (state == 0) {
				estack.push(ExprState(expr->getRight()));
				// estack.push(ExprState(expr, 1));
				estack.push(ExprState(expr->getLeft()));
				// estack.push(ExprState(expr, 1));
			}
			else {
				while (!valstack.empty()) valstack.pop();
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
				int flag = 0;
				if (left.type == Types::INT) flag = (0 != std::get<int>(left.val));
				if (left.type == Types::DOUBLE) flag = (0 != std::get<double>(left.val));
				if (flag) {
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
		int ccount = std::get<int>(valstack.top().val);
		valstack.pop();
		valstack.push(Type(Types::INT, ccount + 1));
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
		vars.insert(name, val);
		varscope.top().push(name);
	}
}

void IterRun::process(FunctionDef* expr, char state) {
	if (state == 0) {
		expr->getScreen() = vars;
		estack.push(ExprState(expr, 1));
		estack.push(ExprState(expr->getName(), 1));
		valstack.push(Type(Types::INT, 0));
		estack.push(ExprState(expr->getParam()));
	}
	else if (state == 1) {
		std::string name = strstack.top();
		strstack.pop();
		char count = std::get<int>(valstack.top().val);
		valstack.pop();
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
		valstack.push(Type(Types::INT, 0));
		estack.push(ExprState(expr->getParam()));
	}
	else if (state == 1) {
		std::string name = strstack.top();
		strstack.pop();
		char count = std::get<int>(valstack.top().val);
		valstack.pop();
		if (!functions.contain({name, count})) {
			std::cout << "Used undeclared function " << name << " with " << (int)count << " arguments" << std::endl;
			throw -1;
		}
		expr->getDesc() = {name, count};
		estack.push(ExprState(expr, 2));
		estack.push(ExprState(expr->getParam(), 1));
	}
	else if (state == 2) {
		tempvars.push(TTable<std::string, Type>());
		vars.swap(tempvars.top());
		estack.push(ExprState(expr, 3));
		estack.push(ExprState(functions.get(expr->getDesc()), 2));
	}
	else if (state == 3) {
		vars.refresh(tempvars.top());
		estack.push(ExprState(expr, 4));
		estack.push(ExprState(functions.get(expr->getDesc()), 3));
	}
	else {
		vars.swap(tempvars.top());
		vars.refresh(tempvars.top());
		tempvars.pop();
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
