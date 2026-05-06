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
    BSTIterator();
    ~BSTIterator() = default;
    BSTIterator(const BSTIterator< Key, Value >&);
    BSTIterator(BSTIterator< Key, Value >&&);
    BSTIterator< Key, Value > opertor=(const BSTIterator< Key, Value >&);
    BSTIterator< Key, Value > opertor=(BSTIterator< Key, Value >&&);
    BSTIterator(Node*);

    BSTIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;

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
    BSTConstIterator();
    ~BSTConstIterator() = default;
    BSTConstIterator(const BSTConstIterator< Key, Value >&);
    BSTConstIterator(BSTConstIterator< Key, Value >&&);
    BSTConstIterator< Key, Value >& opertor=(const BSTConstIterator< Key, Value >&);
    BSTConstIterator< Key, Value >& opertor=(BSTConstIterator< Key, Value >&&);
    BSTConstIterator(const Node*);

    BSTConstIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTConstIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;

    BSTConstIterator< Key, Value > opertor++();

    const std::pair< Key, Value >& opertor*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
    friend BSTIterator< Key, Value >;
    friend BSTConstIterator< Key, Value >;
    Node* root;
  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);
    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);
    BSTree& opertor=(const BSTree< Key, Value, Compare >&);
    BSTree& opertor=(BSTree< Key, Value, Compare >&&);


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
