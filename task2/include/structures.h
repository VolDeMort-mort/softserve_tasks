#include <algorithm>
#include <type_traits>
#include <stdexcept>
#include <utility>

class BadAnyCast : public std::runtime_error
{
public:
	BadAnyCast() : std::runtime_error("Bad cast") {};
};

class AnyType
{
public:
	AnyType() = default;

	template<typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, AnyType>>>
	AnyType(T&& arg)
	{
		emplace<T>(std::forward<T>(arg));
	}

	~AnyType()
	{
		reset();
	}

	AnyType(const AnyType& other)
	{
		if (!other.ptr_controller)
			return;

		arguments arg;
		arg.ptr_any = this;
		other.ptr_controller(operation::COPY, &other, &arg);

	}

	AnyType(AnyType&& other) noexcept
	{
		if (!other.ptr_controller)
			return;

		arguments arg;
		arg.ptr_any = this;
		other.ptr_controller(operation::MOVE, &other, &arg);
		other.ptr_controller = nullptr;
	}

	AnyType& operator=(const AnyType& other)
	{
		if (this == &other)
			return *this;

		reset();

		if (other.ptr_controller)
		{
			arguments arg;
			arg.ptr_any = this;
			other.ptr_controller(operation::COPY, &other, &arg);
		}
		return *this;
	}

	AnyType& operator=(AnyType&& other) noexcept
	{
		if (this == &other) return *this;

		reset();

		if (other.ptr_controller)
		{
			arguments arg;
			arg.ptr_any = this;
			other.ptr_controller(operation::MOVE, &other, &arg);
			other.ptr_controller = nullptr;
		}

		return *this;
	}

	void reset()
	{
		if (!ptr_controller)
			return;

		ptr_controller(operation::DESTROY, this, nullptr);
		ptr_controller = nullptr;
	}

	template<typename T>
	auto& emplace(T&& arg)
	{
		static_assert(!std::is_void_v<T>, "void is not a valid AnyType value");
		static_assert(!std::is_null_pointer_v<T>, "null_pointer is not a valid AnyType value");
		static_assert(std::is_fundamental_v<std::decay_t<T>>, "only fundamental types allowed");

		reset();
	
		new(buffer)T(std::forward<T>(arg));
		ptr_controller = &Controller<T>::control;
		return *reinterpret_cast<T*>(&buffer);	
	}

	bool has_value() const noexcept
	{
		return ptr_controller != nullptr;
	}

	const type_info& type() const noexcept
	{
		if (!ptr_controller)
			throw std::logic_error("AnyType is empty");


		arguments arg;
		ptr_controller(operation::GET_TYPE, this, &arg);
		return *arg.ptr_type;
	}

	template<typename T>
	const T& cast() const
	{
		if (!ptr_controller || type() != typeid(T))
			throw BadAnyCast();

		arguments arg;
		ptr_controller(operation::ACCESS, this, &arg);
		return *static_cast<const T*>(arg.ptr_access);
	}

	void swap(AnyType& other) noexcept
	{
		if (this == &other) 
			return;

		AnyType tmp = std::move(*this);
		*this = std::move(other);
		other = std::move(tmp);
	}

private:

	alignas(alignof(long double)) unsigned char buffer[sizeof(long double)]{};
	
	enum class operation
	{
		ACCESS,
		GET_TYPE,
		COPY,
		DESTROY,
		MOVE
	};

	union arguments
	{
		void* ptr_access;
		const type_info* ptr_type;
		AnyType* ptr_any;
	};

	using controller = void(*)(operation, const AnyType*, arguments*);
	controller ptr_controller = nullptr;

	template<typename T>
	struct Controller
	{
		static void control(operation op, const AnyType* any, arguments* arg)
		{
			auto ptr_obj = reinterpret_cast<const T*>(&any->buffer);
			switch (op)
			{
			case AnyType::operation::ACCESS:

				arg->ptr_access = const_cast<T*> (ptr_obj); 
				break;

			case AnyType::operation::GET_TYPE:

				arg->ptr_type = &typeid(T);
				break;

			case AnyType::operation::COPY:

				new(&arg->ptr_any->buffer)T(*ptr_obj);
				arg->ptr_any->ptr_controller = any->ptr_controller;
				break;

			case AnyType::operation::DESTROY:

				ptr_obj->~T();
				break;

			case AnyType::operation::MOVE: 

				new(&arg->ptr_any->buffer) T(std::move(*const_cast<T*>(ptr_obj)));
				arg->ptr_any->ptr_controller = any->ptr_controller;
				const_cast<AnyType*>(any)->ptr_controller = nullptr;
				break;

			default:
				break;
			}
		}
	};
};

