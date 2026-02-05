#pragma once
#include <typeinfo>
#include <stdexcept>

class AnyType
{
private:
	union ValueUnion {
		int int_val;
		short short_val;
		long long_val;

		char char_val;

		float float_val;
		double double_val;

		bool bool_val;
	} value;

	const std::type_info* type = nullptr;


	template<typename T>
	void setValue(T val) {
		if constexpr (std::is_same_v<T, int>) {
			value.int_val = val;
		}
		else if constexpr (std::is_same_v<T, short>) {
			value.short_val = val;
		}
		else if constexpr (std::is_same_v<T, long>) {
			value.long_val = val;
		}
		else if constexpr (std::is_same_v<T, char>) {
			value.char_val = val;
		}
		else if constexpr (std::is_same_v<T, float>) {
			value.float_val = val;
		}
		else if constexpr (std::is_same_v<T, double>) {
			value.double_val = val;
		}
		else if constexpr (std::is_same_v<T, bool>) {
			value.bool_val = val;
		}
		else{
			throw std::runtime_error("Unsupported type");
		}
	}

public:
	AnyType() : type(&typeid(void)){}

	template<typename T>
	AnyType(T val) {
		setValue(val);
		type = &typeid(T);
	}

	AnyType(const AnyType& other) {
		value = other.value;
		type = other.type;
	}

	AnyType(AnyType&& other) noexcept{
		value = other.value;
		type = other.type;
		other.destroy();
	}

	AnyType& operator=(const AnyType& other) {
		value = other.value;
		type = other.type;
		return *this;
	}

	AnyType& operator=(AnyType&& other) noexcept {
		value = other.value;
		type = other.type;
		other.type = &typeid(void);
		return *this;
	}

	template<typename T>
	AnyType& operator=(T val) {
		setValue(val);
		type = &typeid(T);
		return *this;
	}

	void swap(AnyType& other) {
		std::swap(value, other.value);
		std::swap(type, other.type);
	}

	int toInt() const {
		if (*type != typeid(int)) {
			throw std::bad_cast();
		}
		return value.int_val;
	}

	short toShort() const {
		if (*type != typeid(short)) {
			throw std::bad_cast();
		}
		return value.short_val;
	}

	long toLong() const {
		if (*type != typeid(long)) {
			throw std::bad_cast();
		}
		return value.long_val;
	}

	char toChar() const {
		if (*type != typeid(char)) {
			throw std::bad_cast();
		}
		return value.char_val;
	}

	float toFloat() const {
		if (*type != typeid(float)) {
			throw std::bad_cast();
		}
		return value.float_val;
	}

	double toDouble() const {
		if (*type != typeid(double)) {
			throw std::bad_cast();
		}
		return value.double_val;
	}

	bool toBool() const {
		if (*type != typeid(bool)) {
			throw std::bad_cast();
		}
		return value.bool_val;
	}

	void destroy() {
		type = &typeid(void);
	}

	const char* getType() const{
		return type->name();
	}
};
