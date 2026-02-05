#include "AnyType.h"
#include <iostream>

AnyType::AnyType() : type(TypeId::Empty) {};

AnyType::AnyType(int val) : type(TypeId::Int), int_data(val) {};
AnyType::AnyType(bool val) : type(TypeId::Bool), bool_data(val) {};
AnyType::AnyType(double val) : type(TypeId::Double), double_data(val) {};
AnyType::AnyType(char val) : type(TypeId::Char), char_data(val) {};
AnyType::AnyType(float val) : type(TypeId::Float), float_data(val) {};


AnyType::AnyType(AnyType&& other) noexcept : AnyType() {
	this->Swap(other);
	other.type = TypeId::Empty;
	other.double_data = 0.0;
}

AnyType::AnyType(const AnyType& other) {

	switch (other.type) {
	case TypeId::Int:
		type = TypeId::Int;
		int_data = other.int_data;
		break;
	case TypeId::Bool:
		type = TypeId::Bool;
		bool_data = other.bool_data;
		break;
	case TypeId::Double:
		type = TypeId::Double;
		double_data = other.double_data;
		break;
	case TypeId::Char:
		type = other.type;
		char_data = other.char_data;
		break;
	case TypeId::Float:
		type = other.type;
		float_data = other.float_data;
		break;
	case TypeId::Empty:
		type = TypeId::Empty;
		double_data = 0.0;
		break;
	}
};



AnyType& AnyType::operator=(AnyType other) {
	this->Swap(other);
	return *this;
}

AnyType& AnyType::operator=(int val) {
	type = TypeId::Int;
	int_data = val;
	return *this;
}
AnyType& AnyType::operator=(bool val) {
	type = TypeId::Bool;
	bool_data = val;
	return *this;
}
AnyType& AnyType::operator=(double val) {
	type = TypeId::Double;
	double_data = val;
	return *this;
}
AnyType& AnyType::operator=(char val) {
	type = TypeId::Char;
	char_data = val;
	return *this;
}
AnyType& AnyType::operator=(float val) {
	type = TypeId::Float;
	float_data = val;
	return *this;
}

int AnyType::ToInt() const {
	if (type == TypeId::Int)
	{
		return 	int_data;
	}
	throw BadAnyCast();
}
bool AnyType::ToBool() const {
	if (type == TypeId::Bool)
	{
		return bool_data;
	}
	throw BadAnyCast();
}
double AnyType::ToDouble() const {
	if (type == TypeId::Double)
	{
		return double_data;
	}
	throw BadAnyCast();
}
char AnyType::ToChar() const {
	if (type == TypeId::Char)
	{
		return char_data;
	}
	throw BadAnyCast();
}
float AnyType::ToFloat() const {
	if (type == TypeId::Float)
	{
		return float_data;
	}
	throw BadAnyCast();
}

void AnyType::Swap(AnyType& other) {

	std::swap(other.type, this->type);
	std::swap(other.double_data, this->double_data);

}

const char* AnyType::get_type() const
{
	switch (type) {
	case TypeId::Int: return "INT";
	case TypeId::Float: return "FLOAT";
	case TypeId::Double: return "DOUBLE";
	case TypeId::Char: return "CHAR";
	case TypeId::Bool: return "BOOL";
	default: return "UNDEFINED";
	}
}

void AnyType::get_data() const
{
	switch (this->type) {
	case TypeId::Int: std::cout << this->int_data; break;
	case TypeId::Float: std::cout << this->float_data; break;
	case TypeId::Double: std::cout << this->double_data; break;
	case TypeId::Char: std::cout << this->char_data; break;
	case TypeId::Bool: std::cout << this->bool_data; break;
	default: std::cout << 0;
	}
}

void AnyType::destroy() {
	type = TypeId::Empty;
}

void AnyType::print() {
	std::cout << "Tag " << get_type();
	std::cout << ' ';
	get_data();
	std::cout << std::endl;
}
