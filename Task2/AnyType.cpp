#include "AnyType.h"
#include <algorithm>

const char* BadAnyTypeCast::what() const noexcept
{
	return "Bad AnyType cast";
}

AnyType::AnyType() = default;

AnyType::AnyType(bool v) { store(v); }
AnyType::AnyType(char v) { store(v); }
AnyType::AnyType(int v) { store(v); }
AnyType::AnyType(float v) { store(v); }
AnyType::AnyType(double v) { store(v); }
AnyType::AnyType(long v) { store(v); }

AnyType::AnyType(const AnyType& other) {
	copyFrom(other);
}

AnyType::AnyType(AnyType&& other) noexcept
{
	moveFrom(std::move(other));
}

AnyType::~AnyType() {
	Reset();
}

AnyType& AnyType::operator=(const AnyType& other) {
	if (this != &other) {
		Reset();
		copyFrom(other);
	}
	return *this;
}

AnyType& AnyType::operator=(AnyType&& other) noexcept {
	if (this != &other) {
		Reset();
		moveFrom(std::move(other));
	}
	return *this;
}

AnyType& AnyType::operator=(bool v) { Reset(); store(v); return *this; }
AnyType& AnyType::operator=(char v) { Reset(); store(v); return *this; }
AnyType& AnyType::operator=(int v) { Reset(); store(v); return *this; }
AnyType& AnyType::operator=(float v) { Reset(); store(v); return *this; }
AnyType& AnyType::operator=(double v) { Reset(); store(v); return *this; }
AnyType& AnyType::operator=(long v) { Reset(); store(v); return *this; }

void AnyType::Reset() {
	type = Type::None;
}

void AnyType::Swap(AnyType& other) noexcept {
	std::swap(storage, other.storage);
	std::swap(type, other.type);
}

AnyType::Type AnyType::GetType() const noexcept {
	return type;
}

bool AnyType::ToBool() const
{
	if (type != Type::Bool) throw BadAnyTypeCast();
	return storage.b;
}

char AnyType::ToChar() const
{
	if (type != Type::Char) throw BadAnyTypeCast();
	return storage.c;
}

int AnyType::ToInt() const
{
	if (type != Type::Int) throw BadAnyTypeCast();
	return storage.i;
}

float AnyType::ToFloat() const
{
	if (type != Type::Float) throw BadAnyTypeCast();
	return storage.f;
}

double AnyType::ToDouble() const
{
	if (type != Type::Double) throw BadAnyTypeCast();
	return storage.d;
}

long AnyType::ToLong() const
{
	if (type != Type::Long) throw BadAnyTypeCast();
	return storage.l;
}

void AnyType::copyFrom(const AnyType& other) {
	type = other.type;
	storage = other.storage;
}

void AnyType::moveFrom(AnyType&& other) {
	type = other.type;
	storage = other.storage;
	other.type = Type::None;
}

void AnyType::store(bool v) { storage.b = v; type = Type::Bool; }
void AnyType::store(char v) { storage.c = v; type = Type::Char; }
void AnyType::store(int v) { storage.i = v; type = Type::Int; }
void AnyType::store(float v) { storage.f = v; type = Type::Float; }
void AnyType::store(double v) { storage.d = v; type = Type::Double; }
void AnyType::store(long v) { storage.l = v; type = Type::Long; }