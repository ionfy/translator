#pragma once

#include <string>

enum class OperationType: int {
	ADD, SUB, MUL, DIV,
	ASSIGN,
	SEMI,
	NOT, EQUAL, NEQUAL, LESS, GREATER, LEQUAL, GEQUAL,
	WHILE, IF,
	PRINT,
};

class IterRun;

class Expr {
public:
	virtual void process(IterRun* itr, char state) = 0;
};

class Var : public Expr {
	std::string val;
public:
	Var(std::string _val) : val(_val) {}
	std::string getVal();
	void process(IterRun *itr, char state) override;
};

class Number: public Expr {
	int val;
public:
	Number(int _val): val(_val) {}
	Number(std::string _val);
	int getVal();
	void process(IterRun *itr, char state) override;
};

class UnOperation: public Expr {
	OperationType op;
	Expr* next;
public:
	UnOperation(OperationType _op, Expr* _next): op(_op), next(_next) {}
	OperationType getOp();
	Expr* getNext();
	void process(IterRun *itr, char state) override;
};

class BiOperation: public Expr {
	OperationType op;
	Expr* left;
	Expr* right;
public:
	BiOperation(OperationType _op, Expr* _left, Expr* _right): op(_op), left(_left), right(_right) {}
	OperationType getOp();
	Expr* getLeft();
	Expr* getRight();
	void process(IterRun *itr, char state) override;
};

class TriOperation: public Expr {
	OperationType op;
	Expr* left;
	Expr* midle;
	Expr* right;
public:
	TriOperation(OperationType _op, Expr* _left, Expr* _midle, Expr* _right): op(_op), left(_left), midle(_midle), right(_right) {}
	OperationType getOp();
	Expr* getLeft();
	Expr* getMidle();
	Expr* getRight();
	void process(IterRun *itr, char state) override;
};

