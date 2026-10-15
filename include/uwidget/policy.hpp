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
   * @brief Causes the Nth execution of the specified operation to throw. Defaults to N = 1
   */
  template <Op OpV, size_t N = 1>
  struct ThrowOnNth : policy_base{};


  /**
   * @brief Forces the specified operation to be non-noexcept
   */
  template <Op OpV>
  struct ForceNonNoexcept : policy_base{};

  /**
   * @brief Dummy policy only used to differentiate otherwise identical Widgets in the same scope
   */
  template <auto n = []{}>
  struct Unique : policy_base{};

}
