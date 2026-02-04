#pragma once
#include <exception>

class BadAnyTypeCast : public std::exception
{
public:
	const char* what() const noexcept override;
};

class AnyType {
public:
	enum class Type {
		None,
		Bool,
		Char,
		Int,
		Float,
		Double,
		Long
	};

	AnyType();
	AnyType(bool);
	AnyType(char);
	AnyType(int);
	AnyType(float);
	AnyType(double);
	AnyType(long);

	~AnyType();

	AnyType(const AnyType&);
	AnyType(AnyType&&) noexcept;

	AnyType& operator=(const AnyType&);
	AnyType& operator=(AnyType&&) noexcept;

	AnyType& operator=(bool);
	AnyType& operator=(char);
	AnyType& operator=(int);
	AnyType& operator=(float);
	AnyType& operator=(double);
	AnyType& operator=(long);

	void Reset();
	void Swap(AnyType&) noexcept;
	Type GetType() const noexcept;

	bool ToBool() const;
	char ToChar() const;
	int ToInt() const;
	float ToFloat() const;
	double ToDouble() const;
	long ToLong() const;

private:
	union Storage {
		bool b;
		char c;
		int i;
		float f;
		double d;
		long l;

		Storage() {}
		~Storage() {}
	};

	Storage storage;
	Type type = Type::None;

	void copyFrom(const AnyType&);
	void moveFrom(AnyType&&);

	void store(bool);
	void store(char);
	void store(int);
	void store(float);
	void store(double);
	void store(long);
};