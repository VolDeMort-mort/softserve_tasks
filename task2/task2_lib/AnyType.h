#pragma once

#include <utility>
#include <exception>
#include <string>
#include <string_view>
#include <array>

class BadAnyTypeCase;

class AnyType
{
public:
    enum class Type {
        Empty,
        Char,
        UnsignedChar,
        SignedChar,
        Int,
        ShortInt,
        LongInt,
        LongLongInt,
        UnsignedInt,
        UnsignedShortInt,
        UnsignedLongInt,
        UnsignedLongLongInt,
        Float,
        Double,
        LongDouble,
    };
private:
    static const std::array<std::string, 15> TypeNames;

    union Storage {
        char c;
        unsigned char uc;
        signed char sc;
        int i;
        short int si;
        long int li;
        long long int lli;
        unsigned int ui;
        unsigned short int usi;
        unsigned long int uli;
        unsigned long long int ulli;
        float f;
        double d;
        long double ld;
    } m_storage;

    Type m_type;

    template<typename R, R Storage::* Member>
    R getValue(Type expected) const {
        if (m_type != expected) throw BadAnyTypeCase(expected, m_type);
        return m_storage.*Member;
    }


public:
    AnyType() noexcept : m_type{Type::Empty} {}

    AnyType(char v) :                            m_type{ Type::Char } { m_storage.c = v; };
    AnyType(unsigned char v) noexcept :          m_type{ Type::UnsignedChar } { m_storage.uc = v; }
    AnyType(signed char v) noexcept :            m_type{ Type::SignedChar } { m_storage.sc = v; }
    AnyType(int v) noexcept :                    m_type{ Type::Int } { m_storage.i = v; }
    AnyType(short int v) noexcept :              m_type{ Type::ShortInt } { m_storage.si = v; }
    AnyType(long int v) noexcept :               m_type{ Type::LongInt } { m_storage.li = v; }
    AnyType(long long int v) noexcept :          m_type{ Type::LongLongInt } { m_storage.lli = v; }
    AnyType(unsigned int v) noexcept :           m_type{ Type::UnsignedInt } { m_storage.ui = v; }
    AnyType(unsigned short int v) noexcept :     m_type{ Type::UnsignedShortInt } { m_storage.usi = v; }
    AnyType(unsigned long int v) noexcept :      m_type{ Type::UnsignedLongInt } { m_storage.uli = v; }
    AnyType(unsigned long long int v) noexcept : m_type{ Type::UnsignedLongLongInt } { m_storage.ulli = v; }
    AnyType(float v) noexcept :                  m_type{ Type::Float } { m_storage.f = v; }
    AnyType(double v) noexcept :                 m_type{ Type::Double } { m_storage.d = v; }
    AnyType(long double v) noexcept :            m_type{ Type::LongDouble } { m_storage.ld = v; }

    //Copy
    AnyType(const AnyType& other) noexcept 
        : m_type{ other.m_type }, m_storage{ other.m_storage } 
    {};

    AnyType& operator= (const AnyType& other) noexcept {
        if (this == &other) return *this;
        m_type = other.m_type;
        m_storage = other.m_storage;
        return *this;
    }

    //Move
    AnyType(AnyType&& other) noexcept 
        : m_type{other.m_type}, m_storage{ std::move(other.m_storage) } {
        other.m_type = Type::Empty;
    }

    AnyType& operator= (AnyType&& other) noexcept {
        if (this == &other) return *this;
        m_type = other.m_type;
        other.m_type = Type::Empty;

        m_storage = std::move(other.m_storage);
        return *this;
    }

    void destroy() {
        m_type = Type::Empty;
    }

    ~AnyType() {
        this->destroy();
    }

    void swap(AnyType& other) noexcept {
        std::swap(m_storage, other.m_storage);
        std::swap(m_type, other.m_type);
    }

    Type getType() const noexcept { 
        return m_type; 
    }

    static std::string getTypeName(Type type) {
        return TypeNames[static_cast<std::size_t>(type)];
    }

    AnyType& operator=(char v) noexcept;
    AnyType& operator=(unsigned char v) noexcept;
    AnyType& operator=(signed char v) noexcept;
    AnyType& operator=(int v) noexcept;
    AnyType& operator=(short int v) noexcept;
    AnyType& operator=(long int v) noexcept;
    AnyType& operator=(long long int v) noexcept;
    AnyType& operator=(unsigned int v) noexcept;
    AnyType& operator=(unsigned short int v) noexcept;
    AnyType& operator=(unsigned long int v) noexcept;
    AnyType& operator=(unsigned long long int v) noexcept;
    AnyType& operator=(float v) noexcept;
    AnyType& operator=(double v) noexcept;
    AnyType& operator=(long double v) noexcept;


    char getChar() const;
    unsigned char getUnsignedChar() const;
    signed char getSignedChar() const;
    int getInt() const;
    short int getShortInt() const;
    long int getLongInt() const;
    long long int getLongLongInt() const;
    unsigned int getUnsignedInt() const;
    unsigned short int getUnsignedShortInt() const;
    unsigned long int getUnsignedLongInt() const;
    unsigned long long int getUnsignedLongLongInt() const;
    float getFloat() const;
    double getDouble() const;
    long double getLongDouble() const;
};

class BadAnyTypeCase : public std::exception {
private:
    std::string message;
public:
    BadAnyTypeCase(AnyType::Type expected, AnyType::Type actual) 
        :message("Bad AnyType access: expected " + AnyType::getTypeName(expected) +
            ", got " + AnyType::getTypeName(actual)) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

};