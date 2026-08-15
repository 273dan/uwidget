#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = Widget<>;

  assert(Default_w::active_instances() == 0);
  {
    Default_w w1{};
    Default_w w2{};
    assert(Default_w::active_instances() == 2);
    
    {
      Default_w w3 = w1;
      assert(Default_w::active_instances() == 3);
    }

    assert(Default_w::active_instances() == 2);
    assert(Default_w::destructions() == 1);

    {
      Default_w w3 = std::move(w1);
      assert(Default_w::active_instances() == 3);
    }

    assert(Default_w::active_instances() == 2);
    assert(Default_w::destructions() == 2);
    
  }

  assert(Default_w::active_instances() == 0);
  assert(Default_w::destructions() == 4);
}
