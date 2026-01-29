#include "BadAnyCast.h"

BadAnyCast::BadAnyCast(const std::string& msg) : msg_(msg) {}

const char* BadAnyCast::what() const noexcept
{
    return msg_.c_str();
}