#ifndef MYRBT_HPP
#define MYRBT_HPP
#include <iostream>

namespace detail
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  struct NodeBST
  {
    std::pair< Key, Value > data_;
    NodeBST< Key, Value >* left_, * right_, * parent_;
    bool is_black_;
    size_t height_;
    size_t black_height_;
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
    BSTIterator< Key, Value > operator--();

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
    size_t size_;
  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);
    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >&);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&&);

    NodeBST< Key, Value >* push(Key k, Value v);
    BSTIterator< Key, Value > get(const Key& k);
    BSTConstIterator< Key, Value > get(const Key& k) const;
    std::tuple< NodeBST< Key, Value >*, Value, bool > drop(const Key& k);
    void clear();
    template< class Predicate >
    BSTIterator< Key, Value > find(Predicate);
    template< class Predicate >
    BSTIterator< Key, Value > rfind(Predicate);

    void swap(BSTree< Key, Value, Compare >&);

    size_t size() const;

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

    size_t blackHeight(BSTConstIterator< Key, Value > it) const noexcept;
    size_t blackHeight() const noexcept;
  };

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
    root_(nullptr),
    size_(0)
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
    root_(nullptr),
    size_(0)
  {
    NodeBST< Key, Value >* current = other.root_;
    if (!current)
    {
      root_ = nullptr;
      return;
    }
    BSTree< Key, Value, Compare > new_tree;
    new_tree.root_ = new NodeBST< Key, Value >
      {current->data_, nullptr, nullptr, nullptr, current->is_black_, current->height_, current->black_height_};
    new_tree.size_ = other.size_;
    NodeBST< Key, Value >* new_node = new_tree.root_;
    while (current)
    {
      if (current->left_ && !new_node->left_)
      {
        new_node->left_ = new NodeBST< Key, Value >
          {current->left_->data_, nullptr, nullptr, new_node, current->left_->is_black_, current->left_->height_, current->left_->black_height_};
        new_node = new_node->left_;
        current = current->left_;
      }
      else if (current->right_ && !new_node->right_)
      {
        new_node->right_ = new NodeBST< Key, Value >
          {current->right_->data_, nullptr, nullptr, new_node, current->right_->is_black_, current->right_->height_, current->right_->black_height_};
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
    root_(other.root_),
    size_(other.size_)
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
  NodeBST< Key, Value >* BSTree< Key, Value, Compare >::push(Key k, Value v)
  {
    Compare comparator;
    NodeBST< Key, Value >* current = root_;
    NodeBST< Key, Value >* inserted = nullptr;
    if (!current)
    {
      inserted = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, nullptr, true, 1, 1};
      size_ += 1;
      root_ = inserted;
    }
    while (!inserted)
    {
      if (comparator(k, current->data_.first))
      {
        if (!current->left_)
        {
          inserted = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, current, false, 1, 0};
          size_ += 1;
          current->left_ = inserted;
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
          inserted = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, current, false, 1, 0};
          size_ += 1;
          current->right_ = inserted;
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
      size_t leftBlackHeight = current->left_ ? current->left_->black_height_ : 0;
      size_t rightBlackHeight = current->right_ ? current->right_->black_height_ : 0;
      current->black_height_ = (current->is_black_ ? 1 : 0) + std::max(leftBlackHeight, rightBlackHeight);
      size_t leftHeight = current->left_ ? current->left_->height_ : 0;
      size_t rightHeight = current->right_ ? current->right_->height_ : 0;
      current->height_ = 1 + std::max(leftHeight, rightHeight);
      current = current->parent_;
    }
    return inserted;
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::get(const Key& k)
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
        return BSTIterator< Key, Value > (current);
      }
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::get(const Key& k) const
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
        return BSTIterator< Key, Value > (current);
      }
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Compare >
  std::tuple< NodeBST< Key, Value >*, Value, bool > BSTree< Key, Value, Compare >::drop(const Key& k)
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
          NodeBST< Key, Value >* maxLeft = falRight(current->left_);
          NodeBST< Key, Value >* currParent = current->parent_;
          NodeBST< Key, Value >* currLeft = current->left_;
          NodeBST< Key, Value >* maxParent = maxLeft->parent_;
          NodeBST< Key, Value >* maxLeftChild = maxLeft->left_;
          std::swap(current->black_height_, maxLeft->black_height_);
          std::swap(current->height_, maxLeft->height_);
          std::swap(current->is_black_, maxLeft->is_black_);
          maxLeft->parent_ = currParent;
          if (currParent)
          {
            if (currParent->left_ == current) currParent->left_ = maxLeft;
            else currParent->right_ = maxLeft;
          }
          else
          {
            root_ = maxLeft;
          }
          maxLeft->right_ = nullptr;
          if (maxLeft == currLeft)
          {
            maxLeft->left_ = current;
            current->parent_ = maxLeft;
          }
          else
          {
            maxLeft->left_ = currLeft;
            if (currLeft) currLeft->parent_ = maxLeft;
            current->parent_ = maxParent;
            if (maxParent)
            {
              if (maxParent->left_ == maxLeft) maxParent->left_ = current;
              else maxParent->right_ = current;
            }
          }
          current->right_ = nullptr;
          current->left_ = maxLeftChild;
          if (maxLeftChild) maxLeftChild->parent_ = current;
          startHeightUpdate = current->parent_;
          if (current->parent_ && current->parent_->left_ == current)
          {
            current->parent_->left_ = current->left_;
            if (current->left_)
            {
              current->left_->parent_ = current->parent_;
            }
          }
          else if (current->parent_)
          {
            current->parent_->right_ = current->left_;
            if (current->left_)
            {
              current->left_->parent_ = current->parent_;
            }
          }
          else
          {
            root_ = current->left_;
            if (current->left_)
            {
              current->left_->parent_ = nullptr;
            }
          }
        }
        else
        {
          NodeBST< Key, Value >* minRight = falLeft(current->right_);
          NodeBST< Key, Value >* currParent = current->parent_;
          NodeBST< Key, Value >* currLeft = current->left_;
          NodeBST< Key, Value >* currRight = current->right_;
          NodeBST< Key, Value >* minParent = minRight->parent_;
          NodeBST< Key, Value >* minRightChild = minRight->right_;
          std::swap(current->black_height_, minRight->black_height_);
          std::swap(current->height_, minRight->height_);
          std::swap(current->is_black_, minRight->is_black_);
          minRight->parent_ = currParent;
          if (currParent)
          {
            if (currParent->left_ == current) currParent->left_ = minRight;
            else currParent->right_ = minRight;
          }
          else
          {
            root_ = minRight;
          }
          minRight->left_ = currLeft;
          if (currLeft) currLeft->parent_ = minRight;
          if (minRight == currRight)
          {
            minRight->right_ = current;
            current->parent_ = minRight;
          }
          else
          {
            minRight->right_ = currRight;
            if (currRight) currRight->parent_ = minRight;
            current->parent_ = minParent;
            if (minParent)
            {
              if (minParent->left_ == minRight) minParent->left_ = current;
              else minParent->right_ = current;
            }
          }
          current->left_ = nullptr;
          current->right_ = minRightChild;
          if (minRightChild) minRightChild->parent_ = current;
          startHeightUpdate = current->parent_;
          if (current->parent_ && current->parent_->left_ == current)
          {
            current->parent_->left_ = current->right_;
            if (current->right_)
            {
              current->right_->parent_ = current->parent_;
            }
          }
          else if (current->parent_)
          {
            current->parent_->right_ = current->right_;
            if (current->right_)
            {
              current->right_->parent_ = current->parent_;
            }
          }
          else
          {
            root_ = current->right_;
            if (current->right_)
            {
              current->right_->parent_ = nullptr;
            }
          }
        }
        NodeBST< Key, Value >* res_ptr = current->parent_;
        Value res_val = current->data_.second;
        bool res_is_black = current->is_black_;
        delete current;
        size_ -= 1;
        NodeBST< Key, Value >* node = startHeightUpdate;
        while (node)
        {
          size_t leftBlackHeight = node->left_ ? node->left_->black_height_ : 0;
          size_t rightBlackHeight = node->right_ ? node->right_->black_height_ : 0;
          node->black_height_ = (node->is_black_ ? 1 : 0) + std::max(leftBlackHeight, rightBlackHeight);
          size_t leftHeight = node->left_ ? node->left_->height_ : 0;
          size_t rightHeight = node->right_ ? node->right_->height_ : 0;
          node->height_ = 1 + std::max(leftHeight, rightHeight);
          node = node->parent_;
        }
        return {res_ptr, res_val, res_is_black};
      }
    }
    throw std::runtime_error("Key is not in table");
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
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
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  template< class Predicate >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::find(Predicate pred)
  {
    if (!root_)
    {
      return end();
    }
    for (BSTIterator< Key, Value > current = begin(); current != end(); ++current)
    {
      if (pred(* current))
      {
        return current;
      }
    }
    return end();
  }

  template< class Key, class Value, class Compare >
  template< class Predicate >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rfind(Predicate pred)
  {
    if (!root_)
    {
      return end();
    }
    for (BSTIterator< Key, Value > current = falRight(root_); current != begin(); --current)
    {
      if (pred(* current))
      {
        return current;
      }
    }
    if (pred(* begin()))
    {
      return begin();
    }
    return end();
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree< Key, Value, Compare >& other)
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const
  {
    return size_;
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
    P->black_height_ =
      std::max(P->left_ ? P->left_->black_height_ : 0, P->right_ ? P->right_->black_height_ : 0) + (P->is_black_ ? 1 : 0);
    current->height_ =
      std::max(current->right_ ? current->right_->height_ : 0, current->left_ ? current->left_->height_ : 0) + 1;
    current->black_height_ =
      std::max(current->left_ ? current->left_->black_height_ : 0, current->right_ ? current->right_->black_height_ : 0) + (current->is_black_ ? 1 : 0);
    if (current->parent_)
    {
      current->parent_->height_ = std::max(current->parent_->right_ ? current->parent_->right_->height_ : 0,
        current->parent_->left_ ? current->parent_->left_->height_ : 0) + 1;
      current->parent_->black_height_ = std::max(current->parent_->right_ ? current->parent_->right_->black_height_ : 0,
        current->parent_->left_ ? current->parent_->left_->black_height_ : 0) + (current->parent_->is_black_ ? 1 : 0);
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
    P->black_height_ =
      std::max(P->left_ ? P->left_->black_height_ : 0, P->right_ ? P->right_->black_height_ : 0) + (P->is_black_ ? 1 : 0);
    current->height_ =
      std::max(current->right_ ? current->right_->height_ : 0, current->left_ ? current->left_->height_ : 0) + 1;
    current->black_height_ =
      std::max(current->left_ ? current->left_->black_height_ : 0, current->right_ ? current->right_->black_height_ : 0) + (current->is_black_ ? 1 : 0);
    if (current->parent_)
    {
      current->parent_->height_ = std::max(current->parent_->right_ ? current->parent_->right_->height_ : 0,
        current->parent_->left_ ? current->parent_->left_->height_ : 0) + 1;
      current->parent_->black_height_ = std::max(current->parent_->right_ ? current->parent_->right_->black_height_ : 0,
        current->parent_->left_ ? current->parent_->left_->black_height_ : 0) + (current->parent_->is_black_ ? 1 : 0);
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

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::blackHeight(BSTConstIterator< Key, Value > it) const noexcept
  {
    if (!it.ptr_)
    {
      return 0;
    }
    return it.ptr_->black_height_;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::blackHeight() const noexcept
  {
    if (!root_)
    {
      return 0;
    }
    return root_->black_height_;
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
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--()
  {
    (* this) = prev();
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

namespace goltsov
{
  template< class Key, class Value >
  using RBTIterator = detail::BSTIterator< Key, Value >;
  template< class Key, class Value >
  using RBTConstIterator = detail::BSTConstIterator< Key, Value >;

  template< class Key, class Value, class Compare >
  class RBTree
  {
  public:
    RBTree() = default;
    ~RBTree() = default;
    RBTree(const RBTree< Key, Value, Compare >&);
    RBTree(RBTree< Key, Value, Compare >&&);
    RBTree< Key, Value, Compare >& operator=(const RBTree< Key, Value, Compare >&);
    RBTree< Key, Value, Compare >& operator=(RBTree< Key, Value, Compare >&&);

    RBTIterator< Key, Value > push(Key, Value);
    RBTIterator< Key, Value > get(const Key&);
    RBTConstIterator< Key, Value > get(const Key&) const;
    Value drop(const Key&);
    void clear();
    template< class Predicate >
    RBTIterator< Key, Value > find(Predicate);
    template< class Predicate >
    RBTIterator< Key, Value > rfind(Predicate);

    void swap(RBTree< Key, Value, Compare >&);

    size_t size() const;

    RBTIterator< Key, Value > begin();
    RBTConstIterator< Key, Value > begin() const;
    RBTIterator< Key, Value > end();
    RBTConstIterator< Key, Value > end() const;

    size_t height(RBTConstIterator< Key, Value > it) const noexcept;
    size_t height() const noexcept;

    size_t blackHeight(RBTConstIterator< Key, Value > it) const noexcept;
    size_t blackHeight() const noexcept;
  private:
    detail::BSTree< Key, Value, Compare > tree_;
    void makeBalanceAfterPush(detail::NodeBST< Key, Value >*);
    void makeBalanceAfterDrop(std::tuple< detail::NodeBST< Key, Value >*, Value, bool >);
  };

  template< class Key, class Value, class Compare >
  RBTree< Key, Value, Compare >::RBTree(const RBTree< Key, Value, Compare >& other):
    tree_(other.tree_)
  {}
  template< class Key, class Value, class Compare >
  RBTree< Key, Value, Compare >::RBTree(RBTree< Key, Value, Compare >&& other):
    tree_(std::move(other.tree_))
  {}
  template< class Key, class Value, class Compare >
  RBTree< Key, Value, Compare >& RBTree< Key, Value, Compare >::operator=(const RBTree< Key, Value, Compare >& other)
  {
    tree_ = other.tree_;
    return (* this);
  }
  template< class Key, class Value, class Compare >
  RBTree< Key, Value, Compare >& RBTree< Key, Value, Compare >::operator=(RBTree< Key, Value, Compare >&& other)
  {
    tree_ = std::move(other.tree_);
    return (* this);
  }
  template< class Key, class Value, class Compare >
  RBTIterator< Key, Value > RBTree< Key, Value, Compare >::push(Key k, Value v)
  {
    RBTIterator< Key, Value > res (tree_.push(k, v));
    makeBalanceAfterPush(res.getPtr());
    return res;
  }
  template< class Key, class Value, class Compare >
  RBTIterator< Key, Value > RBTree< Key, Value, Compare >::get(const Key& k)
  {
    return tree_.get(k);
  }
  template< class Key, class Value, class Compare >
  RBTConstIterator< Key, Value > RBTree< Key, Value, Compare >::get(const Key& k) const
  {
    return tree_.get(k);
  }
  template< class Key, class Value, class Compare >
  Value RBTree< Key, Value, Compare >::drop(const Key& k)
  {
    std::tuple< detail::NodeBST< Key, Value >*, Value, bool > after_drop = tree_.drop(k);
    makeBalanceAfterDrop(after_drop);
    return std::get< 1 >(after_drop);
  }
  template< class Key, class Value, class Compare >
  void RBTree< Key, Value, Compare >::clear()
  {
    tree_.clear();
  }
  template< class Key, class Value, class Compare >
  template< class Predicate >
  RBTIterator< Key, Value > RBTree< Key, Value, Compare >::find(Predicate pred)
  {
    return tree_.find(pred);
  }
  template< class Key, class Value, class Compare >
  template< class Predicate >
  RBTIterator< Key, Value > RBTree< Key, Value, Compare >::rfind(Predicate pred)
  {
    return tree_.rfind(pred);
  }
  template< class Key, class Value, class Compare >
  void RBTree< Key, Value, Compare >::swap(RBTree< Key, Value, Compare >& other)
  {
    tree_.swap(other.tree_);
  }
  template< class Key, class Value, class Compare >
  size_t RBTree< Key, Value, Compare >::size() const
  {
    return tree_.size();
  }
  template< class Key, class Value, class Compare >
  RBTIterator< Key, Value > RBTree< Key, Value, Compare >::begin()
  {
    return tree_.begin();
  }
  template< class Key, class Value, class Compare >
  RBTConstIterator< Key, Value > RBTree< Key, Value, Compare >::begin() const
  {
    return tree_.begin();
  }
  template< class Key, class Value, class Compare >
  RBTIterator< Key, Value > RBTree< Key, Value, Compare >::end()
  {
    return tree_.end();
  }
  template< class Key, class Value, class Compare >
  RBTConstIterator< Key, Value > RBTree< Key, Value, Compare >::end() const
  {
    return tree_.end();
  }
  template< class Key, class Value, class Compare >
  size_t RBTree< Key, Value, Compare >::height(RBTConstIterator< Key, Value > it) const noexcept
  {
    return tree_.height(it);
  }
  template< class Key, class Value, class Compare >
  size_t RBTree< Key, Value, Compare >::height() const noexcept
  {
    return tree_.height();
  }
  template< class Key, class Value, class Compare >
  void RBTree< Key, Value, Compare >::makeBalanceAfterPush(detail::NodeBST< Key, Value >* pushed)
  {
    detail::NodeBST< Key, Value >* X = pushed;
    detail::NodeBST< Key, Value >* P = X->parent_;
    detail::NodeBST< Key, Value >* G = P ? P->parent_ : nullptr;
    detail::NodeBST< Key, Value >* U = G ? (G->left_ == P ? G->right_ : G->left_) : nullptr;
    if (P)
    {
      if (!P->is_black_ && !G)
      {
        P->is_black_ = true;
        P->black_height_ += 1;
        P->height_ = 1 + std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0);
      }
      else if (!P->is_black_ && G && G->is_black_ && U && !U->is_black_)
      {
        P->is_black_ = true;
        P->black_height_ += 1;
        P->height_ = 1 + std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0);
        U->is_black_ = true;
        U->black_height_ += 1;
        G->is_black_ = false;
        makeBalanceAfterPush(G);
      }
      else if (!P->is_black_ && G && G->is_black_ && (!U || U->is_black_))
      {
        if ((P->left_ == X) != (G->left_ == P))
        {
          if (P->left_ == X)
          {
            tree_.rotateRight(X);
          }
          else
          {
            tree_.rotateLeft(X);
          }
          std::swap(X, P);
        }
        if (P->left_ == X)
        {
          tree_.rotateRight(P);
        }
        else
        {
          tree_.rotateLeft(P);
        }
        G->is_black_ = false;
        G->black_height_ -= 1;
        G->height_ = std::max(G->left_ ? G->left_->height_ : 0, G->right_ ? G->right_->height_ : 0) + 1;
        P->is_black_ = true;
        P->black_height_ += 1;
        P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
      }
    }
  }
  template< class Key, class Value, class Compare >
  void RBTree< Key, Value, Compare >::makeBalanceAfterDrop(std::tuple< detail::NodeBST< Key, Value >*, Value, bool > dropedParent)
  {
    detail::NodeBST< Key, Value >* P = std::get< 0 >(dropedParent);
    if (!P)
    {
      return;
    }
    if (std::get< 2 >(dropedParent))
    {
      if (tree_.blackHeight(detail::BSTConstIterator< Key, Value > (P->left_)) < tree_.blackHeight(detail::BSTConstIterator< Key, Value > (P->right_)) && P->left_ && !P->left_->is_black_)
      {
        P->left_->is_black_ = true;
        P->left_->black_height_ += 1;
        P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
      }
      else if (tree_.blackHeight(detail::BSTConstIterator< Key, Value > (P->left_)) > tree_.blackHeight(detail::BSTConstIterator< Key, Value > (P->right_)) && P->right_ && !P->right_->is_black_)
      {
        P->right_->is_black_ = true;
        P->right_->black_height_ += 1;
        P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
      }
      else
      {
        detail::NodeBST< Key, Value >* B = (tree_.blackHeight(P->left_) < tree_.blackHeight(P->right_)) ? P->right_ : P->left_;
        if (B)
        {
          if (B->is_black_ && ((P->left_ == B && B->left_ && !B->left_->is_black_) || (P->right_ == B && B->right_ && !B->right_->is_black_)))
          {
            if (P->left_ == B)
            {
              tree_.rotateRight(B);
              B->is_black_ = P->is_black_;
              B->left_->is_black_ = true;
              P->is_black_ = true;
              P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
              P->black_height_ = std::max(P->left_ ? P->left_->black_height_ : 0, P->right_ ? P->right_->black_height_ : 0) + (P->is_black_ ? 1 : 0);
              B->height_ = std::max(B->left_ ? B->left_->height_ : 0, B->right_ ? B->right_->height_ : 0) + 1;
              B->black_height_ = std::max(B->left_ ? B->left_->black_height_ : 0, B->right_ ? B->right_->black_height_ : 0) + (B->is_black_ ? 1 : 0);
            }
            else
            {
              tree_.rotateLeft(B);
              B->is_black_ = P->is_black_;
              B->right_->is_black_ = true;
              P->is_black_ = true;
              P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
              P->black_height_ = std::max(P->left_ ? P->left_->black_height_ : 0, P->right_ ? P->right_->black_height_ : 0) + (P->is_black_ ? 1 : 0);
              B->height_ = std::max(B->left_ ? B->left_->height_ : 0, B->right_ ? B->right_->height_ : 0) + 1;
              B->black_height_ = std::max(B->left_ ? B->left_->black_height_ : 0, B->right_ ? B->right_->black_height_ : 0) + (B->is_black_ ? 1 : 0);
            }
          }
          else if (B->is_black_ && ((P->left_ == B && B->right_ && !B->right_->is_black_) || (P->right_ == B && B->left_ && !B->left_->is_black_)))
          {
            if (P->left_ == B)
            {
              tree_.rotateLeft(B->right_);
              B->is_black_ = false;
              B->parent_->is_black_ = true;
              B->height_ = std::max(B->left_ ? B->left_->height_ : 0, B->right_ ? B->right_->height_ : 0) + 1;
              B->black_height_ = std::max(B->left_ ? B->left_->black_height_ : 0, B->right_ ? B->right_->black_height_ : 0) + (B->is_black_ ? 1 : 0);
              B->parent_->height_ = std::max(B->parent_->left_ ? B->parent_->left_->height_ : 0, B->parent_->right_ ? B->parent_->right_->height_ : 0) + 1;
              B->parent_->black_height_ = std::max(B->parent_->left_ ? B->parent_->left_->black_height_ : 0, B->parent_->right_ ? B->parent_->right_->black_height_ : 0) + (B->parent_->is_black_ ? 1 : 0);
              makeBalanceAfterDrop(dropedParent);
            }
            else
            {
              tree_.rotateRight(B->left_);
              B->is_black_ = false;
              B->parent_->is_black_ = true;
              B->height_ = std::max(B->left_ ? B->left_->height_ : 0, B->right_ ? B->right_->height_ : 0) + 1;
              B->black_height_ = std::max(B->left_ ? B->left_->black_height_ : 0, B->right_ ? B->right_->black_height_ : 0) + (B->is_black_ ? 1 : 0);
              B->parent_->height_ = std::max(B->parent_->left_ ? B->parent_->left_->height_ : 0, B->parent_->right_ ? B->parent_->right_->height_ : 0) + 1;
              B->parent_->black_height_ = std::max(B->parent_->left_ ? B->parent_->left_->black_height_ : 0, B->parent_->right_ ? B->parent_->right_->black_height_ : 0) + (B->parent_->is_black_ ? 1 : 0);
              makeBalanceAfterDrop(dropedParent);
            }
          }
          else if (!P->is_black_ && B->is_black_ && (!B->left_ || B->left_->is_black_) && (!B->right_ || B->right_->is_black_))
          {
            B->is_black_ = false;
            B->black_height_ -= 1;
            P->is_black_ = true;
          }
          else if (P->is_black_ && !B->is_black_ && (!B->left_ || B->left_->is_black_) && (!B->right_ || B->right_->is_black_))
          {
            if (P->left_ == B)
            {
              tree_.rotateRight(B);
              B->is_black_ = true;
              B->black_height_ += 1;
              P->is_black_ = false;
              P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
              B->height_ = std::max(B->left_ ? B->left_->height_ : 0, B->right_ ? B->right_->height_ : 0) + 1;
              makeBalanceAfterDrop(dropedParent);
            }
            else
            {
              tree_.rotateLeft(B);
              B->is_black_ = true;
              B->height_ += 1;
              P->is_black_ = false;
              P->height_ = std::max(P->left_ ? P->left_->height_ : 0, P->right_ ? P->right_->height_ : 0) + 1;
              B->height_ = std::max(B->left_ ? B->left_->height_ : 0, B->right_ ? B->right_->height_ : 0) + 1;
              makeBalanceAfterDrop(dropedParent);
            }
          }
          else if (P->is_black_ && B->is_black_ && (!B->left_ || B->left_->is_black_) && (!B->right_ || B->right_->is_black_))
          {
            B->is_black_ = false;
            B->black_height_ -= 1;
            P->black_height_ -= 1;
            makeBalanceAfterDrop({P->parent_, P->data_.second, true});
          }
        }
      }
    }
  }
}

#endif
