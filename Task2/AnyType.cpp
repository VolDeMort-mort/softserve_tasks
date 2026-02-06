#include "AnyType.h"
#include <algorithm>


AnyType::Data::Data() : d(0.0),  {}


AnyType::AnyType() : typeTag(TypeTag::Empty) {}
AnyType::AnyType(int val) : typeTag(TypeTag::Int) { data.i = val; }
AnyType::AnyType(double val) : typeTag(TypeTag::Double) { data.d = val; }
AnyType::AnyType(bool val) : typeTag(TypeTag::Bool) { data.b = val; }
AnyType::AnyType(char val) : typeTag(TypeTag::Char) { data.c = val; }

AnyType::AnyType(const AnyType& other) : typeTag(other.typeTag), data(other.data) {}

AnyType::AnyType(AnyType&& other) noexcept : AnyType() {
     Swap(other);
}


TypeTag AnyType::GetType() const {
     return typeTag;
}


AnyType& AnyType::operator=(int val) {
     typeTag = TypeTag::Int;
     data.i = val;
     return *this;
}

AnyType& AnyType::operator=(double val) {
     typeTag = TypeTag::Double;
     data.d = val;
     return *this;
}

AnyType& AnyType::operator=(bool val) {
     typeTag = TypeTag::Bool;
     data.b = val;
     return *this;
}
AnyType& AnyType::operator=(char val) {
     typeTag = TypeTag::Char;
     data.c = val;
     return *this;
}

AnyType& AnyType::operator=(const AnyType& other) {
     AnyType temp(other);
     Swap(temp);
     return *this;
}

AnyType& AnyType::operator=(AnyType&& other) noexcept {
     Clear();
     Swap(other);
     return *this;
}

// Methods
void AnyType::Clear() {
     typeTag = TypeTag::Empty;
}

void AnyType::Swap(AnyType& other) noexcept {
     std::swap(data, other.data);
     std::swap(typeTag, other.typeTag);
}


int AnyType::ToInt() const {
     if (typeTag != TypeTag::Int) 
          throw std::runtime_error("Bad AnyType Cast: Not an Int");
     return data.i;
}

double AnyType::ToDouble() const {
     if (typeTag != TypeTag::Double) 
          throw std::runtime_error("Bad AnyType Cast: Not a Double");
     return data.d;
}

bool AnyType::ToBool() const {
     if (typeTag != TypeTag::Bool) 
          throw std::runtime_error("Bad AnyType Cast: Not a Bool");
     return data.b;
}

char AnyType::ToChar() const {
     if (typeTag != TypeTag::Char) 
          throw std::runtime_error("Bad AnyType Cast: Not a Char");
     return data.c;
}