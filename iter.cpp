#include "iter.h"
#include "stypes.h"
#include "table.h"
#include "tree.h"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#define SIMPLEOPERATION(optype, op)              \
case OperationType::optype:                      \
	if (frame->state == 0) {                       \
		rframe = next_frame(frame, 1);               \
		estack.push(rframe);                         \
		estack.push(new ExprFrame(expr->getRight(),  \
					0, rframe, 1));                        \
		estack.push(new ExprFrame(expr->getLeft(),   \
					0, rframe, 0));                        \
	}                                              \
	else {                                         \
		if (frame->completed < 2) throw -1;          \
		send_frame(frame, Type(                      \
					frame->values[0] op frame->values[1]));\
	}                                              \
	break;

void send_frame(ExprFrame* curr, Type val) {
	if (curr->target) {
		curr->target->values[curr->slot] = val;
		(curr->target->completed)++;
	}
}

ExprFrame* next_frame(ExprFrame* curr, char state) {
	return new ExprFrame(curr->expr, state, curr->target, curr->slot);
}

void IterRun::process(Var* expr, ExprFrame* frame) {
	if (frame->state == 0) {
		if (vars->contain(expr->getVal()))
			send_frame(frame, vars->get(expr->getVal()));
		else {
			std::cerr << "Used undeclared var: " << expr->getVal() << std::endl;
			throw -1;
		}
	}
	else
		send_frame(frame, Type(expr->getVal()));
}

void IterRun::process(RawVal* expr, ExprFrame* frame) {
	send_frame(frame, expr->getVal());
}

void IterRun::process(UnOperation* expr, ExprFrame* frame) {
	ExprFrame* rframe;
	switch (expr->getOp()) {
		case OperationType::PRINT:
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getNext(), 0, rframe, 0));
			}
			else {
				if (frame->completed < 1) throw -1;
				std::cout << frame->values[0].to_string() << std::endl;
			}
			break;

		case OperationType::BLOCK:
			if (frame->state == 0) {
				vars = new MemNodeT(vars);
				estack.push(new ExprFrame(expr, 1));
				estack.push(new ExprFrame(expr->getNext()));
			}
			else {
				MemNodeT* node = vars;
				vars = vars->get_parent();
				delete node;
			}
			break;
		
		case OperationType::RETURN:
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getNext(), 0, rframe, 0));
			}
			else {
				while (estack.top() != ret.top()) {
					estack.pop();
				}
				if (estack.top()->target) {
					if (frame->completed < 1) throw -1;
					frame->target = estack.top()->target;
					frame->slot = estack.top()->slot;
					send_frame(frame, frame->values[0]);
				}
			}
			break;

		case OperationType::NOT:
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getNext(), 0, rframe, 0));
			}
			else {
				if (frame->completed < 1) throw -1;
				send_frame(frame, !(frame->values[0]));
			}
			break;

		default: throw -1;
	}
}

void IterRun::process(BiOperation* expr, ExprFrame* frame) {
	ExprFrame* rframe;

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
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getRight(), 0, rframe, 1));
				estack.push(new ExprFrame(expr->getLeft(), 1, rframe, 0));
			}
			else {
				if (frame->completed < 2) throw -1;
				if (frame->values[0].get_type() != Types::STRING) throw -2;
				vars->insert(frame->values[0].to_string(), frame->values[1]);
				send_frame(frame, frame->values[1]);
			}
			break;

		case OperationType::WHILE:
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getLeft(), 0, rframe, 0));
			}
			else if (frame->state == 1) {
				if (frame->completed < 1) throw - 1;
				if (frame->values[0].to_bool()) {
					estack.push(next_frame(frame, 0));
					estack.push(new ExprFrame(expr->getRight()));
				}
			}
			break;

		case OperationType::IF:
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getLeft(), 0, rframe, 0));
			}
			else if (frame->state == 1) {
				if (frame->completed < 1) throw - 1;
				if (frame->values[0].to_bool()) {
					estack.push(new ExprFrame(expr->getRight()));
				}
			}
			break;

		case OperationType::SEMI:
			estack.push(new ExprFrame(expr->getRight()));
			estack.push(new ExprFrame(expr->getLeft()));
			break;

		case OperationType::COMMA:
			if (frame->state == 0) { // число
				ExprFrame* rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getRight(), 0, rframe, 0));
				estack.push(new ExprFrame(expr->getLeft(), 0, rframe, 1));
			}
			else if (frame->state == 1) {
				if (frame->completed < 2) throw -1;
				send_frame(frame, frame->values[0] + frame->values[1]);
			}
			else if (frame->state == 2) {
				ExprFrame* rframe = next_frame(frame, 3);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getRight(), 2, rframe, 0));
				estack.push(new ExprFrame(expr->getLeft(), 2, rframe, 1));
			}
			else if (frame->state == 3) {
				if (frame->completed < 2) throw -1;
				std::vector<Type> vec = frame->values[0].to_list();
				std::vector<Type> vec2 = frame->values[1].to_list();

				vec.insert(vec.end(), vec2.begin(), vec2.end());
				send_frame(frame, Type(vec));
			}
			else if (frame->state == 4) {
				ExprFrame* rframe = next_frame(frame, 5);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getRight(), 4, rframe, 0));
				estack.push(new ExprFrame(expr->getLeft(), 4, rframe, 1));
			}
			else if (frame->state == 5) {
				if (frame->completed < 2) throw - 1;
				std::vector<Type> vec = frame->values[0].to_list();
				std::vector<Type> vec2 = frame->values[1].to_list();

				vec.insert(vec.end(), vec2.begin(), vec2.end());
				send_frame(frame, Type(vec));
			}
			break;

		default: throw -1;
	}
}

void IterRun::process(TriOperation* expr, ExprFrame* frame) {
	ExprFrame* rframe;

	switch (expr->getOp()) {
		case OperationType::IF:
			if (frame->state == 0) {
				rframe = next_frame(frame, 1);
				estack.push(rframe);
				estack.push(new ExprFrame(expr->getLeft(), 0, rframe, 0));
			}
			else if (frame->state == 1) {
				if (frame->completed < 1) throw -1;
				if (frame->values[0].to_bool()) {
					estack.push(new ExprFrame(expr->getMidle()));
				}
				else {
					estack.push(new ExprFrame(expr->getRight()));
				}
			}
			break;
		default: throw -1;
	}
}

void IterRun::process(FunctionParam* expr, ExprFrame* frame) {
	if (frame->state == 0) { // число
		send_frame(frame, Type(1));
	}
	else if (frame->state == 1) { // знач 2
		if (frame->completed < 1) throw -1;
		send_frame(frame, Type(std::vector<Type>(1, frame->values[0])));
	}
	else if (frame->state == 2) { // знач 1
		ExprFrame* rframe = next_frame(frame, 1);
		estack.push(rframe);
		estack.push(new ExprFrame(expr->getExpr(), 0, rframe, 0));
	}
	else if (frame->state == 3) { // имя 2
		if (frame->completed < 1) throw - 1;
		send_frame(frame, Type(std::vector<Type>(1, frame->values[0])));
	}
	else if (frame->state == 4) { // имя 1
		ExprFrame* rframe = next_frame(frame, 3);
		estack.push(rframe);
		estack.push(new ExprFrame(expr->getExpr(), 1, rframe, 0));
	}
}

void IterRun::process(FunctionDef* expr, ExprFrame* frame) {
	if (frame->state == 0) {
		expr->getScreen() = vars;
		ExprFrame* rframe = next_frame(frame, 1);
		estack.push(rframe);
		estack.push(new ExprFrame(expr->getName(), 1, rframe, 0));
		estack.push(new ExprFrame(expr->getParam(), 0, rframe, 1));
	}
	else if (frame->state == 1) {
		if (frame->completed < 2) throw -1;
		if (frame->values[0].get_type() != Types::STRING) throw -2;
		vars->insert(frame->values[0].to_string(),
				Type(expr, frame->values[1].to_int()));
	}
	else if (frame->state == 2) {
		retvars.push(vars);

		vars = new MemNodeT(expr->getScreen());

		ExprFrame* rframe = next_frame(frame, 3);
		estack.push(rframe);
		estack.push(new ExprFrame(expr->getParam(), 4, rframe, 1));

		frame->target = rframe;
		frame->slot = 0;
		if (frame->completed >= 1)
			send_frame(frame, frame->values[0]);
	}
	else if (frame->state == 3) {
		if (frame->completed >= 2) {
			auto v0 = frame->values[0].to_list();
			auto v0p = v0.begin();
			auto v1 = frame->values[1].to_list();
			auto v1p = v1.begin();
			while (v1p != v1.end()) {
				vars->insert_last((*v1p).to_string(), *v0p);
				v1p++;
				v0p++;
			}
		}

		ExprFrame* rframe = next_frame(frame, 4);
		estack.push(rframe);
		ret.push(rframe);

		estack.push(new ExprFrame(expr->getBody()));
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

void IterRun::process(FunctionCall* expr, ExprFrame* frame) {
	if (frame->state == 0) {
		ExprFrame* rframe = next_frame(frame, 1);
		estack.push(rframe);
		estack.push(new ExprFrame(expr->getName(), 1, rframe, 0));
		estack.push(new ExprFrame(expr->getParam(), 0, rframe, 1));
	}
	else {
		if (frame->completed < 2) throw -1;
		if (frame->values[0].get_type() != Types::STRING) throw -2;
		std::string name = frame->values[0].to_string();
		int count = frame->values[1].to_int();
		if (!(vars->contain(name) &&
					vars->get(name).get_type() == Types::FUNCTION &&
					vars->get(name).get_args_count() == count)) {
			std::cout << "Used undeclared function " << name << " with " << count << " arguments" << std::endl;
			throw -1;
		}

		ExprFrame* func = new ExprFrame(vars->get(name).get_function(), 2, frame->target, frame->slot);
		estack.push(func);
		estack.push(new ExprFrame(expr->getParam(), 2, func, 0));
	}
}

void IterRun::run(Expr* expr) {
	estack.push(new ExprFrame(expr, 0));
	while (!estack.empty()) {
		ExprFrame* curr = estack.top();
		estack.pop();
		curr->expr->process(this, curr);
		delete curr;
	}
}
