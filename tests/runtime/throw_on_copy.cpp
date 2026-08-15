#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ThrowOnCopy_w = Widget<ThrowOnCopy>;

  ThrowOnCopy_w w1{};

  bool caught = false;

  assert(ThrowOnCopy_w::active_instances() == 1);
  try {
    ThrowOnCopy_w w2 = w1;
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(ThrowOnCopy_w::active_instances() == 1);
}
