#include <type_traits>
#include <stdexcept>
#include <utility>
#include <new>

class BadAnyCast : public std::exception {
public:
    const char* what() const noexcept override {
        return "Bad AnyType cast";
    }
};

class AnyType {
private:
    template<typename... Ts>
    struct TypeList {};

    using Supported = TypeList<
        int, unsigned int,
        short, unsigned short,
        char, signed char, unsigned char,
        long, unsigned long,
        long long, unsigned long long,
        float, double, long double,
        bool,
        wchar_t, char16_t, char32_t
    >;

    template<typename T, typename... Rest>
    struct MaxSizeAlign {
        static constexpr size_t size =
            sizeof(T) > MaxSizeAlign<Rest...>::size ? sizeof(T) : MaxSizeAlign<Rest...>::size;
        static constexpr size_t align =
            alignof(T) > MaxSizeAlign<Rest...>::align ? alignof(T) : MaxSizeAlign<Rest...>::align;
    };

    template<typename T>
    struct MaxSizeAlign<T> {
        static constexpr size_t size = sizeof(T);
        static constexpr size_t align = alignof(T);
    };

    using StorageInfo = MaxSizeAlign<
        int, unsigned int,
        short, unsigned short,
        char, signed char, unsigned char,
        long, unsigned long,
        long long, unsigned long long,
        float, double, long double,
        bool,
        wchar_t, char16_t, char32_t
    >;

    using Storage = std::aligned_storage_t<StorageInfo::size, StorageInfo::align>;
    Storage storage_;

	struct VTable {
		void (*copy)(const void*, void*);
		void (*move)(void*, void*);
		void (*destroy)(void*);
		const std::type_info& (*type)();
	};

    const VTable* vtable_ = nullptr;

    template<typename T>
    static void copy_fn(const void* src, void* dst) {
        new(dst) T(*reinterpret_cast<const T*>(src));
    }

    template<typename T>
    static void move_fn(void* src, void* dst) {
        new(dst) T(std::move(*reinterpret_cast<T*>(src)));
        destroy_fn<T>(src);
    }

    template<typename T>
    static void destroy_fn(void* ptr) {
        reinterpret_cast<T*>(ptr)->~T();
    }

    template<typename T>
    static const std::type_info& type_fn() {
        return typeid(T);
    }

    template<typename T>
    static const VTable vtable_for;

public:
    AnyType() noexcept = default;

    ~AnyType() {
        destroy();
    }

	template<typename T,
		typename = std::enable_if_t<
			std::is_fundamental_v<T> &&
			!std::is_same_v<T, void> &&
			!std::is_same_v<T, std::nullptr_t>>>
			AnyType(T value) {
				emplace<T>(value);
			}

	AnyType(const AnyType& other) {
		if(other.vtable_) {
			other.vtable_->copy(&other.storage_, &storage_);
			vtable_ = other.vtable_;
		}
	}

	AnyType(AnyType&& other) noexcept {
		if(other.vtable_) {
			other.vtable_->move(&other.storage_, &storage_);
			vtable_ = other.vtable_;
			other.vtable_ = nullptr;
		}
	}

	AnyType& operator=(const AnyType& other) {
		if(this != &other) {
			destroy();
			if(other.vtable_) {
				other.vtable_->copy(&other.storage_, &storage_);
				vtable_ = other.vtable_;
			}
		}
		return *this;
	}

    AnyType& operator=(AnyType&& other) noexcept {
        if(this != &other) {
            destroy();
            if(other.vtable_) {
                other.vtable_->move(&other.storage_, &storage_);
                vtable_ = other.vtable_;
                other.vtable_ = nullptr;
            }
        }
        return *this;
    }

    template<typename T>
    AnyType& operator=(T value) {
        destroy();
        emplace<T>(value);
        return *this;
    }

    void destroy() {
        if(vtable_) {
            vtable_->destroy(&storage_);
            vtable_ = nullptr;
        }
    }

    void swap(AnyType& other) noexcept {
        AnyType tmp(std::move(other));
        other = std::move(*this);
        *this = std::move(tmp);
    }

    bool has_value() const noexcept {
        return vtable_ != nullptr;
    }

    const std::type_info& type() const {
        if(!vtable_) throw std::logic_error("Empty AnyType");
        return vtable_->type();
    }

    template<typename T>
    T get() const {
        if(!vtable_ || vtable_->type() != typeid(T))
            throw BadAnyCast();
        return *reinterpret_cast<const T*>(&storage_);
    }

private:
    template<typename T>
    void emplace(T value) {
        new(&storage_) T(value);
        vtable_ = &vtable_for<T>;
    }
};

template<typename T>
const AnyType::VTable AnyType::vtable_for = {
    &AnyType::copy_fn<T>,
    &AnyType::move_fn<T>,
    &AnyType::destroy_fn<T>,
    &AnyType::type_fn<T>
};

