#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using Default_w = Widget<>;
  using CurrentSession = Session<Default_w>;

  assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 0);
  {
    Default_w w1{};
    Default_w w2{};
    assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 2);
    
    {
      Default_w w3 = w1;
      assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 3);
    }

    assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 2);
    assert(CurrentSession::get_data<RegisteredData::Destructions>() == 1);

    {
      Default_w w3 = std::move(w1);
      assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 3);
    }

    assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 2);
    assert(CurrentSession::get_data<RegisteredData::Destructions>() == 2);
    
  }

  assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 0);
  assert(CurrentSession::get_data<RegisteredData::Destructions>() == 4);




}
