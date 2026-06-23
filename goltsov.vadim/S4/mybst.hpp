#ifndef MYBST_HPP
#define MYBST_HPP
#include <cstddef>
#include <utility>

namespace goltsov
{
  template< class Key, class Value, class Compare >
  class BSTree;
  template< class Key, class Value >
  class BSTIterator;
  template< class Key, class Value >
  class BSTConstIterator;

  namespace detail
  {
    template< class Key, class Value >
    struct NodeBST
    {
      std::pair< Key, Value > data;
      NodeBST< Key, Value >* left;
      NodeBST< Key, Value >* right;
      NodeBST< Key, Value >* parent;
      size_t height;
    };
    template< class Key, class Value >
    BSTIterator< Key, Value > makeBSTIterByPtr(NodeBST< Key, Value >* ptr);
    template< class Key, class Value >
    BSTConstIterator< Key, Value > makeBSTConstIterByPtr(NodeBST< Key, Value >* ptr);
  }

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    BSTIterator();
    BSTIterator(const BSTIterator< Key, Value >&);
    BSTIterator(BSTIterator< Key, Value >&&);
    ~BSTIterator() = default;
    BSTIterator< Key, Value > operator=(const BSTIterator< Key, Value >&);
    BSTIterator< Key, Value > operator=(BSTIterator< Key, Value >&&);
    BSTIterator< Key, Value > operator++();
    BSTIterator< Key, Value > operator++(int);
    BSTIterator< Key, Value > operator--();
    BSTIterator< Key, Value > operator--(int);
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTConstIterator< Key, Value >&) const noexcept;
    operator BSTConstIterator< Key, Value >() const;
    detail::NodeBST< Key, Value >* getPtr();
  private:
    template< class K, class V, class C >
    friend class BSTree;
    friend BSTConstIterator< Key, Value >;
    friend BSTIterator< Key, Value > detail::makeBSTIterByPtr< Key, Value >(NodeBST< Key, Value >* ptr);
    detail::NodeBST< Key, Value>* ptr_;
    BSTIterator(detail::NodeBST< Key, Value >*);
    BSTIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    BSTConstIterator();
    BSTConstIterator(const BSTConstIterator< Key, Value >&);
    BSTConstIterator(BSTConstIterator< Key, Value >&&);
    ~BSTConstIterator() = default;
    BSTConstIterator< Key, Value >& operator=(const BSTConstIterator< Key, Value >&);
    BSTConstIterator< Key, Value >& operator=(BSTConstIterator< Key, Value >&&);
    BSTConstIterator< Key, Value > operator++();
    BSTConstIterator< Key, Value > operator++(int);
    BSTConstIterator< Key, Value > operator--();
    BSTConstIterator< Key, Value > operator--(int);
    const std::pair< Key, Value >& operator*();
    const std::pair< Key, Value >* operator->();
    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTConstIterator< Key, Value >&) const noexcept;
    const detail::NodeBST< Key, Value >* getPtr();
  private:
    template< class K, class V, class C >
    friend class BSTree;
    friend class BSTIterator< Key, Value >;
    friend BSTConstIterator< Key, Value > detail::makeBSTConstIterByPtr< Key, Value >(NodeBST< Key, Value >* ptr);
    const detail::NodeBST< Key, Value>* ptr_;
    BSTConstIterator(const detail::NodeBST< Key, Value >*);
    BSTConstIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTConstIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    BSTree();
    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);
    ~BSTree();
    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >&);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&&);
    template< class K, class V >
    std::pair< BSTIterator< Key, Value >, bool > insert(std::pair< K, V >&&) noexcept;
    Value& at(const Key&);
    const Value& at(const Key&) const;
    Value& operator[](const Key&) noexcept;
    size_t erase(const Key&) noexcept;
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
  private:
    friend class BSTIterator< Key, Value >;
    friend class BSTConstIterator< Key, Value >;
    detail::NodeBST< Key, Value >* root_;
  };
}

namespace goltsov
{
  template< class Key, class Value >
  detail::NodeBST< Key, Value >* falLeft(detail::NodeBST< Key, Value >* a)
  {
    if (!a)
    {
      return nullptr;
    }
    while(a->left)
    {
      a = a->left;
    }
    return a;
  }

  template< class Key, class Value >
  detail::NodeBST< Key, Value >* falRight(detail::NodeBST< Key, Value >* a)
  {
    if (!a)
    {
      return nullptr;
    }
    while(a->right)
    {
      a = a->right;
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
    detail::NodeBST< Key, Value >* current = root_;
    while (current)
    {
      if (!current->left && !current->right)
      {
        if (!current->parent)
        {
          delete current;
          root_ = nullptr;
          current = nullptr;
        }
        else
        {
          if (current == current->parent->left)
          {
            current = current->parent;
            delete current->left;
            current->left = nullptr;
          }
          else
          {
            current = current->parent;
            delete current->right;
            current->right = nullptr;
          }
        }
      }
      else if (current->left)
      {
        current = current->left;
      }
      else if (current->right)
      {
        current = current->right;
      }
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree< Key, Value, Compare >& other):
    root_(nullptr)
  {
    detail::NodeBST< Key, Value >* current = other.root_;
    if (!current)
    {
      root_ = nullptr;
      return;
    }
    BSTree< Key, Value, Compare > new_tree;
    new_tree.root_ = new detail::NodeBST< Key, Value >
      {current->data, nullptr, nullptr, nullptr, current->height};
    detail::NodeBST< Key, Value >* new_node = new_tree.root_;
    while (current)
    {
      if (current->left && !new_node->left)
      {
        new_node->left = new detail::NodeBST< Key, Value >
          {current->left->data, nullptr, nullptr, new_node, current->left->height};
        new_node = new_node->left;
        current = current->left;
      }
      else if (current->right && !new_node->right)
      {
        new_node->right = new detail::NodeBST< Key, Value >
          {current->right->data, nullptr, nullptr, new_node, current->right->height};
        new_node = new_node->right;
        current = current->right;
      }
      else
      {
        new_node = new_node->parent;
        current = current->parent;
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
  template< class K, class V >
  std::pair< BSTIterator< Key, Value >, bool > BSTree< Key, Value, Compare >::insert(std::pair< K, V >&& k_v) noexcept
  {
    Compare comparator;
    if (!root_)
    {
      root_ = new detail::NodeBST< Key, Value > {std::forward< std::pair< K, V > >(k_v), nullptr, nullptr, nullptr, 1};
      return {BSTIterator< Key, Value >{}, false};
    }
    detail::NodeBST< Key, Value >* current = root_;
    detail::NodeBST< Key, Value >* inserted = nullptr;
    while (true)
    {
      if (comparator(k_v.first, current->data.first))
      {
        if (!current->left)
        {
          inserted = new detail::NodeBST< Key, Value > {std::forward< std::pair< K, V > >(k_v), nullptr, nullptr, current, 1};
          current->left = inserted;
          break;
        }
        else
        {
          current = current->left;
        }
      }
      else if (comparator(current->data.first, k_v.first))
      {
        if (!current->right)
        {
          inserted = new detail::NodeBST< Key, Value > {std::forward< std::pair< K, V > >(k_v), nullptr, nullptr, current, 1};
          current->right = inserted;
          break;
        }
        else
        {
          current = current->right;
        }
      }
      else
      {
        return {BSTIterator< Key, Value >(current), false};
      }
    }
    current = inserted;
    while (current)
    {
      size_t leftHeight = current->left ? current->left->height : 0;
      size_t rightHeight = current->right ? current->right->height : 0;
      current->height = 1 + std::max(leftHeight, rightHeight);
      current = current->parent;
    }
    return {BSTIterator< Key, Value >(inserted), true};
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::at(const Key& k)
  {
    return const_cast< Value& >(static_cast< const BSTree< Key, Value, Compare > >(*this).at(k));
  }
  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::at(const Key& k) const
  {
    Compare comparator;
    detail::NodeBST< Key, Value >* current = root_;
    while (current)
    {
      if (comparator(k, current->data.first))
      {
        current = current->left;
      }
      else if (comparator(current->data.first, k))
      {
        current = current->right;
      }
      else
      {
        return current->data.second;
      }
    }
    throw std::logic_error("No such key");
  }
  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::operator[](const Key& k) noexcept
  {
    try
    {
      return at(k);
    }
    catch (std::logic_error&)
    {
      return (insert(std::pair< Key, Value >{k, Value{}})).first->second;
    }
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::erase(const Key& k) noexcept
  {
    Compare comparator;
    detail::NodeBST<Key, Value>* current = root_;
    while (current)
    {
      if (comparator(k, current->data.first))
      {
        current = current->left;
      }
      else if (comparator(current->data.first, k))
      {
        current = current->right;
      }
      else
      {
        detail::NodeBST< Key, Value >* startHeightUpdate = nullptr;
        if (!current->left && !current->right)
        {
          startHeightUpdate = current->parent;
          if (current->parent && current->parent->left == current)
          {
            current->parent->left = nullptr;
          }
          else if (current->parent)
          {
            current->parent->right = nullptr;
          }
          else
          {
            root_ = nullptr;
          }
        }
        else if (current->left && !current->right)
        {
          startHeightUpdate = current->parent;
          if (current->parent && current->parent->left == current)
          {
            current->parent->left = current->left;
          }
          else if (current->parent)
          {
            current->parent->right = current->left;
          }
          else
          {
            root_ = current->left;
          }
          if (current->left)
          {
            current->left->parent = current->parent;
          }
        }
        else if (!current->left && current->right)
        {
          startHeightUpdate = current->parent;
          if (current->parent && current->parent->left == current)
          {
            current->parent->left = current->right;
          }
          else if (current->parent)
          {
            current->parent->right = current->right;
          }
          else
          {
            root_ = current->right;
          }
          if (current->right)
          {
            current->right->parent = current->parent;
          }
        }
        else
        {
          detail::NodeBST<Key, Value>* successor = falLeft(current->right);
          std::swap(current->data, successor->data);
          startHeightUpdate = successor->parent;
          if (successor->parent->left == successor)
          {
            successor->parent->left = successor->right;
          }
          else
          {
            successor->parent->right = successor->right;
          }
          if (successor->right)
          {
            successor->right->parent = successor->parent;
          }
          delete successor;
          detail::NodeBST< Key, Value >* node = startHeightUpdate;
          while (node)
          {
            size_t leftHeight = node->left ? node->left->height : 0;
            size_t rightHeight = node->right ? node->right->height : 0;
            node->height = 1 + std::max(leftHeight, rightHeight);
            node = node->parent;
          }
          return 1;
        }
        delete current;
        detail::NodeBST< Key, Value >* node = startHeightUpdate;
        while (node)
        {
          size_t leftHeight = node->left ? node->left->height : 0;
          size_t rightHeight = node->right ? node->right->height : 0;
          node->height = 1 + std::max(leftHeight, rightHeight);
          node = node->parent;
        }
        return 1;
      }
    }
    return 0;
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
    detail::NodeBST< Key, Value >* current = const_cast< detail::NodeBST< Key, Value >* >(it.ptr_);
    if (!current)
    {
      throw std::logic_error("Can not do rotate. Iterator is empty");
    }
    if (!current->parent)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    if (current == current->parent->left)
    {
      throw std::logic_error("Can not do left rotate.");
    }
    detail::NodeBST< Key, Value >* P = current->parent;
    detail::NodeBST< Key, Value >* GP = current->parent->parent;
    detail::NodeBST< Key, Value >* L = current->left;
    P->right = L;
    if (L)
    {
      L->parent = P;
    }
    current->left = P;
    P->parent = current;
    current->parent = GP;
    if (GP)
    {
      if (GP->left == P)
      {
        GP->left = current;
      }
      else
      {
        GP->right = current;
      }
    }
    else
    {
      root_ = current;
    }
    P->height = std::max(P->left ? P->left->height : 0, P->right ? P->right->height : 0) + 1;
    current->height =
      std::max(current->right ? current->right->height : 0, current->left ? current->left->height : 0) + 1;
    if (current->parent)
    {
      current->parent->height = std::max(current->parent->right ? current->parent->right->height : 0,
        current->parent->left ? current->parent->left->height : 0) + 1;
    }
    return BSTIterator< Key, Value > (current->right);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateRight(BSTConstIterator< Key, Value > it)
  {
    detail::NodeBST< Key, Value >* current = const_cast< detail::NodeBST< Key, Value >* >(it.ptr_);
    if (!current)
    {
      throw std::logic_error("Can not do rotate. Iterator is empty");
    }
    if (!current->parent)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    if (current == current->parent->right)
    {
      throw std::logic_error("Can not do right rotate.");
    }
    detail::NodeBST< Key, Value >* P = current->parent;
    detail::NodeBST< Key, Value >* GP = current->parent->parent;
    detail::NodeBST< Key, Value >* R = current->right;
    P->left = R;
    if (R)
    {
      R->parent = P;
    }
    current->right = P;
    P->parent = current;
    current->parent = GP;
    if (GP)
    {
      if (GP->left == P)
      {
        GP->left = current;
      }
      else
      {
        GP->right = current;
      }
    }
    else
    {
      root_ = current;
    }
    P->height = std::max(P->left ? P->left->height : 0, P->right ? P->right->height : 0) + 1;
    current->height =
      std::max(current->right ? current->right->height : 0, current->left ? current->left->height : 0) + 1;
    if (current->parent)
    {
      current->parent->height = std::max(current->parent->right ? current->parent->right->height : 0,
        current->parent->left ? current->parent->left->height : 0) + 1;
    }
    return BSTIterator< Key, Value > (current->left);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeLeft(BSTConstIterator< Key, Value > it)
  {
    detail::NodeBST< Key, Value >* current = const_cast< detail::NodeBST< Key, Value >* >(it.ptr_);
    if (!current->parent || !current->parent->parent)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    rotateRight(it);
    rotateLeft(it);
    return BSTIterator< Key, Value > (it.ptr_->right->left);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeRight(BSTConstIterator< Key, Value > it)
  {
    detail::NodeBST< Key, Value >* current = const_cast< detail::NodeBST< Key, Value >* >(it.ptr_);
    if (!current->parent || !current->parent->parent)
    {
      throw std::logic_error("Can not do rotate. No parent");
    }
    rotateLeft(it);
    rotateRight(it);
    return BSTIterator< Key, Value > (it.ptr_->left->right);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(BSTConstIterator< Key, Value > it) const noexcept
  {
    if (!it.ptr_)
    {
      return 0;
    }
    return it.ptr_->height;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const noexcept
  {
    if (!root_)
    {
      return 0;
    }
    return root_->height;
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
  BSTIterator< Key, Value >::BSTIterator(detail::NodeBST< Key, Value >* ptr):
    ptr_(ptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::next() const
  {
    detail::NodeBST< Key, Value >* current = ptr_;
    if (!current)
    {
      throw std::logic_error("No next");
    }
    if (current->right)
    {
      return BSTIterator< Key, Value >(falLeft(current->right));
    }
    detail::NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->left == previos)
      {
        break;
      }
      previos = current;
      current = current->parent;
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
    detail::NodeBST< Key, Value >* current = ptr_;
    if (current->left)
    {
      return BSTIterator< Key, Value >(falRight(current->left));
    }
    detail::NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->right == previos)
      {
        break;
      }
      previos = current;
      current = current->parent;
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
    (*this) = next();
    return (*this);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator< Key, Value > temp = (*this);
    (*this) = next();
    return temp;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--()
  {
    (*this) = prev();
    return (*this);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator< Key, Value > temp = (*this);
    (*this) = prev();
    return temp;
  }

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*()
  {
    return ptr_->data;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->()
  {
    return &(ptr_->data);
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
  BSTConstIterator< Key, Value >::BSTConstIterator(const detail::NodeBST< Key, Value >* ptr)
  {
    ptr_ = ptr;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::next() const
  {
    detail::NodeBST< Key, Value >* current = const_cast< detail::NodeBST< Key, Value >* >(ptr_);
    if (!current)
    {
      throw std::logic_error("No next");
    }
    if (current->right)
    {
      return BSTConstIterator< Key, Value >(falLeft(current->right));
    }
    detail::NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->left == previos)
      {
        break;
      }
      previos = current;
      current = current->parent;
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
    detail::NodeBST< Key, Value >* current = const_cast< detail::NodeBST< Key, Value >* > (ptr_);
    if (current->left)
    {
      return BSTConstIterator< Key, Value > (falRight(current->left));
    }
    detail::NodeBST< Key, Value >* previos = current;
    while(current)
    {
      if (current->right == previos)
      {
        break;
      }
      previos = current;
      current = current->parent;
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
    (*this) = next();
    return (*this);
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator++(int)
  {
    BSTConstIterator< Key, Value > temp = (*this);
    (*this) = next();
    return temp;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator--()
  {
    (*this) = prev();
    return (*this);
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator--(int)
  {
    BSTConstIterator< Key, Value > temp = (*this);
    (*this) = prev();
    return temp;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >& BSTConstIterator< Key, Value >::operator*()
  {
    return ptr_->data;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >* BSTConstIterator< Key, Value >::operator->()
  {
    return & (ptr_->data);
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
  detail::NodeBST< Key, Value >* BSTIterator< Key, Value >::getPtr()
  {
    return ptr_;
  }
  template< class Key, class Value >
  const detail::NodeBST< Key, Value >* BSTConstIterator< Key, Value >::getPtr()
  {
    return ptr_;
  }
  template< class Key, class Value >
  BSTIterator< Key, Value > detail::makeBSTIterByPtr(NodeBST< Key, Value >* ptr)
  {
    return BSTIterator< Key, Value >(ptr);
  }
  template< class Key, class Value >
  BSTConstIterator< Key, Value > detail::makeBSTConstIterByPtr(NodeBST< Key, Value >* ptr)
  {
    return BSTConstIterator< Key, Value >(ptr);
  }
}

#endif
