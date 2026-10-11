#include "uwidget/uwidget.hpp"

// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

template <typename T, size_t N>
class SmallVector {
public:

  ~SmallVector() {
    clear();
    if(on_heap) {
      free(data_);
    }

  }

  void clear() {
    for(size_t i{0}; i < size_; ++i) {
      std::destroy_at(&data_[i]);
    }
    size_ = 0;

  }

  void push_back(const T& x) {
    if(size_ == capacity_) {
      grow();
    }
    std::construct_at<T>(data_ + size_, x);
    ++size_;
  }

  size_t size() const { return size_; }

  T& operator[](size_t index) const {
    return data_[index];
  }

private:

  constexpr static size_t GROWTH{2};

  alignas(T) std::byte stack_buf_[sizeof(T) * N];
  T* data_{ reinterpret_cast<T*>(stack_buf_) };

  bool on_heap{ false };
  size_t size_{ 0 };
  size_t capacity_{ N };

  void grow() {
    size_t new_cap = capacity_ * GROWTH;
    T* new_buf = reinterpret_cast<T*>(malloc(sizeof(T) * new_cap));
    T* old_buf = data_;

    size_t constructed{0};
    try {
      for(size_t i{0}; i < size_; ++i) {
        if constexpr(std::is_nothrow_move_constructible_v<T>) std::construct_at(new_buf + i, std::move(old_buf[i]));
        else std::construct_at(new_buf + i, old_buf[i]);
        constructed++;
      }
    } catch (...) {
      for(size_t i{0}; i < constructed; ++i) std::destroy_at(new_buf + i);
      free(new_buf);
      throw;
    }

    for(size_t i{0}; i < size_; ++i) std::destroy_at(old_buf + i);


    data_ = new_buf;
    if(on_heap) free(old_buf);

    capacity_ = new_cap;
    on_heap = true;

  }


};

using namespace uwidget;
using namespace uwidget::policy;

TEST_CASE("no elements leaked") {
  /* Using a normal widget here, we are only looking at tracking metrics */
  using W = Widget<Unique<>>; 
  SUBCASE("during inline mode") {
    {
      SmallVector<W, 3> small_vec{};
      small_vec.push_back(W{});
      small_vec.push_back(W{});
      small_vec.push_back(W{});
    }
    CHECK(W::active_instances() == 0);
  }
  SUBCASE("during heap mode") {
    {
      SmallVector<W, 3> small_vec{};
      small_vec.push_back(W{});
      small_vec.push_back(W{});
      small_vec.push_back(W{});
      small_vec.push_back(W{});
      small_vec.push_back(W{});
    }
    CHECK(W::active_instances() == 0);
  }
}

TEST_CASE("reallocation semantics") {
  SUBCASE("when move is noexcept") {
    /* Using a normal widget here, we are only looking at tracking metrics */
    using W = Widget<Unique<>>;
    SmallVector<W, 3> small_vec{};
    const W w{};
    small_vec.push_back(w);
    small_vec.push_back(w);
    small_vec.push_back(w);
    small_vec.push_back(w);
    small_vec.push_back(w);
    CHECK_MESSAGE(W::default_constructions() == 1, "one default construction for 'w'");
    CHECK_MESSAGE(W::copy_constructions() == 5,    "each of 5 push backs copy constructs a new element");
    CHECK_MESSAGE(W::move_constructions() == 3,    "when small vector enters heap mode, the three existing items are moved to the heap");
  }
  SUBCASE("when move is non noexcept") {
    /* Here, W's move constructor is made non-noexcept so we can test if
     * small_vector correctly falls back on copy construction
     */
    using W = Widget<Unique<>, ForceNonNoexcept<Op::MoveConstruction>>;
    SmallVector<W, 3> small_vec{};
    const W w{};
    small_vec.push_back(w);
    small_vec.push_back(w);
    small_vec.push_back(w);
    small_vec.push_back(w);
    small_vec.push_back(w);

    CHECK_MESSAGE(W::default_constructions() == 1, "one default construction for 'w'");

    CHECK_MESSAGE(W::copy_constructions() == 8,    "each of 5 push backs copy constructs a new element," 
                                                   "plus 3 elements reallocated by copying as moving is non-noexcept");

    CHECK_MESSAGE(W::move_constructions() == 0,    "small vector should not move construct when non-noexcept");
  }
}

TEST_CASE("strong exception safety during reallocation") {
  /* Here, W's move constructor is disabled to force copy construction on reallocation
   * We set W to throw on the 4th copy construction to see if the strong exception guarantee is held
   */
  using W = Widget<Unique<>, Disable<Op::MoveConstruction>, ThrowOnNth<Op::CopyConstruction, 4>>;

  SmallVector<W, 3> small_vec{};

  const W w1{1};
  const W w2{2};
  const W w3{3};

  small_vec.push_back(w1);
  small_vec.push_back(w2);
  small_vec.push_back(w3);

  CHECK_THROWS(small_vec.push_back(w1));

  REQUIRE_MESSAGE(small_vec.size() == 3,    "the three original vec elements remain");

  CHECK_MESSAGE((
      small_vec[0].value == 1 &&
      small_vec[1].value == 2 &&
      small_vec[2].value == 3),             "the three original vec elements are preserved");
  
  CHECK_MESSAGE(W::active_instances() == 6, "three original widgets and three copies in the small vec, no leaked elements");

}
