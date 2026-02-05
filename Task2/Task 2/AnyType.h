#pragma once

#include <typeinfo>
#include <type_traits>
#include <stdexcept>
#include <algorithm>


class AnyType
{
private:
	// tagged union pattern
	union uStorage {
		int intType;
		unsigned int uiType;
		short shortType;
		double doubleType;
		float floatType;
		bool boolType;
		char charType;
		long longType;
		long long long_lType;
	};
	enum valueType { NONE,INT,UINT,SHORT,DOUBLE,FLOAT,BOOL,CHAR,LONG,LONG_LONG};
	uStorage unType;
	valueType currentType;
public:

	AnyType() : currentType(NONE) {}

	template <typename T>
	AnyType(const T& value)
	{
		changeType(value);
	}
	AnyType(const AnyType& otherData)
	{
		unType = otherData.unType;
		currentType = otherData.currentType;
	}
	AnyType(AnyType&& otherData) noexcept
	{
		unType = otherData.unType;
		currentType = otherData.currentType;
		otherData.clearType();
	}

	template <typename T>
	AnyType& operator=(const T& value)
	{
		changeType(value);
		return *this;
	}
	AnyType& operator=(const AnyType& otherData)
	{
		if (this != &otherData)
		{
			unType = otherData.unType;
			currentType = otherData.currentType;
		}
		return *this;
	}
	AnyType& operator=(AnyType&& otherData) noexcept
	{
		if (this != &otherData)
		{
			unType = otherData.unType;
			currentType = otherData.currentType;
			otherData.clearType();
		}
		return *this;
	}
	
	template <typename T>
	T getType()
	{
		if constexpr (std::is_same<T, int>::value){
			if (currentType == INT)
				return unType.intType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, unsigned int>::value){
			if (currentType == UINT)
				return unType.uiType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, short>::value){
			if(currentType == SHORT) 
				return unType.shortType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, double>::value) {
			if (currentType == DOUBLE)
				return unType.doubleType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, float>::value) {
			if (currentType == FLOAT)
				return unType.floatType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, bool>::value){
			if(currentType == BOOL)
				return unType.boolType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, char>::value){
			if(currentType == CHAR)
				return unType.charType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, long>::value){
			if(currentType == LONG)
				return unType.longType;
			throw std::bad_cast();
		}
		else if constexpr (std::is_same<T, long long>::value){
			if(currentType == LONG_LONG)
				return unType.long_lType;
			throw std::bad_cast();
		}
		else
			throw std::bad_cast();
	}

	// all toType methods are getters, with simpliest user use
	int toInt() {
		return getType<int>();
	}
	unsigned int toUInt()
	{
		return getType<unsigned int>();
	}
	short toShort() {
		return getType<short>();
	}
	double toDouble() {
		return getType<double>();
	}
	float toFloat() {
		return getType<float>();
	}
	bool toBool() {
		return getType<bool>();
	}
	char toChar() {
		return getType<char>();
	}
	long toLong() {
		return getType<long>();
	}
	long long toLongLong() {
		return getType<long long>();
	}
	
	//change unionType & enumType
	template <typename T>
	void changeType(T newValue)
	{
		clearType();

		if constexpr (std::is_same<T, int>::value) {
			currentType = INT;
			unType.intType = newValue;
		}
		else if constexpr (std::is_same <T, unsigned int>::value) {
			currentType = UINT;
			unType.uiType = newValue;
		}
		else if constexpr (std::is_same<T, short>::value) {
			currentType = SHORT;
			unType.shortType = newValue;
		}
		else if constexpr (std::is_same <T, double>::value) {
			currentType = DOUBLE;
			unType.doubleType = newValue;
		}
		else if constexpr (std::is_same <T, float>::value) {
			currentType = FLOAT;
			unType.floatType = newValue;
		}
		else if constexpr (std::is_same <T, bool>::value) {
			currentType = BOOL;
			unType.boolType = newValue;
		}
		else if constexpr (std::is_same <T, char>::value) {
			currentType = CHAR;
			unType.charType = newValue;
		}
		else if constexpr (std::is_same <T, long>::value) {
			currentType = LONG;
			unType.longType = newValue;
		}
		else if constexpr (std::is_same <T, long long>::value) {
			currentType = LONG_LONG;
			unType.long_lType = newValue;
		}
		else
			throw std::invalid_argument("invalid type");
	}

	void clearType()
	{
		currentType = NONE;
	};

	void swap(AnyType& otherType)
	{
		if (this == &otherType) return;
		std::swap(unType, otherType.unType);
		std::swap(currentType, otherType.currentType);
	};
	bool hasValue()
	{
		return currentType != NONE;
	}

	~AnyType() { clearType(); }
		
};



