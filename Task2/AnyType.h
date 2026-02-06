#pragma once

#include <stdexcept>

enum class TypeTag {
     Empty,
     Int,
     Double,
     Bool,
     Char
};

class AnyType {
private:
     union Data {
          int i;
          double d;
          bool b;
          char c;
          Data();
     } data;

     TypeTag typeTag;

public:
     AnyType();
     AnyType(int val);
     AnyType(double val);
     AnyType(bool val);
     AnyType(char val);
     AnyType(const AnyType& other); 
     AnyType(AnyType&& other) noexcept; 

     AnyType& operator=(int val);
     AnyType& operator=(double val);
     AnyType& operator=(bool val);
     AnyType& operator=(char val);
     AnyType& operator=(const AnyType& other);
     AnyType& operator=(AnyType&& other) noexcept;

     void Clear();
     void Swap(AnyType& other) noexcept; 
     TypeTag GetType() const;

     int ToInt() const;
     double ToDouble() const;
     bool ToBool() const;
     char ToChar() const;
};