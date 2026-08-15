#include "uwidget/uwidget.hpp"
#include <cassert>
using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Int_w = Widget<Value<int>>;
  Int_w w1{1};
  Int_w w2{2};
  Int_w w3{42};
  Int_w w4{42};

  assert(w2 > w1);
  assert(w1 < w2);
  assert(w2 >= w1);
  assert(w1 <= w2);
  assert(w1 != w2);

  assert(w3 == w4);

}
