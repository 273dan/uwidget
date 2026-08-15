#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;
using namespace uwidget::policy;

struct ThrowingMoveValue {
  ThrowingMoveValue& operator=(ThrowingMoveValue&&) noexcept(false) {return *this; }
  ThrowingMoveValue(ThrowingMoveValue&&) noexcept(false) {}
};

using ThrowingMoveValue_w = Widget<Value<ThrowingMoveValue>>;

static_assert(!std::is_nothrow_move_assignable_v<ThrowingMoveValue_w>, "Throwing move value widget should not be nothrow move assignable");
static_assert(!std::is_nothrow_move_constructible_v<ThrowingMoveValue_w>, "Throwing move value widget should not be nothrow move constructible");


