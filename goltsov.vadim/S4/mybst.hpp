#include <cstddef>

namespace goltsov
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data_;
    Node< Key, Value >* left_, * right_, * parent_;
    size_t height_;
  };

  template< class Key, class Value >
  class BSTIterator
  {
    Node* ptr_;
  public:
    BSTIterator< Key, Value > next() const;
    BSTIterator< Key, Value > hasNext() const noexcept;
    BSTIterator< Key, Value > prev() const;
    BSTIterator< Key, Value > hasPrev() const noexcept;

    BSTIterator< Key, Value > opertor++();

    std::pair< Key, Value >& opertor*() const;
    std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
    const Node* ptr_;
  public:
    BSTConstIterator< Key, Value > next() const;
    BSTConstIterator< Key, Value > hasNext() const noexcept;
    BSTConstIterator< Key, Value > prev() const;
    BSTConstIterator< Key, Value > hasPrev() const noexcept;

    BSTConstIterator< Key, Value > opertor++();

    const std::pair< Key, Value >& opertor*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
    Node* root;
  public:
    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);

    BSTConstIterator< Key, Value > rotateLeft(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateRight(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateLargeLeft(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateLargeRight(BSTConstIterator< Key, Value > it);

    size_t height(BSTConstIterator< Key, Value > it) const noexcept;
    size_t height() const noexcept;
  };
}
