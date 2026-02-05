#pragma once

#include <exception>
class BadAnyCast : public std::exception {
public:
	char const* what() const override {
		return "Bad type casting caused an error!";
	}
};

class AnyType
{
public:
	enum class TypeId { Int, Double, Bool, Float, Char, Empty };
	AnyType();
	AnyType(int val);
	AnyType(bool val);
	AnyType(double val);
	AnyType(char val);
	AnyType(float val);
	AnyType(const AnyType& other);

	~AnyType() = default;

	AnyType(AnyType&& other) noexcept;

	AnyType& operator=(AnyType other);
	AnyType& operator=(int val);
	AnyType& operator=(bool val);
	AnyType& operator=(double val);
	AnyType& operator=(char val);
	AnyType& operator=(float val);



	int ToInt() const;
	bool ToBool() const;
	double ToDouble() const;
	char ToChar() const;
	float ToFloat() const;

	void Swap(AnyType& other);
	const char* get_type() const;
	void get_data() const;
	void destroy();
	void print();

private:
	TypeId type;
	union {
		int int_data;
		bool bool_data;
		double double_data;
		char char_data;
		float float_data;
	};
};

