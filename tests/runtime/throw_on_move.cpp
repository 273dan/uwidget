#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ThrowOnMove_w = Widget<ThrowOnMove>;
  using TestSession = Session<ThrowOnMove_w>;

  ThrowOnMove_w w1{};

  bool caught = false;

  assert(TestSession::get_data<RegisteredData::ActiveInstance>() == 1);
  try {
    ThrowOnMove_w w2 = std::move(w1);
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(TestSession::get_data<RegisteredData::ActiveInstance>() == 1);

}
