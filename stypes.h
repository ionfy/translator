#pragma once
#include <string>
#include <vector>

enum class Types {NONE, BOOL, INT, DOUBLE, STRING, FUNCTION, LIST};

class FunctionDef;

class Type {
	Types type;
	bool bool_val = 0;
	int int_val = 0;
	double double_val = 0;
	std::string string_val = "";
	FunctionDef* func = nullptr;
	char args_count = 0;
	std::vector<Type> list_val;
public:
	Type(): type(Types::NONE) {};
	Type(bool _val) : type(Types::BOOL), bool_val(_val) {}
	Type(int _val): type(Types::INT), int_val(_val) {}
	Type(double _val): type(Types::DOUBLE), double_val(_val) {}
	Type(std::string _val): type(Types::STRING), string_val(_val) {}
	Type(FunctionDef* _func, char _val) : type(Types::FUNCTION), func(_func), args_count(_val) {}
	Type(std::vector<Type> _val) : type(Types::LIST), list_val(_val) {}

	Types get_type() const;
	FunctionDef* get_function() const;
	char get_args_count() const;
	std::vector<Type>& get_list();

	bool is_numeric() const;
	bool to_bool() const;
	int to_int() const;
	double to_double() const;
	std::string to_string() const;
	std::vector<Type> to_list() const;

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

