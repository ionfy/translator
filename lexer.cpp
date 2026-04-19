#include "lexer.h"
#include <queue>
#include <string>

int iskeyword(std::string str) {
	if (str == "while" || str == "if" || str == "else" ||
			str == "print" || str == "fn" || str == "return") return 1;
	return 0;
}

int isdoubleoperation(std::string str) {
	if (str == "==" || str == "!=" || str == "<=" ||
			str == ">=" ) return 1;
	return 0;
}

int Avt::getindex(char ch) {
	if (ch >= 'a' && ch <= 'z') return 1;
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '=' || ch == ';' ||
		ch == '(' || ch == ')' || ch == '!' ||
		ch == '<' || ch == '>' || ch == '{' ||
		ch == '}' || ch == ',') return 2;
	if (ch == '0') return 3;
	if (ch >= '1' && ch <= '9') return 4;
	if (ch == ' ' || ch == '\n') return 5;
	return 0;
}

void Avt::changepos(char ch) {
	if (ch == '\n') {
		col = 1;
		row++;
	}
	else {
		col++;
	}
}

void Avt::addscope(char ch) {
	scope += ch;
}

void Avt::adderror(char ch) {
	scope.clear();
	if (getindex(ch) != 5) scope += ch;
}

void Avt::tokenstr(char ch) {
	if (iskeyword(scope))
		tokens.push({TokenType::KEYWORD, scope});
	else
		tokens.push({TokenType::VARIABLE, scope});
	scope.clear();
	if (getindex(ch) != 5) scope += ch;
}

void Avt::tokenint(char ch) {
	tokens.push({TokenType::INTEGER, scope});
	scope.clear();
	if (getindex(ch) != 5) scope += ch;
}

void Avt::clrscope(char ch) {
	scope.clear();
}

void Avt::tokenopr(char ch) {
	if (isdoubleoperation(scope + ch)) {
		scope += ch;
	}
	else {
		tokens.push({ TokenType::OPERATION, scope});
		scope.clear();
		if (getindex(ch) != 5) scope += ch;
	}
}

void Avt::skipchar(char ch) {}

std::queue<Token> Avt::fromString(std::string str) {
	str += " ";
	scope = "";
	tokens = std::queue<Token>();
	row = 0;
	col = 0;
	int state = (int)START;
	for (char ch: str) {
		int ind = getindex(ch);
		(this->*funcs[state][ind])(ch);
		state = (int)transitions[state][ind];
		changepos(ch);
	}
	return tokens;
}

