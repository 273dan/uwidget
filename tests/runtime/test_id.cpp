#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ValueFloat_w = Widget<Value<float>>;
  using ValueFloat2_w = Widget<Value<float>, Id<>>;

  ValueFloat_w w{};
  ValueFloat_w w1{};

  ValueFloat2_w w2_1{};
  ValueFloat2_w w2_2{};
  ValueFloat2_w w2_3{};

  assert(ValueFloat_w::default_constructions() == 2);
  assert(ValueFloat2_w::default_constructions() == 3);
}
