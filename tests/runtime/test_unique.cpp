#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ValueFloat1_w = Widget<Value<float>>;
  using ValueFloat2_w = Widget<Value<float>, Unique<>>;
  using ValueFloat3_w = Widget<Value<float>, Unique<>>;

  ValueFloat1_w w_1{};

  ValueFloat2_w w2_1{};
  ValueFloat2_w w2_2{};

  ValueFloat3_w w3_1{};
  ValueFloat3_w w3_2{};
  ValueFloat3_w w3_3{};

  assert(ValueFloat1_w::default_constructions() == 1);
  assert(ValueFloat2_w::default_constructions() == 2);
  assert(ValueFloat3_w::default_constructions() == 3);
}
