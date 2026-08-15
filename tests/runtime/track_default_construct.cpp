#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = Widget<>;
  Default_w w{};

  assert(Default_w::default_constructions() == 1);
}
