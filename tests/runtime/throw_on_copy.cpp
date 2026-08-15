#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using ThrowOnCopy_w = Widget<ThrowOnCopy>;
  using TestSession = Session<ThrowOnCopy_w>;

  ThrowOnCopy_w w1{};

  bool caught = false;

  assert(TestSession::get_data<RegisteredData::ActiveInstances>() == 1);
  try {
    ThrowOnCopy_w w2 = w1;
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(TestSession::get_data<RegisteredData::ActiveInstances>() == 1);

}
