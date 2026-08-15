#include "uwidget/uwidget.hpp"
#include <type_traits>
#include <string>

using namespace uwidget;


// primitive value_t
using Float_w = Widget<float>;
static_assert(std::is_same_v<Float_w::value_t, float>, "FloatWidget value should be float");
static_assert(std::is_copy_constructible_v<Float_w>, "FloatWidget should be copy constructible");
static_assert(std::is_move_constructible_v<Float_w>, "FloatWidget should be move constructible");
static_assert(std::is_copy_assignable_v<Float_w>, "FloatWidget should be copy assignable");
static_assert(std::is_move_assignable_v<Float_w>, "FloatWidget should be move assignable");
Float_w fw{42.0f};

// complex value_t
using String_w = Widget<std::string>;
static_assert(std::is_same_v<String_w::value_t, std::string>, "StringWidget value should be string");
static_assert(std::is_copy_constructible_v<String_w>, "StringWidget should be copy constructible");
static_assert(std::is_move_constructible_v<String_w>, "StringWidget should be move constructible");
static_assert(std::is_copy_assignable_v<String_w>, "StringWidget should be copy assignable");
static_assert(std::is_move_assignable_v<String_w>, "StringWidget should be move assignable");
String_w sw{"hello, world!"};

// with policies
using FloatNoCopy_w = Widget<float, policy::NoCopy>;
static_assert(std::is_same_v<FloatNoCopy_w::value_t, float>, "FloatNoCopyWidget value should be float");
static_assert(!std::is_copy_constructible_v<FloatNoCopy_w>, "FloatNoCopyWidget should not be copy constructible");
static_assert(std::is_move_constructible_v<FloatNoCopy_w>, "FloatNoCopyWidget should be move constructible");
static_assert(!std::is_copy_assignable_v<FloatNoCopy_w>, "FloatNoCopyWidget should not be copy assignable");
static_assert(std::is_move_assignable_v<FloatNoCopy_w>, "FloatNoCopyWidget should be move assignable");
FloatNoCopy_w fncw{42.0f};
