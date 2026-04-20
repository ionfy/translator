#pragma once
#include <string>

enum class Types {NONE, BOOL, INT, DOUBLE, STRING};

class Type {
	Types type;
	bool bool_val = 0;
	int int_val = 0;
	double double_val = 0;
	std::string string_val = "";
public:
	Type(): type(Types::NONE) {};
	Type(Types _type, bool _val) : type(_type), bool_val(_val) {}
	Type(Types _type, int _val): type(_type), int_val(_val) {}
	Type(Types _type, double _val): type(_type), double_val(_val) {}
	Type(Types _type, std::string _val): type(_type), string_val(_val) {}

	Types get_type();

	bool is_numeric();
	bool to_bool();
	int to_int();
	double to_double();
	std::string to_string();
};

