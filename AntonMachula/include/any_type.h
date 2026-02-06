#ifndef ANY_TYPE_H
#define ANY_TYPE_H

#include <stdexcept>
#include <typeinfo>
#include <utility>

class BadAnyCast : public std::runtime_error {
public:
    BadAnyCast() : std::runtime_error("bad any cast") {}
};

class AnyType {
    struct Holder {
        virtual Holder* clone() const = 0;
        virtual ~Holder() = 0;
    };

    template <typename T>
    struct HolderImpl: Holder {
       T value;
       HolderImpl(const T& value): value(value) {}
       Holder* clone() const override {
           return new HolderImpl(value);
       }
       ~HolderImpl() = default;
    };

    Holder* holder_ = nullptr;

public:
    AnyType() = default;

    template <typename T>
    AnyType(const T& value) : holder_(new HolderImpl<T>(value)) {}

    AnyType(const AnyType& other)
        : holder_(other.holder_ ? other.holder_->clone() : nullptr) {}

    AnyType(AnyType&& other) noexcept
        : holder_(std::exchange(other.holder_, nullptr)) {}

    ~AnyType() { delete holder_; }

    AnyType& operator=(const AnyType& other) {
        if (this != &other) {
            AnyType tmp(other);
            swap(tmp);
        }
        return *this;
    }

    AnyType& operator=(AnyType&& other) noexcept {
        if (this != &other) {
            delete holder_;
            holder_ = std::exchange(other.holder_, nullptr);
        }
        return *this;
    }

    template <typename T>
    AnyType& operator=(T value) {
        AnyType tmp(std::move(value));
        swap(tmp);
        return *this;
    }

    void reset() {
        delete holder_;
        holder_ = nullptr;
    }

    void swap(AnyType& other) noexcept {
        std::swap(holder_, other.holder_);
    }

    bool hasValue() const { return holder_ != nullptr; }

    template <typename T>
    T get() const {
        auto* p = dynamic_cast<HolderImpl<T>*>(holder_);
        if (!p)
            throw BadAnyCast();
        return p->value;
    }
};

inline AnyType::Holder::~Holder() = default;

#endif
