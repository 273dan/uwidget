#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ThrowOn3Copies_w = Widget<ThrowOnNthOperation<Op::CopyConstruction, 3>>;

  ThrowOn3Copies_w w{1};

  auto w_copy_1{w};
  auto w_copy_2{w};

  bool caught{false};
  try {
    auto w_copy_3{w};
  } catch(WidgetException& e) {
    caught = true;
  }

  assert(caught);

}
