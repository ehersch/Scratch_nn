#ifndef MICROGRAD_H
#define MICROGRAD_H

#include <string>
#include <set>
#include <functional>
#include <cmath>

namespace MicroGrad {
  class Value {
    private:
      double value;
      std::set<const Value*> children;

    public:
      std::string name;
      std::string operation;
      double grad;
      std::function<void()> _backprop;

      double get_value() const;
      Value(double value, std::string name = "", std::string operation = "");
      Value operator+(const Value& other) const;
      Value operator*(const Value& other) const;
      Value operator-() const;
      Value operator-(const Value& other) const;
      Value operator^(int exp) const;
      Value operator/(const Value& other) const;
      Value exp() const;

      void print();

      void backward();
  };
  Value operator+(double lhs, const Value& rhs);
  Value operator*(double lhs, const Value& rhs);
}
#endif