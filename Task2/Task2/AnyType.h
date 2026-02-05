#pragma once

#include <stdexcept>
#include <type_traits>

class AnyType {
public:
	enum class Type {
		None,
		Bool,
		Char,
		SigChar,
		UChar,
		Short,
		UShort,
		Int,
		UInt,
		Long,
		ULong,
		LongLong,
		ULongLong,
		Float,
		Double,
		LongDouble
	};

	AnyType() noexcept : data{}, currentType(Type::None) {}

	AnyType(const AnyType& other) noexcept : data(other.data), currentType(other.currentType) {}

	AnyType(AnyType&& other) noexcept : data(other.data), currentType(other.currentType) {
		other.currentType = Type::None;
	}

	template <typename T>
	explicit AnyType(T val) {
		static_assert(std::is_arithmetic_v<T>, "AnyType only supports arithmetic types");
		setData(val);
	}

	template <typename T>
	AnyType& operator=(T val) {
		static_assert(std::is_arithmetic_v<T>, "AnyType only supports arithmetic types");
		setData(val);
		return *this;
	}

	AnyType& operator=(const AnyType& other) noexcept {
		if (this != &other) {
			currentType = other.currentType;
			data = other.data;
		}
		return *this;
	}

	AnyType& operator=(AnyType&& other) noexcept {
		if (this != &other) {
			currentType = other.currentType;
			data = other.data;
			other.currentType = Type::None;
		}
		return *this;
	}

	~AnyType() noexcept = default;

	void destroy() noexcept { currentType = Type::None; }

	void swapWith(AnyType& other) noexcept {
		std::swap(currentType, other.currentType);
		std::swap(data, other.data);
	}

	Type typeof() const noexcept { return currentType; }

	template <typename T>
	T get() const {
		static_assert(std::is_arithmetic_v<T>, "Type not supported");

		if (currentType == Type::None)
			throw std::runtime_error("Can't get value from empty AnyType");

		if (toType<T>() != currentType)
			throw std::invalid_argument("Type mismatch in AnyType::get()");

		if constexpr (std::is_same_v<T, bool>) return data.b;
		else if constexpr (std::is_same_v<T, char>) return data.c;
		else if constexpr (std::is_same_v<T, unsigned char>) return data.uc;
		else if constexpr (std::is_same_v<T, signed char>) return data.sc;
		else if constexpr (std::is_same_v<T, short>) return data.s;
		else if constexpr (std::is_same_v<T, unsigned short>) return data.us;
		else if constexpr (std::is_same_v<T, int>) return data.i;
		else if constexpr (std::is_same_v<T, unsigned int>) return data.ui;
		else if constexpr (std::is_same_v<T, long>) return data.l;
		else if constexpr (std::is_same_v<T, unsigned long>) return data.ul;
		else if constexpr (std::is_same_v<T, long long>) return data.ll;
		else if constexpr (std::is_same_v<T, unsigned long long>) return data.ull;
		else if constexpr (std::is_same_v<T, float>) return data.f;
		else if constexpr (std::is_same_v<T, double>) return data.d;
		else if constexpr (std::is_same_v<T, long double>) return data.ld;
		else throw std::logic_error("Unsupported type");
	}
	
	template <typename T>
	void set(T val) {
		static_assert(std::is_arithmetic_v<T>, "AnyType only supports arithmetic types");
		setData(val);
	}

	template <typename T>
	static constexpr Type toType() noexcept {
		if constexpr (std::is_same_v<T, bool>) return Type::Bool;
		else if constexpr (std::is_same_v<T, char>) return Type::Char;
		else if constexpr (std::is_same_v<T, unsigned char>) return Type::UChar;
		else if constexpr (std::is_same_v<T, signed char>) return Type::SigChar; 
		else if constexpr (std::is_same_v<T, short>) return Type::Short;
		else if constexpr (std::is_same_v<T, unsigned short>) return Type::UShort;
		else if constexpr (std::is_same_v<T, int>) return Type::Int;
		else if constexpr (std::is_same_v<T, unsigned int>) return Type::UInt;
		else if constexpr (std::is_same_v<T, long>) return Type::Long;
		else if constexpr (std::is_same_v<T, unsigned long>) return Type::ULong;
		else if constexpr (std::is_same_v<T, long long>) return Type::LongLong;
		else if constexpr (std::is_same_v<T, unsigned long long>) return Type::ULongLong;
		else if constexpr (std::is_same_v<T, float>) return Type::Float;
		else if constexpr (std::is_same_v<T, double>) return Type::Double;
		else if constexpr (std::is_same_v<T, long double>) return Type::LongDouble;
		else return Type::None;
	}

private:
	union Storage {
		bool b;
		char c;
		unsigned char uc;
		signed char sc;
		short s;
		unsigned short us;
		int i;
		unsigned int ui;
		long l;
		unsigned long ul;
		long long ll;
		unsigned long long ull;
		float f;
		double d;
		long double ld;

		Storage() : ld(0) {}
		~Storage() {}
	};

	Storage data{};
	Type currentType;

	template <typename T>
	void setData(T val) noexcept {
		currentType = toType<T>();
		if constexpr (std::is_same_v<T, bool>) data.b = val;
		else if constexpr (std::is_same_v<T, char>) data.c = val;
		else if constexpr (std::is_same_v<T, unsigned char>) data.uc = val;
		else if constexpr (std::is_same_v<T, signed char>) data.sc = val;
		else if constexpr (std::is_same_v<T, short>) data.s = val;
		else if constexpr (std::is_same_v<T, unsigned short>) data.us = val;
		else if constexpr (std::is_same_v<T, int>) data.i = val;
		else if constexpr (std::is_same_v<T, unsigned int>) data.ui = val;
		else if constexpr (std::is_same_v<T, long>) data.l = val;
		else if constexpr (std::is_same_v<T, unsigned long>) data.ul = val;
		else if constexpr (std::is_same_v<T, long long>) data.ll = val;
		else if constexpr (std::is_same_v<T, unsigned long long>) data.ull = val;
		else if constexpr (std::is_same_v<T, float>) data.f = val;
		else if constexpr (std::is_same_v<T, double>) data.d = val;
		else if constexpr (std::is_same_v<T, long double>) data.ld = val;
	}
};
