#include "uwidget/uwidget.hpp"
#include <type_traits>

struct NonDefaultConstructible {
  NonDefaultConstructible() = delete;
  explicit NonDefaultConstructible(int x) : x{x} {}
  int x;
};

using namespace uwidget;

using NonDefautConstructibleValuedWidget = Widget<NonDefaultConstructible>;

static_assert(!std::is_default_constructible_v<NonDefautConstructibleValuedWidget>, "Widget should not be default constructible with non-default constructible value");
static_assert(std::is_constructible_v<NonDefautConstructibleValuedWidget, NonDefaultConstructible>, "Widget should still be value constructible");

