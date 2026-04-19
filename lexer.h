#pragma once

#include "token.h"
#include <queue>
#include <string>

#define addsc &Avt::addscope
#define tkstr &Avt::tokenstr
#define adder &Avt::adderror
#define tkint &Avt::tokenint
#define tkdbl &Avt::tokendbl
#define tksep &Avt::tokensep
#define clrsc &Avt::clrscope
#define tkprn &Avt::tokenprn
#define tkopr &Avt::tokenopr
#define skpch &Avt::skipchar

class Avt {
	enum State: int {START = 0, STR, ZERO, INT, OP, ERR, DOUBLE};
	
	int row, col;
	std::string scope;
	std::queue<Token> tokens;

	int getindex(char ch);
	void changepos(char ch);

	void addscope(char ch);
	void adderror(char ch);
	void tokenstr(char ch);
	void tokenint(char ch);
	void tokendbl(char ch);
	void clrscope(char ch);
	void tokenopr(char ch);
	void skipchar(char ch);
	
	State transitions[7][7] = {
		//err    a-z     op      0       1-9     \n      .
		{START,  STR,    OP,     ZERO,   INT,    START,  DOUBLE},//START
		{STR,    STR,    OP,     STR,    STR,    START,  ERR},//STR
		{ZERO,   ERR,    OP,     ERR,    ERR,    START,  DOUBLE},//ZERO
		{INT,    ERR,    OP,     INT,    INT,    START,  DOUBLE},//INT
		{OP,     STR,    OP,     ZERO,   INT,    START,  DOUBLE},//OP
		{ERR,    ERR,    OP,     ERR,    ERR,    START,  ERR},//ERR
		{DOUBLE, ERR,    OP,     DOUBLE, DOUBLE, START,  ERR},//DOUBLE
	};

	void (Avt::*funcs[7][7])(char) = {
		//err   a-z    op     0      1-9    \n     .
		{skpch, addsc, addsc, addsc, addsc, clrsc, addsc},//START
		{skpch, addsc, tkstr, addsc, addsc, tkstr, addsc},//STR
		{skpch, addsc, tkint, addsc, addsc, tkint, addsc},//ZERO
		{skpch, addsc, tkint, addsc, addsc, tkint, addsc},//INT
		{skpch, tkopr, tkopr, tkopr, tkopr, tkopr, tkopr},//OP
		{addsc, addsc, adder, addsc, addsc, adder, addsc},//ERR
		{skpch, addsc, tkdbl, addsc, addsc, tkdbl, addsc},//DOUBLE
	};

public:
	Avt(): row(1), col(1), scope("") {}
	std::queue<Token> fromString(std::string str);
};

#undef addsc
#undef tkstr
#undef adder
#undef tkint
#undef tkdbl
#undef tksep
#undef clrsc
#undef tkprn
#undef tkopr
#undef skpch


