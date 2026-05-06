#include <cstddef>
#include "../Common/mystack.hpp"

namespace goltsov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  struct NodeBST
  {
    std::pair< Key, Value > data_;
    NodeBST< Key, Value >* left_, * right_, * parent_;
    size_t height_;
  };

  template< class Key, class Value >
  class BSTIterator
  {
    template< class K, class V, class C >
    friend BSTree< K, V, C >;
    friend BSTree;
    Node* ptr_;
  public:
    BSTIterator();
    ~BSTIterator() = default;
    BSTIterator(const BSTIterator< Key, Value >&);
    BSTIterator(BSTIterator< Key, Value >&&);
    BSTIterator< Key, Value > operator=(const BSTIterator< Key, Value >&);
    BSTIterator< Key, Value > operator=(BSTIterator< Key, Value >&&);
    BSTIterator(Node*);

    BSTIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;

    BSTIterator< Key, Value > opertor++();

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
    template< class K, class V, class C >
    friend BSTree< K, V, C >;
    const Node* ptr_;
  public:
    BSTConstIterator();
    ~BSTConstIterator() = default;
    BSTConstIterator(const BSTConstIterator< Key, Value >&);
    BSTConstIterator(BSTConstIterator< Key, Value >&&);
    BSTConstIterator< Key, Value >& operator=(const BSTConstIterator< Key, Value >&);
    BSTConstIterator< Key, Value >& operator=(BSTConstIterator< Key, Value >&&);
    BSTConstIterator(const Node*);

    BSTConstIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    BSTConstIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;

    BSTConstIterator< Key, Value > operator++();

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator==(const BSTConstIterator< Key, Value >&) const noexcept;
    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
    friend BSTIterator< Key, Value >;
    friend BSTConstIterator< Key, Value >;
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

    void swap(BSTree< Key, Value >&);

    BSTIterator< Key, Value > begin();
    BSTConstIterator< Key, Value > begin() const;
    BSTIterator< Key, Value > end();
    BSTConstIterator< Key, Value > end() const;

    BSTConstIterator< Key, Value > rotateLeft(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateRight(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateLargeLeft(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateLargeRight(BSTConstIterator< Key, Value > it);

    size_t height(BSTConstIterator< Key, Value > it) const noexcept;
    size_t height() const noexcept;
  };
}

namespace goltsov
{
  template< class Key, class Value >
  NodeBST< Key, Value >* falLeft(NodeBST< Key, Value >* a)
  {
    while(a->left_)
    {
      a = a->left_;
    }
    return a;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree()
  {
    ptr_ = nullptr;
  }

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
  BSTree< Key, Value, Compare >::BSTree(const BSTree< Key, Value, Compare >& other)
  {
    NodeBST< Key, Value >* current = root_;
    BSTree< Key, Value, Compare > new_tree;
    new_tree.root_ = new NodeBST< Key, Value > {current->data_, nullptr, nullptr, nullptr, current->height_};
    NodeBST< Key, Value >* new_node = new_tree.root_;
    while (current)
    {
      if (current->left_ && !new_node->left_)
      {
        new_node->left_ = new NodeBST< Key, Value > {current->left_->data_, nullptr, nullptr, new_node, current->left_->height_};
        new_node = new_node->left_;
        current = current->left_;
      }
      else if (current->right_ && !new_node->right_)
      {
        new_node->right_ = new NodeBST< Key, Value > {current->right_->data_, nullptr, nullptr, new_node, current->right_->height_};
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
  BSTree< Key, Value, Compare >::BSTree(BSTree< Key, Value, Compare >&& other)
  {
    root_ = other.root_;
    other.root_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree< Key, Value, Compare >& other)
  {
    NodeBST< Key, Value >* current = root_;
    BSTree< Key, Value, Compare > new_tree;
    new_tree.root_ = new NodeBST< Key, Value > {current->data_, nullptr, nullptr, nullptr, current->height_};
    NodeBST< Key, Value >* new_node = new_tree.root_;
    while (current)
    {
      if (current->left_ && !new_node->left_)
      {
        new_node->left_ = new NodeBST< Key, Value > {current->left_->data_, nullptr, nullptr, new_node, current->left_->height_};
        new_node = new_node->left_;
        current = current->left_;
      }
      else if (current->right_ && !new_node->right_)
      {
        new_node->right_ = new NodeBST< Key, Value > {current->right_->data_, nullptr, nullptr, new_node, current->right_->height_};
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
    return (* this);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree< Key, Value, Compare >&& other)
  {
    root_ = other.root_;
    other.root_ = nullptr;
    return (* this);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(Key k, Value v)
  {
    Compare comparator;
    NodeBST< Key, Value >* current = root_;
    while (true)
    {
      if (comparator(k, current->data_.first))
      {
        if (!current->left_)
        {
          current->left_ = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, current, 1};
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
          current->right_ = new NodeBST< Key, Value > {{k, v}, nullptr, nullptr, current, 1};
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
    throw std::runtime_error("Key is not in table");
  }
  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(Key k)
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
        if (current->left)
        {
          if (current->parent_->left == current)
          {
            current->parent->left = current->left_;
          }
          else
          {
            current->parent->right_ = current->left_;
          }
          current->left_->parent_ = current->parent_;
          if (current->right_)
          {
            if (current->left_->right_)
            {
              Node< Key, Value >* rightFalLeft = falLeft(current->right_);
              rightFalLeft->left_ = current->left_->right_;
              current->left_->right_->parent_ = rightFalLeft;
            }
            current->left_->right_ = current->right_;
          }
        }
        else if (current->right_)
        {
          if (current->parent_->left == current)
          {
            current->parent_->left_ = current->right_;
          }
          else
          {
            current->parent_->right_ = current->right_;
          }
          current->right_->parent_ = current->parent_;
        }
        delete current;
        current = nullptr;
      }
    }
    throw std::runtime_error("Key is not in table");
  }
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLeft(BSTConstIterator< Key, Value > it);
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateRight(BSTConstIterator< Key, Value > it);
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeLeft(BSTConstIterator< Key, Value > it);
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeRight(BSTConstIterator< Key, Value > it);
  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(BSTConstIterator< Key, Value > it) const noexcept;
  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const noexcept;
}
