#pragma once
#include <variant>
#include <string>

enum class Types {NONE, INT, DOUBLE, STRING};

struct Type {
	Types type;
	std::variant<int, double, std::string> val;

	Type(): type(Types::NONE), val(0) {};
	Type(Types _type, int _val): type(_type), val(_val) {}
	Type(Types _type, double _val): type(_type), val(_val) {}
	Type(Types _type, std::string _val): type(_type), val(_val) {}
};

