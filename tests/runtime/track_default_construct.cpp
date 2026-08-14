#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using DefaultWidget = Widget<>;
  using CurrentSession = Session<Widget<>>;
  Widget<> w{};

  assert(CurrentSession::get_data<RegisteredData::DefaultConstructions>() == 1);




}
