#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using WidgetInt = Widget<int>;
  using CurrentSession = Session<WidgetInt>;

  WidgetInt w1{42};

  assert(w1.value == 42);
  assert(CurrentSession::get_data<RegisteredData::ValueConstruction>() == 1);

  // copy construction
  WidgetInt w2{w1};
  assert(w2.value == 42);

  // move construction
  WidgetInt w3{std::move(w2)};
  assert(w3.value == 42);

  // copy assignment
  WidgetInt w4{};
  w4 = w3;
  assert(w4.value == 42);

  // move assignment
  WidgetInt w5{};
  w5 = std::move(w4);
  assert(w5.value == 42);

}
