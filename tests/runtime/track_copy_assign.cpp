#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using Default_w = Widget<>;
  using CurrentSession = Session<Default_w>;
  Default_w w{};
  Default_w w1{};
  Default_w w2{};

  w1 = w;
  w2 = w;

  assert(CurrentSession::get_data<RegisteredData::CopyAssignments>() == 2);
}
