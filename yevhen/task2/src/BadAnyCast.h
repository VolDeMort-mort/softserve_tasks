#ifndef BADANYCAST_H
#define BADANYCAST_H

#include <exception>
#include <string>

class BadAnyCast : public std::exception {
private:
    std::string msg_;
public:
    explicit BadAnyCast(const std::string& msg);
    const char* what() const noexcept override;
};

#endif // BADANYCAST_H