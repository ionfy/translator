#pragma once
#include <variant>
#include <string>

enum class Types {INT, DOUBLE, STRING};

struct Type {
	Types type;
	std::variant<int, double, std::string> val;
	Type(Types _type, int _val): type(_type), val(_val) {}
	Type(Types _type, double _val): type(_type), val(_val) {}
	Type(Types _type, std::string _val): type(_type), val(_val) {}
};
