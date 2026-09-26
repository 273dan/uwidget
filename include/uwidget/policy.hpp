#pragma once

#include "uwidget/operation.hpp"
#include <cstddef>
namespace uwidget::policy {

  /**
   * @brief Base class determining if a struct is a policy
   */
  struct policy_base {};
  

  /**
   * @brief Determines the type of the Widget's member. Defaults to int
   */
  template <typename T>
  struct Value : policy_base{};

  /**
   * @brief Deletes the operation specified by OpV
   */
  template <Op OpV>
  struct Disable : policy_base{};

  /**
   * @brief Disables both move operations
   * This is a shorthand for Disable<Op::MoveConstruction>, Disable<Op::MoveAssignment>
   */
  struct NoMove : policy_base{};

  /**
   * @brief Deletes copy both operations
   * This is a shorthand for Disable<Op::CopyConstruction>, Disable<Op::CopyAssignment>
   */
  struct NoCopy : policy_base{};

  /**
   * @brief Causes the operation specified by OpV to throw
   */
  template <Op OpV>
  struct ThrowOn : policy_base{};

  /**
   * @brief Causes the Nth instantiation of the specified operation to throw
   */
  template <Op op, size_t N>
  struct ThrowOnNthOperation : policy_base{};


  /**
   * @brief Forces the specified operation to be non-noexcept
   */
  template <Op op>
  struct ForceNonNoexcept : policy_base{};

  /**
   * @brief Dummy policy only used to differentiate otherwise identical Widgets in the same scope
   */
  template <int n = 0>
  struct Id : policy_base{};

}
