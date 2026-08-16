#include <cassert>
#include <unordered_set>
#include "uwidget/uwidget.hpp"
using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Int_w = Widget<Value<int>>;

  std::unordered_set<Int_w> us{};

  us.insert(Int_w{1});
  us.insert(Int_w{2});

  assert(us.find(Int_w{1}) != us.end());
  assert(us.find(Int_w{2}) != us.end());
  assert(us.find(Int_w{42}) == us.end());
}
