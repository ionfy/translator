#pragma once

#include "table.h"
#include <string>

enum class OperationType: int {
	ADD, SUB, MUL, DIV,
	ASSIGN,
	SEMI,
	NOT, EQUAL, NEQUAL, LESS, GREATER, LEQUAL, GEQUAL,
	WHILE, IF,
	COMMA,
	RETURN,
	BLOCK,
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

class FunctionParam: public Expr {
	Expr* expr;
public:
	FunctionParam(Expr* _expr): expr(_expr) {}
	Expr* getExpr();
	void process(IterRun *itr, char state) override;
};

class FunctionDef: public Expr {
	Expr* name;
	Expr* param;
	Expr* body;
	TTable<std::string, int> screen;
public:
	FunctionDef(Expr* _name, Expr* _param, Expr* _body): name(_name), param(_param), body(_body) {}
	Expr* getName();
	Expr* getParam();
	Expr* getBody();
	TTable<std::string, int>& getScreen();
	void process(IterRun *itr, char state) override;
};

struct FuncDesc {
	std::string name;
	char pcount;

	int operator== (const FuncDesc& other) {
		return name == other.name && pcount == other.pcount;
	}

	int operator< (const FuncDesc& other) {
		if (name != other.name) return name < other.name;
		return pcount < other.pcount;
	}

	int operator> (const FuncDesc& other) {
		if (name != other.name) return name > other.name;
		return pcount > other.pcount;
	}
};

class FunctionCall: public Expr {
	Expr* name;
	Expr* param;
	FuncDesc desc;
	TTable<std::string, int> temp;
public:
	FunctionCall(Expr* _name, Expr* _param): name(_name), param(_param) {}
	Expr* getName();
	Expr* getParam();
	FuncDesc& getDesc();
	TTable<std::string, int>& getTemp();
	void process(IterRun *itr, char state) override;
};

