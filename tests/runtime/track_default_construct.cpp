#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = Widget<>;
  using CurrentSession = Session<Default_w>;
  Default_w w{};

  assert(CurrentSession::get_data<RegisteredData::DefaultConstruction>() == 1);




}
