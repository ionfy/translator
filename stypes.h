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
	Type(bool _val) : type(Types::BOOL), bool_val(_val) {}
	Type(int _val): type(Types::INT), int_val(_val) {}
	Type(double _val): type(Types::DOUBLE), double_val(_val) {}
	Type(std::string _val): type(Types::STRING), string_val(_val) {}

	Types get_type();

	bool is_numeric();
	bool to_bool();
	int to_int();
	double to_double();
	std::string to_string();
};

