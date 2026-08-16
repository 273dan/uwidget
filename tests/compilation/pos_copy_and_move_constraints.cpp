#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;
using namespace uwidget::policy;
using NoCopy_w = Widget<NoCopy>;
using NoMove_w = Widget<NoMove>;
using ThrowOnMove_w = Widget<ThrowOn<Op::MoveConstruction>, ThrowOn<Op::MoveAssignment>>;

static_assert(!std::is_copy_assignable_v<NoCopy_w>, "No copy widget should not be copy assignable");
static_assert(!std::is_copy_constructible_v<NoCopy_w>, "No copy widget should not be copy constructable");

static_assert(!std::is_move_assignable_v<NoMove_w>, "No move widget should not be move assignable");
static_assert(!std::is_move_constructible_v<NoMove_w>, "No move widget should not be move constructable");

static_assert(!std::is_nothrow_move_assignable_v<ThrowOnMove_w>, "Throw on move widget should not be nothrow move assignable");
static_assert(!std::is_nothrow_move_constructible_v<ThrowOnMove_w>, "Throw on move widget should not be nothrow move constructable");

