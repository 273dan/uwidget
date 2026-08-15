#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ThrowOnMove_w = Widget<ThrowOnMove>;

  ThrowOnMove_w w1{};

  bool caught = false;

  assert(ThrowOnMove_w::active_instances == 1);
  try {
    ThrowOnMove_w w2 = std::move(w1);
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(ThrowOnMove_w::active_instances == 1);
}
