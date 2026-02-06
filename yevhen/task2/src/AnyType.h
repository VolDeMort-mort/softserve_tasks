#ifndef ANYTYPE_H
#define ANYTYPE_H

#include <mutex>
#include <string>

enum class Type
{
    ch,
    u_ch,
    sh,
    u_sh,
    i,
    u_i,
    l,
    u_l,
    ll,
    u_ll,
    f,
    d,
    ld,
    b,
    invalid
};
std::string to_string(const Type &type);

class AnyType
{
private:
    union U
    {
        char ch;
        unsigned char u_ch;
        short sh;
        unsigned short u_sh;
        int i;
        unsigned int u_i;
        long l;
        unsigned long u_l;
        long long ll;
        unsigned long long u_ll;
        float f;
        double d;
        long double ld;
        bool b;
    } data_;
    Type type_;
    mutable std::mutex mutex_;

public:
    AnyType();
    AnyType(char p);
    AnyType(unsigned char p);
    AnyType(short p);
    AnyType(unsigned short p);
    AnyType(int p);
    AnyType(unsigned int p);
    AnyType(long p);
    AnyType(unsigned long p);
    AnyType(long long p);
    AnyType(unsigned long long p);
    AnyType(float p);
    AnyType(double p);
    AnyType(long double p);
    AnyType(bool p);

    AnyType(const AnyType &other);
    AnyType &operator=(const AnyType &other);

    AnyType &operator=(const char p);
    AnyType &operator=(const unsigned char p);
    AnyType &operator=(const short p);
    AnyType &operator=(const unsigned short p);
    AnyType &operator=(const int p);
    AnyType &operator=(const unsigned int p);
    AnyType &operator=(const long p);
    AnyType &operator=(const unsigned long p);
    AnyType &operator=(const long long p);
    AnyType &operator=(const unsigned long long p);
    AnyType &operator=(const float p);
    AnyType &operator=(const double p);
    AnyType &operator=(const long double p);
    AnyType &operator=(const bool p);

    AnyType &reset();

    friend void swap(AnyType &first, AnyType &second);

    Type getType() const;

    char ToChar() const;
    unsigned char ToUChar() const;
    short ToShort() const;
    unsigned short ToUShort() const;
    int ToInt() const;
    unsigned int ToUInt() const;
    long ToLong() const;
    unsigned long ToULong() const;
    long long ToLongLong() const;
    unsigned long long ToULongLong() const;
    float ToFloat() const;
    double ToDouble() const;
    long double ToLongDouble() const;
    bool ToBool() const;
};

#endif // ANYTYPE_H