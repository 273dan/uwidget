#pragma once

namespace uwidget::policy {

  // operator deletion
  struct NoMove{};
  struct NoCopy{};
  struct NoDefaultConstruct{};

  // throw on operations
  struct ThrowOnMove{};
  struct ThrowOnCopy{};
  struct ThrowOnDefaultConstruction{};

}
