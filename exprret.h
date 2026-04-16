#pragma once
#include <string>

struct ExprRet {
	double val;
	std::string var;
	ExprRet(double _val) : val(_val) {};
	ExprRet(std::string _var) : var(_var) {};
};
