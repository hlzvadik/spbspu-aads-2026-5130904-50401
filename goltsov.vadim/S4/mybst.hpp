#ifndef MYBST_HPP
#define MYBST_HPP
#include <cstddef>
#include "../Common/mystack.hpp"

namespace goltsov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  struct NodeBST
  {
    std::pair< Key, Value > data_;
    NodeBST< Key, Value >* left_;
    NodeBST< Key, Value >* right_;
    NodeBST< Key, Value >* parent_;
    size_t height_;
  };

  template< class Key, class Value >
  class BSTIterator
  {
    template< class K, class V, class C >
    friend class BSTree;
    friend BSTConstIterator< Key, Value >;
    NodeBST< Key, Value>* ptr_;
  public:
    BSTIterator();
    ~BSTIterator() = default;
    BSTIterator(const BSTIterator< Key, Value >&);
    BSTIterator(BSTIterator< Key, Value >&&);
    BSTIterator< Key, Value > operator=(const BSTIterator< Key, Value >&);
    BSTIterator< Key, Value > operator=(BSTIterator< Key, Value >&&);
    BSTIterator(NodeBST< Key, Value >*);

    BSTIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;

    BSTIterator< Key, Value > operator++();

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTConstIterator< Key, Value >&) const noexcept;

    operator BSTConstIterator< Key, Value >() const;

    NodeBST< Key, Value >* getPtr() const;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
    template< class K, class V, class C >
    friend class BSTree;
    friend class BSTIterator< Key, Value >;
    const NodeBST< Key, Value>* ptr_;
  public:
    BSTConstIterator();
    ~BSTConstIterator() = default;
    BSTConstIterator(const BSTConstIterator< Key, Value >&);
    BSTConstIterator(BSTConstIterator< Key, Value >&&);
    BSTConstIterator< Key, Value >& operator=(const BSTConstIterator< Key, Value >&);
    BSTConstIterator< Key, Value >& operator=(BSTConstIterator< Key, Value >&&);
    BSTConstIterator(const NodeBST< Key, Value >*);

    BSTConstIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTConstIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;

    BSTConstIterator< Key, Value > operator++();

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTConstIterator< Key, Value >&) const noexcept;

    const NodeBST< Key, Value >* getPtr() const;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
    friend class BSTIterator< Key, Value >;
    friend class BSTConstIterator< Key, Value >;
    NodeBST< Key, Value >* root_;
  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);
    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >&);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&&);

    void push(Key k, Value v);
    Value& get(Key k);
    Value drop(Key k);

    void swap(BSTree< Key, Value, Compare >&);

    BSTIterator< Key, Value > begin();
    BSTConstIterator< Key, Value > begin() const;
    BSTIterator< Key, Value > end();
    BSTConstIterator< Key, Value > end() const;

    BSTIterator< Key, Value > rotateLeft(BSTConstIterator< Key, Value > it);
    BSTIterator< Key, Value > rotateRight(BSTConstIterator< Key, Value > it);
    BSTIterator< Key, Value > rotateLargeLeft(BSTConstIterator< Key, Value > it);
    BSTIterator< Key, Value > rotateLargeRight(BSTConstIterator< Key, Value > it);

    size_t height(BSTConstIterator< Key, Value > it) const noexcept;
    size_t height() const noexcept;
  };
}

namespace goltsov
{
  template< class Key, class Value >
  NodeBST< Key, Value >* falLeft(NodeBST< Key, Value >* a)
  {
    if (!a)
    {
      return nullptr;
    }
    while(a->left_)
    {
      a = a->left_;
    }
    return a;
  }

  template< class Key, class Value >
  NodeBST< Key, Value >* falRight(NodeBST< Key, Value >* a)
  {
    if (!a)
    {
      return nullptr;
    }
    while(a->right_)
    {
      a = a->right_;
    }
    return a;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    root_(nullptr)
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    NodeBST< Key, Value >* current = root_;
    while (current)
    {
      if (!current->left_ && !current->right_)
      {
        if (!current->parent_)
        {
          delete current;
          root_ = nullptr;
          current = nullptr;
        }
        else
        {
          if (current == current->parent_->left_)
          {
            current = current->parent_;
            delete current->left_;
            current->left_ = nullptr;
          }
          else
          {
            current = current->parent_;
            delete current->right_;
            current->right_ = nullptr;
          }
        }
      }
      else if (current->left_)
      {
        current = current->left_;
      }
      else if (current->right_)
      {
        current = current->right_;
      }
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree< Key, Value, Compare >& other):
    root_(nullptr)
  {
    NodeBST< Key, Value >* current = other.root_;
    if (!current)
    {
      root_ = nullptr;
      return;
    }
    BSTree< Key, Value, Compare > new_tree;
    new_tree.root_ = new NodeBST< Key, Value >
      {current->data_, nullptr, nullptr, nullptr, current->height_};
    NodeBST< Key, Value >* new_node = new_tree.root_;
    while (current)
    {
      if (current->left_ && !new_node->left_)
      {
        new_node->left_ = new NodeBST< Key, Value >
          {current->left_->data_, nullptr, nullptr, new_node, current->left_->height_};
        new_node = new_node->left_;
        current = current->left_;
      }
      else if (current->right_ && !new_node->right_)
      {
        new_node->right_ = new NodeBST< Key, Value >
          {current->right_->data_, nullptr, nullptr, new_node, current->right_->height_};
        new_node = new_node->right_;
        current = current->right_;
      }
      else
      {
        new_node = new_node->parent_;
        current = current->parent_;
      }
    }
    swap(new_tree);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree< Key, Value, Compare >&& other):
    root_(other.root_)
  {
    other.root_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree< Key, Value, Compare >& other)
  {
    BSTree< Key, Value, Compare > new_tree (other);
    swap(new_tree);
    return (* this);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree< Key, Value, Compare >&& other)
  {
    if (&other != this)
    {
      swap(other);
    }
    return (* this);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(Key k, Value v)
  {
    Compare comparator;
    if (!root_)
    {
      root_ = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, nullptr, 1};
      return;
    }
    NodeBST< Key, Value >* current = root_;
    NodeBST< Key, Value >* inserted = nullptr;
    while (true)
    {
      if (comparator(k, current->data_.first))
      {
        if (!current->left_)
        {
          inserted = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, current, 1};
          current->left_ = inserted;
          break;
        }
        else
        {
          current = current->left_;
        }
      }
      else if (comparator(current->data_.first, k))
      {
        if (!current->right_)
        {
          inserted = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, current, 1};
          current->right_ = inserted;
          break;
        }
        else
        {
          current = current->right_;
        }
      }
      else
      {
        throw std::logic_error("Key allready in table");
      }
    }
    current = inserted;
    while (current)
    {
      size_t leftHeight = current->left_ ? current->left_->height_ : 0;
      size_t rightHeight = current->right_ ? current->right_->height_ : 0;
      current->height_ = 1 + std::max(leftHeight, rightHeight);
      current = current->parent_;
    }
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::get(Key k)
  {
    Compare comparator;
    NodeBST< Key, Value >* current = root_;
    while (current)
    {
      if (comparator(k, current->data_.first))
      {
        current = current->left_;
      }
      else if (comparator(current->data_.first, k))
      {
        current = current->right_;
      }
      else
      {
        return current->data_.second;
      }
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(Key k)
  {
    Compare comparator;
    NodeBST<Key, Value>* current = root_;
    while (current)
    {
      if (comparator(k, current->data_.first))
      {
         current = current->left_;
      }
      else if (comparator(current->data_.first, k))
      {
        current = current->right_;
      }
      else
      {
        NodeBST< Key, Value >* startHeightUpdate = nullptr;
        if (!current->left_ && !current->right_)
        {
          startHeightUpdate = current->parent_;
          if (current->parent_ && current->parent_->left_ == current)
          {
            current->parent_->left_ = nullptr;
          }
          else if (current->parent_)
          {
            current->parent_->right_ = nullptr;
          }
          else
          {
            root_ = nullptr;
          }
        }
        else if (current->left_ && !current->right_)
        {
          startHeightUpdate = current->parent_;
          if (current->parent_ && current->parent_->left_ == current)
          {
            current->parent_->left_ = current->left_;
          }
          else if (current->parent_)
          {
            current->parent_->right_ = current->left_;
          }
          else
          {
            root_ = current->left_;
          }
          if (current->left_)
          {
            current->left_->parent_ = current->parent_;
          }
        }
        else if (!current->left_ && current->right_)
        {
          startHeightUpdate = current->parent_;
          if (current->parent_ && current->parent_->left_ == current)
          {
            current->parent_->left_ = current->right_;
          }
          else if (current->parent_)
          {
            current->parent_->right_ = current->right_;
          }
          else
          {
            root_ = current->right_;
          }
          if (current->right_)
          {
            current->right_->parent_ = current->parent_;
          }
        }
        else
        {
          NodeBST<Key, Value>* successor = falLeft(current->right_);
          std::swap(current->data_, successor->data_);
          Value res = current->data_.second;
          startHeightUpdate = successor->parent_;
          if (successor->parent_->left_ == successor)
          {
            successor->parent_->left_ = successor->right_;
          }
          else
          {
            successor->parent_->right_ = successor->right_;
          }
          if (successor->right_)
          {
            successor->right_->parent_ = successor->parent_;
          }
          delete successor;
          NodeBST< Key, Value >* node = startHeightUpdate;
          while (node)
          {
            size_t leftHeight = node->left_ ? node->left_->height_ : 0;
            size_t rightHeight = node->right_ ? node->right_->height_ : 0;
            node->height_ = 1 + std::max(leftHeight, rightHeight);
            node = node->parent_;
          }
          return res;
        }
        Value res = current->data_.second;
        delete current;
        NodeBST< Key, Value >* node = startHeightUpdate;
        while (node)
        {
          size_t leftHeight = node->left_ ? node->left_->height_ : 0;
          size_t rightHeight = node->right_ ? node->right_->height_ : 0;
          node->height_ = 1 + std::max(leftHeight, rightHeight);
          node = node->parent_;
        }
        return res;
      }
    }
    throw std::runtime_error("Key is not in table");
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree< Key, Value, Compare >& other)
  {
    std::swap(root_, other.root_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::begin()
  {
    return BSTIterator< Key, Value >(falLeft(root_));
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::begin() const
  {
    return BSTConstIterator< Key, Value >(falLeft(root_));
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::end()
  {
    return BSTIterator< Key, Value >(nullptr);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::end() const
  {
    return BSTConstIterator< Key, Value >(nullptr);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLeft(BSTConstIterator< Key, Value > it)
  {
    NodeBST< Key, Value >* current = const_cast< NodeBST< Key, Value >* >(it.ptr_);
    if (!current)
    {
      throw std::logic_error("Can not do rotate. Iterator is empty");
    }
    if (!current->parent_)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    if (current == current->parent_->left_)
    {
      throw std::logic_error("Can not do left rotate.");
    }
    NodeBST< Key, Value >* P = current->parent_;
    NodeBST< Key, Value >* GP = current->parent_->parent_;
    NodeBST< Key, Value >* L = current->left_;
    P->right_ = L;
    if (L)
    {
      L->parent_ = P;
    }
    current->left_ = P;
    P->parent_ = current;
    current->parent_ = GP;
    if (GP)
    {
      if (GP->left_ == P)
      {
        GP->left_ = current;
      }
      else
      {
        GP->right_ = current;
      }
    }
    else
    {
      root_ = current;
    }
    P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
    current->height_ =
      std::max(current->right_ ? current->right_->height_ : 0, current->left_ ? current->left_->height_ : 0) + 1;
    if (current->parent_)
    {
      current->parent_->height_ = std::max(current->parent_->right_ ? current->parent_->right_->height_ : 0,
        current->parent_->left_ ? current->parent_->left_->height_ : 0) + 1;
    }
    return BSTIterator< Key, Value > (current->right_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateRight(BSTConstIterator< Key, Value > it)
  {
    NodeBST< Key, Value >* current = const_cast< NodeBST< Key, Value >* >(it.ptr_);
    if (!current)
    {
      throw std::logic_error("Can not do rotate. Iterator is empty");
    }
    if (!current->parent_)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    if (current == current->parent_->right_)
    {
      throw std::logic_error("Can not do right rotate.");
    }
    NodeBST< Key, Value >* P = current->parent_;
    NodeBST< Key, Value >* GP = current->parent_->parent_;
    NodeBST< Key, Value >* R = current->right_;
    P->left_ = R;
    if (R)
    {
      R->parent_ = P;
    }
    current->right_ = P;
    P->parent_ = current;
    current->parent_ = GP;
    if (GP)
    {
      if (GP->left_ == P)
      {
        GP->left_ = current;
      }
      else
      {
        GP->right_ = current;
      }
    }
    else
    {
      root_ = current;
    }
    P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
    current->height_ =
      std::max(current->right_ ? current->right_->height_ : 0, current->left_ ? current->left_->height_ : 0) + 1;
    if (current->parent_)
    {
      current->parent_->height_ = std::max(current->parent_->right_ ? current->parent_->right_->height_ : 0,
        current->parent_->left_ ? current->parent_->left_->height_ : 0) + 1;
    }
    return BSTIterator< Key, Value > (current->left_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeLeft(BSTConstIterator< Key, Value > it)
  {
    NodeBST< Key, Value >* current = const_cast< NodeBST< Key, Value >* >(it.ptr_);
    if (!current->parent_ || !current->parent_->parent_)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    rotateRight(it);
    rotateLeft(it);
    return BSTIterator< Key, Value > (it.ptr_->right_->left_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeRight(BSTConstIterator< Key, Value > it)
  {
    NodeBST< Key, Value >* current = const_cast< NodeBST< Key, Value >* >(it.ptr_);
    if (!current->parent_ || !current->parent_->parent_)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    rotateLeft(it);
    rotateRight(it);
    return BSTIterator< Key, Value > (it.ptr_->left_->right_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(BSTConstIterator< Key, Value > it) const noexcept
  {
    if (!it.ptr_)
    {
      return 0;
    }
    return it.ptr_->height_;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const noexcept
  {
    if (!root_)
    {
      return 0;
    }
    return root_->height_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    ptr_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(const BSTIterator< Key, Value >& other):
    ptr_(other.ptr_)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(BSTIterator< Key, Value >&& other):
    ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator=(const BSTIterator< Key, Value >& other)
  {
    ptr_ = other.ptr_;
    return (* this);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator=(BSTIterator< Key, Value >&& other)
  {
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return (* this);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(NodeBST< Key, Value >* ptr):
    ptr_(ptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::next() const
  {
    NodeBST< Key, Value >* current = ptr_;
    if (!current)
    {
      throw std::logic_error("No next");
    }
    if (current->right_)
    {
      return BSTIterator< Key, Value >(falLeft(current->right_));
    }
    NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->left_ == previos)
      {
        break;
      }
      previos = current;
      current = current->parent_;
    }
    return BSTIterator< Key, Value > (current);
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::hasNext() const noexcept
  {
    return ptr_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::prev() const
  {
    NodeBST< Key, Value >* current = ptr_;
    if (current->left_)
    {
      return BSTIterator< Key, Value >(falRight(current->left_));
    }
    NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->right_ == previos)
      {
        break;
      }
      previos = current;
      current = current->parent_;
    }
    return BSTIterator< Key, Value > (current);
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::hasPrev() const noexcept
  {
    return prev().ptr_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++()
  {
    (* this) = next();
    return BSTIterator< Key, Value > (* this);
  }

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*() const
  {
    return ptr_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->() const
  {
    return &(ptr_->data_);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::operator BSTConstIterator< Key, Value >() const
  {
    return BSTConstIterator< Key, Value > (ptr_);
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator():
    ptr_(nullptr)
  {}

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(const BSTConstIterator< Key, Value >& other):
    ptr_(other.ptr_)
  {}

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(BSTConstIterator< Key, Value >&& other):
    ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >& BSTConstIterator< Key, Value >::operator=(const BSTConstIterator< Key, Value >& other)
  {
    ptr_ = other.ptr_;
    return (* this);
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >& BSTConstIterator< Key, Value >::operator=(BSTConstIterator< Key, Value >&& other)
  {
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return (* this);
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(const NodeBST< Key, Value >* ptr)
  {
    ptr_ = ptr;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::next() const
  {
    NodeBST< Key, Value >* current = const_cast< NodeBST< Key, Value >* >(ptr_);
    if (!current)
    {
      throw std::logic_error("No next");
    }
    if (current->right_)
    {
      return BSTConstIterator< Key, Value >(falLeft(current->right_));
    }
    NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->left_ == previos)
      {
        break;
      }
      previos = current;
      current = current->parent_;
    }
    return BSTConstIterator< Key, Value > (current);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::hasNext() const noexcept
  {
    return ptr_;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::prev() const
  {
    NodeBST< Key, Value >* current = const_cast< NodeBST< Key, Value >* > (ptr_);
    if (current->left_)
    {
      return BSTConstIterator< Key, Value > (falRight(current->left_));
    }
    NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->right_ == previos)
      {
        break;
      }
      previos = current;
      current = current->parent_;
    }
    return BSTConstIterator< Key, Value > (current);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::hasPrev() const noexcept
  {
    return prev().ptr_;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator++()
  {
    (* this) = next();
    return (* this);
  }

  template< class Key, class Value >
  const std::pair< Key, Value >& BSTConstIterator< Key, Value >::operator*() const
  {
    return ptr_->data_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >* BSTConstIterator< Key, Value >::operator->() const
  {
    return & (ptr_->data_);
  }

  template< class Key, class Value >
  bool operator==(const BSTConstIterator< Key, Value >& a, const BSTConstIterator< Key, Value >& b)
  {
    return a.ptr_ == b.ptr_;
  }

  template< class Key, class Value >
  bool operator!=(const BSTConstIterator< Key, Value >& a, const BSTConstIterator< Key, Value >& b)
  {
    return !(a.ptr_ == b.ptr_);
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator< Key, Value >& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator< Key, Value >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTConstIterator< Key, Value >& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTConstIterator< Key, Value >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTIterator< Key, Value >& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTIterator< Key, Value >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTConstIterator< Key, Value >& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator< Key, Value >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class Key, class Value >
  NodeBST< Key, Value >* BSTIterator< Key, Value >::getPtr() const
  {
    return ptr_;
  }

  template< class Key, class Value >
  const NodeBST< Key, Value >* BSTConstIterator< Key, Value >::getPtr() const
  {
    return ptr_;
  }
}

#endif
