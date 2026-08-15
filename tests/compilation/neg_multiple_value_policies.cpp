#include "uwidget/uwidget.hpp"

using namespace uwidget;
using MultipleValuePolicies_w = Widget<policy::Value<int>, policy::NoMove, policy::Value<double>>;

MultipleValuePolicies_w x{};
