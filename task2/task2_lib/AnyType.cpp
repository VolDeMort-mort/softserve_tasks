#include "pch.h"
#include "AnyType.h"
#include <array>
#include <string_view>


const std::array<std::string, 15> AnyType::TypeNames = {
    "Empty",
    "Char",
    "UnsignedChar",
    "SignedChar",
    "Int",
    "ShortInt",
    "LongInt",
    "LongLongInt",
    "UnsignedInt",
    "UnsignedShortInt",
    "UnsignedLongInt",
    "UnsignedLongLongInt",
    "Float",
    "Double",
    "LongDouble"
};

AnyType& AnyType::operator=(char v) noexcept { m_storage.c = v; m_type = Type::Char; return *this; }
AnyType& AnyType::operator=(unsigned char v) noexcept { m_storage.uc = v; m_type = Type::UnsignedChar; return *this; }
AnyType& AnyType::operator=(signed char v) noexcept { m_storage.sc = v; m_type = Type::SignedChar; return *this; }
AnyType& AnyType::operator=(int v) noexcept { m_storage.i = v; m_type = Type::Int; return *this; }
AnyType& AnyType::operator=(short int v) noexcept { m_storage.si = v; m_type = Type::ShortInt; return *this; }
AnyType& AnyType::operator=(long int v) noexcept { m_storage.li = v; m_type = Type::LongInt; return *this; }
AnyType& AnyType::operator=(long long int v) noexcept { m_storage.lli = v; m_type = Type::LongLongInt; return *this; }
AnyType& AnyType::operator=(unsigned int v) noexcept { m_storage.ui = v; m_type = Type::UnsignedInt; return *this; }
AnyType& AnyType::operator=(unsigned short int v) noexcept { m_storage.usi = v; m_type = Type::UnsignedShortInt; return *this; }
AnyType& AnyType::operator=(unsigned long int v) noexcept { m_storage.uli = v; m_type = Type::UnsignedLongInt; return *this; }
AnyType& AnyType::operator=(unsigned long long int v) noexcept { m_storage.ulli = v; m_type = Type::UnsignedLongLongInt; return *this; }
AnyType& AnyType::operator=(float v) noexcept { m_storage.f = v; m_type = Type::Float; return *this; }
AnyType& AnyType::operator=(double v) noexcept { m_storage.d = v; m_type = Type::Double; return *this; }
AnyType& AnyType::operator=(long double v) noexcept { m_storage.ld = v; m_type = Type::LongDouble; return *this; }

char AnyType::getChar() const {
    return getValue<char, &Storage::c>(Type::Char);
}

unsigned char AnyType::getUnsignedChar() const {
    return getValue<unsigned char, &Storage::uc>(Type::UnsignedChar);
}

signed char AnyType::getSignedChar() const {
    return getValue<signed char, &Storage::sc>(Type::SignedChar);
}

int AnyType::getInt() const {
    return getValue<int, &Storage::i>(Type::Int);
}

short int AnyType::getShortInt() const {
    return getValue<short int, &Storage::si>(Type::ShortInt);
}

long int AnyType::getLongInt() const {
    return getValue<long int, &Storage::li>(Type::LongInt);
}

long long int AnyType::getLongLongInt() const {
    return getValue<long long int, &Storage::lli>(Type::LongLongInt);
}

unsigned int AnyType::getUnsignedInt() const {
    return getValue<unsigned int, &Storage::ui>(Type::UnsignedInt);
}

unsigned short int AnyType::getUnsignedShortInt() const {
    return getValue<unsigned short int, &Storage::usi>(Type::UnsignedShortInt);
}

unsigned long int AnyType::getUnsignedLongInt() const {
    return getValue<unsigned long int, &Storage::uli>(Type::UnsignedLongInt);
}

unsigned long long int AnyType::getUnsignedLongLongInt() const {
    return getValue<unsigned long long int, &Storage::ulli>(Type::UnsignedLongLongInt);
}

float AnyType::getFloat() const {
    return getValue<float, &Storage::f>(Type::Float);
}

double AnyType::getDouble() const {
    return getValue<double, &Storage::d>(Type::Double);
}

long double AnyType::getLongDouble() const {
    return getValue<long double, &Storage::ld>(Type::LongDouble);
}

