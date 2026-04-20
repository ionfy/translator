#include "tree.h"
#include "iter.h"
#include <string>

std::string Var::getVal() {
	return val;
}
void Var::process(IterRun *itr, char state) {
	itr->process(this, state);
}

Type RawVal::getVal() {
	return val;
}
void RawVal::process(IterRun *itr, char state) {
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

Expr* FunctionParam::getExpr() {
	return expr;
}
void FunctionParam::process(IterRun*itr, char state) {
	itr->process(this, state);
}

Expr* FunctionDef::getName() {
	return name;
}
Expr* FunctionDef::getParam() {
	return param;
}
Expr* FunctionDef::getBody() {
	return body;
}
TTable<std::string, Type>& FunctionDef::getScreen() {
	return screen;
}
void FunctionDef::process(IterRun*itr, char state) {
	itr->process(this, state);
}

Expr* FunctionCall::getName() {
	return name;
}
Expr* FunctionCall::getParam() {
	return param;
}
FuncDesc& FunctionCall::getDesc() {
	return desc;
}
TTable<std::string, Type>& FunctionCall::getTemp() {
	return temp;
}
void FunctionCall::process(IterRun*itr, char state) {
	itr->process(this, state);
}

