#include "tree.h"
#include "iter.h"
#include <string>

std::string Var::getVal() {
	return val;
}
void Var::process(IterRun* itr, ExprFrame* frame) {
	itr->process(this, frame);
}

Type RawVal::getVal() {
	return val;
}
void RawVal::process(IterRun *itr, ExprFrame* frame) {
	itr->process(this, frame);
}

OperationType UnOperation::getOp() {
	return op;
}
Expr* UnOperation::getNext() {
	return next;
}
void UnOperation::process(IterRun *itr, ExprFrame* frame) {
	itr->process(this, frame);
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
void BiOperation::process(IterRun *itr, ExprFrame* frame) {
	itr->process(this, frame);
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
void TriOperation::process(IterRun *itr, ExprFrame* frame) {
	itr->process(this, frame);
}

Expr* FunctionParam::getExpr() {
	return expr;
}
void FunctionParam::process(IterRun*itr, ExprFrame* frame) {
	itr->process(this, frame);
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
MemNodeT*& FunctionDef::getScreen() {
	return screen;
}
void FunctionDef::process(IterRun*itr, ExprFrame* frame) {
	itr->process(this, frame);
}

Expr* FunctionCall::getName() {
	return name;
}
Expr* FunctionCall::getParam() {
	return param;
}
void FunctionCall::process(IterRun*itr, ExprFrame* frame) {
	itr->process(this, frame);
}

