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

	Types get_type() const;

	bool is_numeric() const;
	bool to_bool() const;
	int to_int() const;
	double to_double() const;
	std::string to_string() const;

	Type operator+(const Type& other) const;
	Type operator-(const Type& other) const;
	Type operator*(const Type& other) const;
	Type operator/(const Type& other) const;

	Type operator!() const;
	Type operator==(const Type& other) const;
	Type operator!=(const Type& other) const;
	Type operator<(const Type& other) const;
	Type operator<=(const Type& other) const;
	Type operator>(const Type& other) const;
	Type operator>=(const Type& other) const;
};

