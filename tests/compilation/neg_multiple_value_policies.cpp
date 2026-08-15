#include "uwidget/uwidget.hpp"

using namespace uwidget;
using namespace uwidget::policy;
using MultipleValuePolicies_w = Widget<Value<int>, NoMove, Value<double>>;

MultipleValuePolicies_w x{};
