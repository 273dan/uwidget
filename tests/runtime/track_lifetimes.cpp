#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = Widget<>;
  using CurrentSession = Session<Default_w>;

  assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 0);
  {
    Default_w w1{};
    Default_w w2{};
    assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 2);
    
    {
      Default_w w3 = w1;
      assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 3);
    }

    assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 2);
    assert(CurrentSession::get_data<RegisteredData::Destruction>() == 1);

    {
      Default_w w3 = std::move(w1);
      assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 3);
    }

    assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 2);
    assert(CurrentSession::get_data<RegisteredData::Destruction>() == 2);
    
  }

  assert(CurrentSession::get_data<RegisteredData::ActiveInstance>() == 0);
  assert(CurrentSession::get_data<RegisteredData::Destruction>() == 4);




}
