# uwidget
[![CI](https://github.com/273dan/uwidget/actions/workflows/ci.yml/badge.svg)](https://github.com/273dan/uwidget/actions/workflows/ci.yml)

**uwidget** is a C++23 header only library designed to test custom containers.


```cpp
#include "doctest.h"
#include "uwidget/uwidget.hpp"
using namespace uwidget;
using namespace uwidget::policy;

TEST_CASE("test strong exception guarantee") {

  using TestWidget = Widget<Value<int>, ThrowOnNth<Op::CopyConstruction, 2>>;
  {
      MyContainer<TestWidget> vec{};

      const TestWidget item1{1};
      const TestWidget item2{2};

      vec.push_back(item1);


      // this will throw when using ThrowOnNth<..., 2>
      CHECK_THROWS(vec.push_back(item2));


      // verify container state was rolled back
      CHECK(vec.size() == 1);
      CHECK(vec[0].value == 1);
    }


  // verify no memory leaks
  CHECK(TestWidget::active_instances() == 0);
}

```

It provides the `Widget` class designed to test container behaviour using custom policies like `Disable<Op::CopyConstruction>`, `ForceNonNoexcept<Op::MoveConstruction>`, and by tracking constructions, assignments and destructions.

To get started, take a look at the `examples/` and follow **Getting Started**

# Operations
The operations tracked by all `Widget`s are defined in the `Op` enum.
```cpp
// include/uwidget/operation.hpp
namespace uwidget {
enum class Op: uint8_t {
  ActiveInstance = 0,
  Destruction,
  DefaultConstruction,
  CopyConstruction,
  CopyAssignment,
  MoveConstruction,
  MoveAssignment,
  ValueConstruction
};
}
```
The `Op` enum is used throughout the library to specify operation types.
# Policy Reference
## Default Widget
Set up a `Widget` type by injecting policies. It is recommended to use a `using` declaration at the beginning of the test case/fixture.
```cpp
using namespace uwidget;
using namespace uwidget::policy;
using W = Widget<Unique<>, NoMove>;
```

A "default" widget (one with no explicit policies) tracks all constructions, assignments, and destructions, and has a single `int` member named `value`, with no extra behaviour.
```cpp
using W = Widget<>;
W w{10};
CHECK(W::value_constructions() == 1);
CHECK(w.value == 10);
```
All operation tracking is correct when multiple threads perform operations on the same `Widget` type concurrently.

## Policies
Specify policies by templating them (and their arguments if necessary) into a `Widget` type definition.
### Disable
- Arguments: `Op OpV`
- Purpose: Deletes the operation specified by `OpV`
### ForceNonNoexcept
- Arguments: `Op OpV`
- Purpose: Forces `OpV` to be non-noexcept
### ThrowOnNth
- Arguments: `Op OpV, size_t N`
- Purpose: Causes the `N`th execution of `OpV` to throw. `N` defaults to 1
### Unique
- Arguments: `auto n` (any)
- Purpose: `Widget` types with the exact same policy configuration will share metric tracking. Use `Unique<>` to force a `Widget`s policy configuration to be unique if this is a problem.

> [!WARNING]
>`Unique<>`'s default argument is a lambda function, which on **gcc** will make each instance of Unique<> truly unique. With **Clang** multiple `Unique<>`s will share a type signature, so use `[]{}` at the **instantiation site**.

### Value
- Arguments: `typename T`
- Purpose: Sets the type of the `Widget`s member to `T` (`int` by default)

## Meta Policies
"Meta policies" are policies which, when used in instantiation of a `Widget`, expand to multiple policies. They are used as shorthand for common configurations.
### NoMove
- Arguments: None
- Expands to: `Disable<Op::MoveConstruction>, Disable<Op::MoveAssignment>`
- Purpose: Shorthand to disable move operations
### NoCopy
- Arguments: None
- Expands to: `Disable<Op::CopyConstruction>, Disable<Op::CopyAssignment>`
- Purpose: Shorthand to disable copy operations

# Getting Started
To use **uwidget**, either:
- Include `single_header/uwidget/uwidget.hpp`
- Install the headers with `sudo make install` (or set `UW_INSTALL_PREFIX` to install to a custom location) and use `find_package(uwidget)`

