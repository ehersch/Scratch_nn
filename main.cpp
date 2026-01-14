#include "micrograd.h"
#include <iostream> // Required for std::cout

using namespace MicroGrad;

Value f(Value x) {
  return 2 * (x^2) + 5;
}

Value test_function(Value x1, Value x2, Value w1, Value w2, Value b) {
  Value x1w1 = x1 * w1;
  Value x2w2 = x2 * w2;
  Value x1w1x2w2 = x1w1 + x2w2;
  Value n = x1w1x2w2 + b;
  Value e = (2*n).exp();
  //Value o = (e - 1) / (e + 1);
  //return o;
  return e;
}

int main() {
  // Value a(2, "a");

  // // Dynamic allocation
  // Value* b = new Value(3, "b");
  // Value* ptr = &a;
  // a.print();
  // std::cout << ((*ptr).name) << std::endl;
  // b->print();
  // delete b;  // frees memory, not the variable itself
  // Value c = a + b;
  // (c*2).print();
  // c.print();
  // (2*c).print();
  // (2+c).print();
  // (c - 3).print();

  /*
     L = exp(2((x1w1 + x2w2) + b))
  */

  Value x1 = Value(2, "x1");
  Value x2 = Value(0, "x2");
  Value w1 = Value(-3, "w1");
  Value w2 = Value(1, "w2");
  Value b = Value(6.8813735870195432, "b");

  Value x1w1 = x1 * w1;
  Value x2w2 = x2 * w2;
  Value x1w1x2w2 = x1w1 + x2w2;
  Value n = x1w1x2w2 + b;
  Value e = (2*n).exp();
  //Value o = (e - 1) / (e + 1);
  //return o;

  //alue L = test_function(2, 0, -3, 1, 6.8813735870195432);
  //L.backward();
  std::cout << "e.grad : " << e.grad << std::endl;
  std::cout << "n.grad : " << n.grad << std::endl;
  std::cout << "x1w1x2w2.grad : " << x1w1x2w2.grad << std::endl;
  std::cout << "x2w2.grad : " << x2w2.grad << std::endl;
  return 0;
}