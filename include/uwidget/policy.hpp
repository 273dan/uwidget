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
   * @brief Deletes move both operations
   */
  struct NoMove : policy_base{};

  /**
   * @brief Deletes copy both operations
   */
  struct NoCopy : policy_base{};

  /**
   * @brief Deletes default constructor
   */
  struct NoDefaultConstruct : policy_base{};

  /**
   * @brief Causes move both operations to throw
   */
  struct ThrowOnMove : policy_base{};

  /**
   * @brief Causes copy both operations to throw
   */
  struct ThrowOnCopy : policy_base{};

  /**
   * @brief Causes default construction to throw
   */
  struct ThrowOnDefaultConstruction : policy_base{};

  /**
   * @brief Causes the Nth instantiation of the specified operation to throw
   */
  template <Op op, size_t N>
  struct ThrowOnNthOperation : policy_base{};

}
