#include "stypes.h"
#include <string>

Types Type::get_type() {
	return type;
}

bool Type::is_numeric() {
	if (type == Types::BOOL || type == Types::INT || type == Types::DOUBLE) return 1;
	else return 0;
}

bool Type::to_bool() {
	if (type == Types::NONE) return (bool)0;
	if (type == Types::BOOL) return (bool)bool_val;
	if (type == Types::INT) return (bool)int_val;
	if (type == Types::DOUBLE) return (bool)double_val;
	if (type == Types::STRING) return string_val != "";
	throw -1;
}

int Type::to_int() {
	if (type == Types::NONE) return (int)0;
	if (type == Types::BOOL) return (int)bool_val;
	if (type == Types::INT) return (int)int_val;
	if (type == Types::DOUBLE) return (int)double_val;
	if (type == Types::STRING) return (int)0;
	throw -1;
}

double Type::to_double() {
	if (type == Types::NONE) return (double)0;
	if (type == Types::BOOL) return (double)bool_val;
	if (type == Types::INT) return (double)int_val;
	if (type == Types::DOUBLE) return (double)double_val;
	if (type == Types::STRING) return (double)0;
	throw -1;
}

std::string Type::to_string() {
	if (type == Types::NONE) return "NONE";
	if (type == Types::BOOL) return bool_val ? "TRUE" : "FALSE";
	if (type == Types::INT) return std::to_string(int_val);
	if (type == Types::DOUBLE) return std::to_string(double_val);
	if (type == Types::STRING) return string_val;
	throw -1;
}

