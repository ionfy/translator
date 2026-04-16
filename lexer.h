#pragma once

#include "token.h"
#include <queue>
#include <string>

#define addsc &Avt::addscope
#define tkstr &Avt::tokenstr
#define adder &Avt::adderror
#define tkint &Avt::tokenint
#define tksep &Avt::tokensep
#define clrsc &Avt::clrscope
#define tkprn &Avt::tokenprn
#define tkopr &Avt::tokenopr
#define skpch &Avt::skipchar

class Avt {
	enum State: int {START = 0, STR, ZERO, INT, OP, ERR};
	
	int row, col;
	std::string scope;
	std::queue<Token> tokens;

	int getindex(char ch);
	void changepos(char ch);

	void addscope(char ch);
	void adderror(char ch);
	void tokenstr(char ch);
	void tokenint(char ch);
	void clrscope(char ch);
	void tokenopr(char ch);
	void skipchar(char ch);
	
	State transitions[6][6] = {
		//err    a-z     op      0       1-9     \n
		{START,  STR,    OP,     ZERO,   INT,    START,  },//START
		{STR,    STR,    OP,     STR,    STR,    START,  },//STR
		{ZERO,   ERR,    OP,     ERR,    ERR,    START,  },//ZERO
		{INT,    START,  OP,     INT,    INT,    START,  },//INT
		{OP,     STR,    OP,     ZERO,   INT,    START,  },//OP
		{ERR,    ERR,    OP,     ERR,    ERR,    START,  },//ERR
	};

	void (Avt::*funcs[6][6])(char) = {
		//err   a-z    op     0      1-9    \n
		{skpch, addsc, addsc, addsc, addsc, clrsc, },//START
		{skpch, addsc, tkstr, addsc, addsc, tkstr, },//STR
		{skpch, addsc, tkint, addsc, addsc, tkint, },//ZERO
		{skpch, addsc, tkint, addsc, addsc, tkint, },//INT
		{skpch, tkopr, tkopr, tkopr, tkopr, tkopr, },//OP
		{addsc, addsc, adder, addsc, addsc, adder, },//ERR
	};

public:
	Avt(): row(1), col(1), scope("") {}
	std::queue<Token> fromString(std::string str);
};

#undef addsc
#undef tkstr
#undef adder
#undef tkint
#undef tksep
#undef clrsc
#undef tkprn
#undef tkopr
#undef skpch


