#include <string>
#include <stdio.h>
#include <iostream> // Required for std::cout
#include <set>
#include "micrograd.h"
#include <cmath> // for std::pow
#include <functional>
#include <vector>

namespace MicroGrad { 
  Value::Value(double value, std::string name, std::string operation)
      : value(value) {
      this->name = name; // just two ways to initialie values
      this->operation = operation;
      grad = 0.0;
  }

  double Value::get_value() const {
    // IF A MEMBER FUNCTION DOESN'T MODIFY VAR, MAKE IT const
    return this->value;
  }

  Value Value::operator+(const Value& other) const {
    std::string combined_str= "";
    if (other.name == "") {
      combined_str = this->name + "+" + std::to_string(other.get_value());
    }
    else {
      combined_str = this->name + "+" + other.name;
    }
    Value out = Value(this->value + other.value, combined_str, "+");
    out._backprop = [this, &other, &out]() {
      const_cast<Value*>(this)->grad += out.grad;
      const_cast<Value&>(other).grad += out.grad;
    };

    return out;
  }

  Value Value::operator*(const Value& other) const {
    // if dont have first const, cant do a * Value(2) or a * 2
    // if dont have second const, if c=5, then do c*2, c = 10
    // DONT want c's value to change, so this is const
    std::string combined_str = this->name + "*" + other.name;
    Value out = Value(this->value * other.value, combined_str, "*");

    // C++ doesn't allow nested functions so must use lambda
    out._backprop = [this, &other, &out]() {
      // this is a pointer, so access member with ->
      const_cast<Value*>(this)->grad += other.value * out.grad;
      // other is a reference, so access member with .
      const_cast<Value&>(other).grad += value * out.grad;
    };

    return out;
  }

  Value Value::operator-() const {
    // in a member, we can just say value instead of this->value
    return Value(-value, "-" + name, "neg");
  }

  Value Value::operator-(const Value& other) const {
    // this is a pointer, so dereference to *value to actually get the object
    return *this + (-other);
  }

  Value Value::operator^(int exp) const {
    Value out = Value(std::pow(value, exp), name + '^' + std::to_string(exp), "pow");

    out._backprop = [this, exp, &out]() {
      const_cast<Value*>(this)->grad += exp * std::pow(value, exp - 1) * out.grad;
    };
    return out;
  }

  Value Value::operator/(const Value& other) const {
    return *this * (other ^ (-1));
  }

  Value Value::exp() const{
    double res = std::exp(value);
    Value out = Value(res, "exp(" + name + ")", "exp");

    out._backprop = [this, &out]() {
      const_cast<Value*>(this)->grad += out.value * out.grad;
    };
    return out;
  }

  void Value::print() {
    std::string combined = this->name + ": " + std::to_string(this->value);
    std::cout << "(" << this-> name << ")" << " : " << (*this).value << std::endl;
    //std::cout << combined << std::endl;
  }

  void Value::backward() {
    
    grad = 1.0;
    std::vector<Value*> topo;
    std::set<Value*> visited;

    std::function<void(Value*)> build_topo = [&] (Value* v) {
      if (visited.count(v) == 0) {
        visited.insert(v);
        for (const Value* child : v->children) {
          build_topo(const_cast<Value*>(child));
        }
        topo.push_back(v);
      }
    };
    build_topo(this);

    for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
        if ((*it)->_backprop) (*it)->_backprop();
    }
  }

      // Define non-member operations
  Value operator+(double lhs, const Value& rhs) {
    // Dont need second const here bc this function is non-member
    std::string combined_str = std::to_string(lhs) + "+" + rhs.name;
    return Value(lhs + rhs.get_value(), combined_str, "+");
  }

  Value operator*(double lhs, const Value& rhs) {
    // second type of implementation
    Value lhs_val(lhs, std::to_string(lhs));
    return lhs_val * rhs;
  }
};