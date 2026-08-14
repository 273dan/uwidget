#pragma once

namespace uwidget::policy {

  // base policy
  struct policy_base {};
  
  // operator deletion
  struct NoMove : policy_base{};
  struct NoCopy : policy_base{};
  struct NoDefaultConstruct : policy_base{};

  // throw on operations
  struct ThrowOnMove : policy_base{};
  struct ThrowOnCopy : policy_base{};
  struct ThrowOnDefaultConstruction : policy_base{};

}
