#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = Widget<>;

  Default_w w{};
  Default_w w1{w};
  Default_w w2{w};


  assert(Default_w::copy_constructions == 2);
}
