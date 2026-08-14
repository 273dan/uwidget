#pragma once

namespace uwidget::policy {

  /**
   * @brief Base class determining if a struct is a policy
   */
  struct policy_base {};
  
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

}
