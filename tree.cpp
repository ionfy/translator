#include "tree.h"
#include "iter.h"
#include <string>

std::string Var::getVal() {
	return val;
}
void Var::process(IterRun *itr, char state) {
	itr->process(this, state);
}

Number::Number(std::string _val) {
	val = 0;
	for (char ch: _val) {
		val *= 10;
		val += ch - '0';
	}
}
int Number::getVal() {
	return val;
}
void Number::process(IterRun *itr, char state) {
	itr->process(this, state);
}

OperationType UnOperation::getOp() {
	return op;
}
Expr* UnOperation::getNext() {
	return next;
}
void UnOperation::process(IterRun *itr, char state) {
	itr->process(this, state);
}

OperationType BiOperation::getOp() {
	return op;
}
Expr* BiOperation::getLeft() {
	return left;
}
Expr* BiOperation::getRight() {
	return right;
}
void BiOperation::process(IterRun *itr, char state) {
	itr->process(this, state);
}

OperationType TriOperation::getOp() {
	return op;
}
Expr* TriOperation::getLeft() {
	return left;
}
Expr* TriOperation::getMidle() {
	return midle;
}
Expr* TriOperation::getRight() {
	return right;
}
void TriOperation::process(IterRun *itr, char state) {
	itr->process(this, state);
}
