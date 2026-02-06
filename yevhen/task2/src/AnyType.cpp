#include "AnyType.h"
#include "BadAnyCast.h"
#include <bits/stl_pair.h>

std::string to_string(const Type &type) {
    switch (type) {
        case Type::ch: return "char";
        case Type::u_ch: return "unsigned char";
        case Type::sh: return "short";
        case Type::u_sh: return "unsigned short";
        case Type::i: return "int";
        case Type::u_i: return "unsigned int";
        case Type::l: return "long";
        case Type::u_l: return "unsigned long";
        case Type::ll: return "long long";
        case Type::u_ll: return "unsigned long long";
        case Type::f: return "float";
        case Type::d: return "double";
        case Type::ld: return "long double";
        case Type::b: return "bool";
        case Type::invalid: return "invalid";
        default: return "unknown";
    }
}


AnyType::AnyType()
{
    type_ = Type::invalid;
}

AnyType::AnyType(char p)
{
    data_.ch = p;
    type_ = Type::ch;
}

AnyType::AnyType(unsigned char p)
{
    data_.u_ch = p;
    type_ = Type::u_ch;
}

AnyType::AnyType(short p)
{
    data_.sh = p;
    type_ = Type::sh;
}

AnyType::AnyType(unsigned short p)
{
    data_.u_sh = p;
    type_ = Type::u_sh;
}

AnyType::AnyType(int p)
{
    data_.i = p;
    type_ = Type::i;
}

AnyType::AnyType(unsigned int p)
{
    data_.u_i = p;
    type_ = Type::u_i;
}

AnyType::AnyType(long p)
{
    data_.l = p;
    type_ = Type::l;
}

AnyType::AnyType(unsigned long p)
{
    data_.u_l = p;
    type_ = Type::u_l;
}

AnyType::AnyType(long long p)
{
    data_.ll = p;
    type_ = Type::ll;
}

AnyType::AnyType(unsigned long long p)
{
    data_.u_ll = p;
    type_ = Type::u_ll;
}

AnyType::AnyType(float p)
{
    data_.f = p;
    type_ = Type::f;
}

AnyType::AnyType(double p)
{
    data_.d = p;
    type_ = Type::d;
}

AnyType::AnyType(long double p)
{
    data_.ld = p;
    type_ = Type::ld;
}

AnyType::AnyType(bool p)
{
    data_.b = p;
    type_ = Type::b;
}

AnyType::AnyType(const AnyType &other)
{
    std::lock_guard lock(other.mutex_);
    type_ = other.type_;
    data_ = other.data_;
}


AnyType &AnyType::operator=(const AnyType &other)
{
    if (this == &other) return *this;
    std::scoped_lock lock(mutex_, other.mutex_);
    type_ = other.type_;
    data_ = other.data_;
    return *this;
}

AnyType &AnyType::operator=(const char p)
{
    std::lock_guard lock(mutex_);
    data_.ch = p;
    type_ = Type::ch;
    return *this;
}

AnyType &AnyType::operator=(const unsigned char p)
{
    std::lock_guard lock(mutex_);
    data_.u_ch = p;
    type_ = Type::u_ch;
    return *this;
}

AnyType &AnyType::operator=(const short p)
{
    std::lock_guard lock(mutex_);
    data_.sh = p;
    type_ = Type::sh;
    return *this;
}

AnyType &AnyType::operator=(const unsigned short p)
{
    std::lock_guard lock(mutex_);
    data_.u_sh = p;
    type_ = Type::u_sh;
    return *this;
}

AnyType &AnyType::operator=(const int p)
{
    std::lock_guard lock(mutex_);
    data_.i = p;
    type_ = Type::i;
    return *this;
}

AnyType &AnyType::operator=(const unsigned int p)
{
    std::lock_guard lock(mutex_);
    data_.u_i = p;
    type_ = Type::u_i;
    return *this;
}

AnyType &AnyType::operator=(const long p)
{
    std::lock_guard lock(mutex_);
    data_.l = p;
    type_ = Type::l;
    return *this;
}

AnyType &AnyType::operator=(const unsigned long p)
{
    std::lock_guard lock(mutex_);
    data_.u_l = p;
    type_ = Type::u_l;
    return *this;
}

AnyType &AnyType::operator=(const long long p)
{
    std::lock_guard lock(mutex_);
    data_.ll = p;
    type_ = Type::ll;
    return *this;
}

AnyType &AnyType::operator=(const unsigned long long p)
{
    std::lock_guard lock(mutex_);
    data_.u_ll = p;
    type_ = Type::u_ll;
    return *this;
}

AnyType &AnyType::operator=(const float p)
{
    std::lock_guard lock(mutex_);
    data_.f = p;
    type_ = Type::f;
    return *this;
}

AnyType &AnyType::operator=(const double p)
{
    std::lock_guard lock(mutex_);
    data_.d = p;
    type_ = Type::d;
    return *this;
}

AnyType &AnyType::operator=(const long double p)
{
    std::lock_guard lock(mutex_);
    data_.ld = p;
    type_ = Type::ld;
    return *this;
}

AnyType &AnyType::operator=(const bool p)
{
    std::lock_guard lock(mutex_);
    data_.b = p;
    type_ = Type::b;
    return *this;
}


AnyType &AnyType::reset()
{
    std::lock_guard lock(mutex_);
    type_ = Type::invalid;
    return *this;
}

void swap(AnyType &first, AnyType &second)
{
    if (&first == &second) return;
    std::scoped_lock lock(first.mutex_, second.mutex_);
    std::swap(first.type_, second.type_);
    std::swap(first.data_, second.data_);
}

Type AnyType::getType() const
{
    std::lock_guard lock(mutex_);
    return type_;
}


char AnyType::ToChar() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::ch)
    {
        throw BadAnyCast("Invalid cast to char from type " + to_string(type_));
    }
    return data_.ch;
}

unsigned char AnyType::ToUChar() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::u_ch)
    {
        throw BadAnyCast("Invalid cast to unsigned char from type " + to_string(type_));
    }
    return data_.u_ch;
}

short AnyType::ToShort() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::sh)
    {
        throw BadAnyCast("Invalid cast to short from type " + to_string(type_));
    }
    return data_.sh;
}

unsigned short AnyType::ToUShort() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::u_sh)
    {
        throw BadAnyCast("Invalid cast to unsigned short from type " + to_string(type_));
    }
    return data_.u_sh;
}

int AnyType::ToInt() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::i)
    {
        throw BadAnyCast("Invalid cast to int from type " + to_string(type_));
    }
    return data_.i;
}

unsigned int AnyType::ToUInt() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::u_i)
    {
        throw BadAnyCast("Invalid cast to unsigned int from type " + to_string(type_));
    }
    return data_.u_i;
}

long AnyType::ToLong() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::l)
    {
        throw BadAnyCast("Invalid cast to long from type " + to_string(type_));
    }
    return data_.l;
}

unsigned long AnyType::ToULong() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::u_l)
    {
        throw BadAnyCast("Invalid cast to unsigned long from type " + to_string(type_));
    }
    return data_.u_l;
}

long long AnyType::ToLongLong() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::ll)
    {
        throw BadAnyCast("Invalid cast to long long from type " + to_string(type_));
    }
    return data_.ll;
}

unsigned long long AnyType::ToULongLong() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::u_ll)
    {
        throw BadAnyCast("Invalid cast to unsigned long long from type " + to_string(type_));
    }
    return data_.u_ll;
}

float AnyType::ToFloat() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::f)
    {
        throw BadAnyCast("Invalid cast to float from type " + to_string(type_));
    }
    return data_.f;
}

double AnyType::ToDouble() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::d)
    {
        throw BadAnyCast("Invalid cast to double from type " + to_string(type_));
    }
    return data_.d;
}

long double AnyType::ToLongDouble() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::ld)
    {
        throw BadAnyCast("Invalid cast to long double from type " + to_string(type_));
    }
    return data_.ld;
}

bool AnyType::ToBool() const
{
    std::lock_guard lock(mutex_);
    if(type_ != Type::b)
    {
        throw BadAnyCast("Invalid cast to bool from type " + to_string(type_));
    }
    return data_.b;
}