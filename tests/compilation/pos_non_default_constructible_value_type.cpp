#include "uwidget/uwidget.hpp"
#include <type_traits>

struct NonDefaultConstructible {
  NonDefaultConstructible() = delete;
  explicit NonDefaultConstructible(int x) : x{x} {}
  int x;
};

using namespace uwidget;

using NonDefaultConstructibleValued_w = Widget<NonDefaultConstructible>;

static_assert(!std::is_default_constructible_v<NonDefaultConstructibleValued_w>, "Widget should not be default constructible with non-default constructible value");
static_assert(std::is_constructible_v<NonDefaultConstructibleValued_w, NonDefaultConstructible>, "Widget should still be value constructible");

