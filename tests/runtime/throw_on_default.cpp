#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ThrowOnDefault_w = Widget<ThrowOnDefaultConstruction>;

  ThrowOnDefault_w w1{42};

  bool caught{false};
  try {
    ThrowOnDefault_w w2;
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(ThrowOnDefault_w::active_instances() == 1);
}
