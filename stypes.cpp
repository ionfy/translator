#include "stypes.h"
#include <string>

Types Type::get_type() const {
	return type;
}

bool Type::is_numeric() const{
	if (type == Types::BOOL || type == Types::INT || type == Types::DOUBLE) return 1;
	else return 0;
}

bool Type::to_bool() const {
	if (type == Types::NONE) return (bool)0;
	if (type == Types::BOOL) return (bool)bool_val;
	if (type == Types::INT) return (bool)int_val;
	if (type == Types::DOUBLE) return (bool)double_val;
	if (type == Types::STRING) return string_val != "";
	throw -1;
}

int Type::to_int() const {
	if (type == Types::NONE) return (int)0;
	if (type == Types::BOOL) return (int)bool_val;
	if (type == Types::INT) return (int)int_val;
	if (type == Types::DOUBLE) return (int)double_val;
	if (type == Types::STRING) return (int)0;
	throw -1;
}

double Type::to_double() const {
	if (type == Types::NONE) return (double)0;
	if (type == Types::BOOL) return (double)bool_val;
	if (type == Types::INT) return (double)int_val;
	if (type == Types::DOUBLE) return (double)double_val;
	if (type == Types::STRING) return (double)0;
	throw -1;
}

std::string Type::to_string() const {
	if (type == Types::NONE) return "";
	if (type == Types::BOOL) return bool_val ? "true" : "false";
	if (type == Types::INT) return std::to_string(int_val);
	if (type == Types::DOUBLE) return std::to_string(double_val);
	if (type == Types::STRING) return string_val;
	throw -1;
}

Type Type::operator!() const {
	return Type(!to_bool());
}

Type Type::operator+(const Type& other) const {
	if (type == Types::STRING || other.type == Types::STRING)
		return Type(to_string()+other.to_string());
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() + other.to_double());
		return Type(to_int() + other.to_int());
	}
	return Type(to_string() + other.to_string());
}

Type Type::operator-(const Type& other) const {
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() - other.to_double());
		return Type(to_int() - other.to_int());
	}
	throw - 1;
}

Type Type::operator*(const Type& other) const {
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() * other.to_double());
		return Type(to_int() * other.to_int());
	}
	throw - 1;
}

Type Type::operator/(const Type& other) const {
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() / other.to_double());
		return Type(to_int() / other.to_int());
	}
	throw - 1;
}

Type Type::operator==(const Type& other) const {
	if (type == Types::STRING || other.type == Types::STRING)
		return Type(to_string() == other.to_string());
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() == other.to_double());
		return Type(to_int() == other.to_int());
	}
	return Type(to_string() == other.to_string());
}

Type Type::operator!=(const Type& other) const {
	return !(*this==other);
}

Type Type::operator<(const Type& other) const {
	if (type == Types::STRING || other.type == Types::STRING)
		return Type(to_string() < other.to_string());
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() < other.to_double());
		return Type(to_int() < other.to_int());
	}
	return Type(to_string() < other.to_string());
}

Type Type::operator>(const Type& other) const {
	if (type == Types::STRING || other.type == Types::STRING)
		return Type(to_string() > other.to_string());
	if (is_numeric() && other.is_numeric()) {
		if (type == Types::DOUBLE || other.type == Types::DOUBLE)
			return Type(to_double() > other.to_double());
		return Type(to_int() > other.to_int());
	}
	return Type(to_string() > other.to_string());
}

Type Type::operator<=(const Type& other) const {
	return !(*this>other);
}

Type Type::operator>=(const Type& other) const {
	return !(*this<other);
}

