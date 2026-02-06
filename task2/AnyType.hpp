#ifndef ANYTYPE_HPP
#define ANYTYPE_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

class BadCastException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Bad cast: requested type does not match stored type.";
    }
};

class AnyType {
public:
    enum class Type {
        None,
        Int,
        Double,
        Bool,
        Char,
        Float,
        Long
    };

private:
    union Storage {
        int i;
        double d;
        bool b;
        char c;
        float f;
        long l;
    };

    Storage storage_;
    Type currentType_;

    template <typename T>
    void setValue(const T& value) {
        if constexpr (std::is_same_v<T, int>) {
            storage_.i = value;
            currentType_ = Type::Int;
        } else if constexpr (std::is_same_v<T, double>) {
            storage_.d = value;
            currentType_ = Type::Double;
        } else if constexpr (std::is_same_v<T, bool>) {
            storage_.b = value;
            currentType_ = Type::Bool;
        } else if constexpr (std::is_same_v<T, char>) {
            storage_.c = value;
            currentType_ = Type::Char;
        } else if constexpr (std::is_same_v<T, float>) {
            storage_.f = value;
            currentType_ = Type::Float;
        } else if constexpr (std::is_same_v<T, long>) {
            storage_.l = value;
            currentType_ = Type::Long;
        }
    }


public:
    AnyType() : currentType_(Type::None) {}

    template <typename T>
    AnyType(const T& value) {
        setValue(value);
    }

    AnyType(const AnyType& other) : storage_(other.storage_), currentType_(other.currentType_) {}

    AnyType(AnyType&& other) noexcept : storage_(other.storage_), currentType_(other.currentType_) {
        other.currentType_ = Type::None;
    }

    ~AnyType() {
        reset();
    }

    AnyType& operator=(const AnyType& other) {
        if (this != &other) {
            storage_ = other.storage_;
            currentType_ = other.currentType_;
        }
        return *this;
    }

    AnyType& operator=(AnyType&& other) noexcept {
        if (this != &other) {
            storage_ = other.storage_;
            currentType_ = other.currentType_;
            other.currentType_ = Type::None;
        }
        return *this;
    }

    template <typename T>
    AnyType& operator=(const T& value) {
        setValue(value);
        return *this;
    }

    void reset() {
        currentType_ = Type::None;
    }

    void swap(AnyType& other) {
        std::swap(storage_, other.storage_);
        std::swap(currentType_, other.currentType_);
    }

    Type getType() const {
        return currentType_;
    }

    template <typename T>
    T getValue() const {
        if constexpr (std::is_same_v<T, int>) {
            if (currentType_ != Type::Int) throw BadCastException();
            return storage_.i;
        } else if constexpr (std::is_same_v<T, double>) {
            if (currentType_ != Type::Double) throw BadCastException();
            return storage_.d;
        } else if constexpr (std::is_same_v<T, bool>) {
            if (currentType_ != Type::Bool) throw BadCastException();
            return storage_.b;
        } else if constexpr (std::is_same_v<T, char>) {
            if (currentType_ != Type::Char) throw BadCastException();
            return storage_.c;
        } else if constexpr (std::is_same_v<T, float>) {
            if (currentType_ != Type::Float) throw BadCastException();
            return storage_.f;
        } else if constexpr (std::is_same_v<T, long>) {
            if (currentType_ != Type::Long) throw BadCastException();
            return storage_.l;
        }
        throw BadCastException();
    }
};

#endif // ANYTYPE_HPP
