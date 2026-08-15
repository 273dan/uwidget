#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Int_w = Widget<Value<int>>;
  using CurrentSession = Session<Int_w>;

  Int_w w1{42};

  assert(w1.value == 42);
  assert(CurrentSession::get_data<RegisteredData::ValueConstruction>() == 1);

  // copy construction
  Int_w w2{w1};
  assert(w2.value == 42);

  // move construction
  Int_w w3{std::move(w2)};
  assert(w3.value == 42);

  // copy assignment
  Int_w w4{};
  w4 = w3;
  assert(w4.value == 42);

  // move assignment
  Int_w w5{};
  w5 = std::move(w4);
  assert(w5.value == 42);

}
