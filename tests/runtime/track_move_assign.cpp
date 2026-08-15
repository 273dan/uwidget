#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = Widget<>;

  Default_w w_move_from1{};
  Default_w w_move_from2{};
  Default_w w1{};
  Default_w w2{};
  w1 = std::move(w_move_from1);
  w2 = std::move(w_move_from2);


  assert(Default_w::move_assignments == 2);
}
