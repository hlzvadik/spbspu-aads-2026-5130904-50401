#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP
#include <cstddef>
#include <../Common/mylist.hpp>

namespace goltsov
{
  template< class Key, class Value >
  struct NodeHashTable
  {
    Key key_;
    Value value_;
    bool is_valid_;

    NodeHashTable():
      is_valid_(false)
    {}
  };

  template< class Key, class Value, size_t CAPACITY >
  struct Bucket
  {
    NodeHashTable< Key, Value > node_[CAPACITY];
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableIterator
  {
    HashTable < Key, Value, Hash, Equal, CAPACITY >* hash_table_;
    size_t ind_;
    size_t ind_backet_;
    LIter< NodeHashTable< Key, Value > > overflow_iterator_;
  public:
    HashTableIterator();
    HashTableIterator(HashTable < Key, Value, Hash, Equal, CAPACITY >*, size_t, size_t, LIter< NodeHashTable< Key, Value > >);

    Key& key() const;
    Value& value();
    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > next() const;
    bool hasNext() const;

    void operator++();

    NodeHashTable< Key, Value >& operator->() const;
    NodeHashTable< Key, Value >& operator*() const;
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableConstIterator
  {
    const HashTable< class Key, class Value, class Hash, class Equal, size_t CAPACITY >* hash_table_;
    size_t ind_;
    size_t ind_backet_;
    LIter< NodeHashTable< Key, Value > > overflow_iterator_;
  public:
    HashTableConstIterator();
    HashTableConstIterator(HashTable < Key, Value, Hash, Equal, CAPACITY >*, size_t, size_t, LIter< NodeHashTable< Key, Value > >);

    const Key& key() const;
    const Value& value() const;
    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > next() const;
    bool hasNext() const;

    const NodeHashTable< Key, Value >& operator->() const;
    const NodeHashTable< Key, Value >& operator*() const;
  };


  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTable
  {
    size_t size_;
    Bucket< Key, Value, CAPACITY > data_[];
    List< NodeHashTable< Key, Value > > overflow_;
  public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable< Key, Value, Hash, Equal, CAPACITY >&);
    HashTable(HashTable< Key, Value, Hash, Equal, CAPACITY >&&);
    HashTable< Key, Value, Hash, Equal, CAPACITY >& operator=(const HashTable< Key, Value, Hash, Equal, CAPACITY >&);
    HashTable< Key, Value, Hash, Equal, CAPACITY >& operator=(HashTable< Key, Value, Hash, Equal, CAPACITY >&&);
    HashTable(size_t);

    void swap(HashTable< Key, Value, Hash, Equal, CAPACITY >&);

    void add(const Key&, const Value&);
    void add(const Key&, Value&&);
    Value drop(const Key&);
    void rehash(const size_t&);
    void clear();

    size_t size() const;
    size_t count() const;

    bool has(const Key&) const;
    Value& get(const Key&) const;
    Value& operator[](const Key&);
    const Value& operator[](const Key&) const;

    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > begin();
    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > end();

    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > begin() const;
    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > end() const;

    size_t& getSize();
    Bucket< Key, Value, CAPACITY >& getData();
    List< NodeHashTable< Key, Value > >& getOverflow();
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator==(HashTableIterator< Key, Value, Hash, Equal, CAPACITY > lhs, HashTableIterator< Key, Value, Hash, Equal, CAPACITY > rhs);

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator!=(HashTableIterator< Key, Value, Hash, Equal, CAPACITY > lhs, HashTableIterator< Key, Value, Hash, Equal, CAPACITY > rhs);
}

namespace goltsov
{
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable():
    size_(1),
    data_(new Bucket< Key, Value, CAPACITY >[1]),
    overflow(List())
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::~HashTable()
  {
    clear();
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable(const HashTable< Key, Value, Hash, Equal, CAPACITY >& other):
    size_(other.size())
  {
    HashTable< Key, Value, Hash, Equal, CAPACITY > new_table (other.size());
    for (HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > it = other.begin(); it != other.end; ++it)
    {
      add(it.key(), it.value());
    }
    swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable(HashTable< Key, Value, Hash, Equal, CAPACITY >&& other):
    size_(other.size())
  {
    data_ = other.data_;
    other.getData() = nullptr;
    other.getSize() = 0;
    overflow = std::move(other.getOverflow());
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator=(const HashTable< Key, Value, Hash, Equal, CAPACITY >& other)
  {
    size_ = other.size();
    HashTable< Key, Value, Hash, Equal, CAPACITY > new_table (other.size());
    for (HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > it = other.begin(); it != other.end; ++it)
    {
      add(it.key(), it.value());
    }
    swap(new_table);
    return * this;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator=(HashTable< Key, Value, Hash, Equal, CAPACITY >&& other)
  {
    size_ = other.size();
    data_ = other.data_;
    other.getData() = nullptr;
    other.getSize() = 0;
    overflow = std::move(other.getOverflow());
    return * this;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable(size_t size):
    size_(size),
    data_(new Bucket< Key, Value, CAPACITY >[size]),
    overflow(List())
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void swap(HashTable< Key, Value, Hash, Equal, CAPACITY >& other)
  {
    std::swap(size_, other.getSize());
    std::swap(data_, other.getData());
    overflow_.swap(other.getOverflow());
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::add(const Key& key, const Value& value)
  {
    Hash hasher;
    size_t ind = hasher.hash(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && data_[ind].node_[i].key_ == key)
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!data_[ind].node_[i].is_valid_)
      {
        data_[ind].node_[i].key_ = key;
        data_[ind].node_[i].value_ = value;
        data_[ind].node_[i].is_valid_ = true;
        return;
      }
    }
    LIter< NodeHashTable< Key, Value > > it_now = overflow.begin();
    LIter< NodeHashTable< Key, Value > > it_now_prev = it_now;
    while (it_now.hasNext())
    {
      if ((*it_now).is_valid_ && (*it_now).key_ == key)
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!(*it_now).is_valid_)
      {
        (*it_now).key_ = key;
        (*it_now).value_ = value;
        (*it_now).is_valid_ = true;
        return;
      }
      if (it_now.hasNext())
      {
        it_now_prev = it_now;
      }
    }
    overflow.insert(it_now_prev, value);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::add(const Key& key, Value&& value)
  {
    Hash hasher;
    size_t ind = hasher.hash(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && data_[ind].node_[i].key_ == key)
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!data_[ind].node_[i].is_valid_)
      {
        data_[ind].node_[i].key_ = key;
        data_[ind].node_[i].value_ = std::move(value);
        data_[ind].node_[i].is_valid_ = true;
        return;
      }
    }
    LIter< NodeHashTable< Key, Value > > it_now = overflow.begin();
    LIter< NodeHashTable< Key, Value > > it_now_prev = it_now;
    while (it_now.hasNext())
    {
      if ((*it_now).is_valid_ && (*it_now).key_ == key)
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!(*it_now).is_valid_)
      {
        (*it_now).key_ = key;
        (*it_now).value_ = std::move(value);
        (*it_now).is_valid_ = true;
        return;
      }
      if (it_now.hasNext())
      {
        it_now_prev = it_now;
      }
    }
    overflow.insert(it_now_prev, std::move(value));
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value HashTable< Key, Value, Hash, Equal, CAPACITY >::drop(const Key& key)
  {
    Hash hasher;
    size_t ind = hasher.hash(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && data_[ind].node_[i].key_ == key)
      {
        data_[ind].node_[i].is_valid_ = false;
        return;
      }
    }
    LIter< NodeHashTable< Key, Value > > it_now = overflow.begin();
    while (it_now.hasNext())
    {
      if ((*it_now).is_valid_ && (*it_now).key_ == key)
      {
        (*it_now).is_valid_ = false;
        return;
      }
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::rehash(const size_t& new_size)
  {
    Hash hasher;
    HashTable< Key, Value, Hash, Equal, CAPACITY > new_table(new_size);
    for (HashTableIterator it = begin(); it != end(); ++it)
    {
      new_table.add((*it).key_, (*it).value_);
    }
    this->swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::clear()
  {
    delete[] data_;
    overflow_.clear();
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  size_t HashTable< Key, Value, Hash, Equal, CAPACITY >::size() const
  {
    return size_ * CAPACITY;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  size_t HashTable< Key, Value, Hash, Equal, CAPACITY >::count() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool HashTable< Key, Value, Hash, Equal, CAPACITY >::has(const Key& key) const
  {
    for (HashTableIterator< Key, Value, Hash, Equal, CAPACITY > it = begin(); i != end(); ++i)
    {
      Equal e ();
      if (e(key, (* it).key_))
      {
        return true;
      }
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value& HashTable< Key, Value, Hash, Equal, CAPACITY >::get(const Key& key) const
  {
    for (HashTableIterator< Key, Value, Hash, Equal, CAPACITY > it = begin(); i != end(); ++i)
    {
      Equal e ();
      if (e(key, (* it).key_))
      {
        return (* it).value_;
      }
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator[](const Key& key)
  {
    return get(key);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const Value& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator[](const Key& key) const
  {
    return get(key);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY > HashTable< Key, Value, Hash, Equal, CAPACITY >::begin()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      for (size_t j = 0; j < CAPACITY; ++j)
      {
        if (data_[i].node_[j].is_valid_)
        {
          return HashTableIterator(this, i, j, LIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
    }
    LIter< NodeHashTable< Key, Value > > overflow_now = overflow_.begin();
    while(overflow_now.hasNext())
    {
      if ((* overflow_now).is_valid_)
      {
        return HashTableIterator(this, size_, CAPACITY, overflow_now);
      }
    }
    return HashTableIterator(this, size_, CAPACITY, overflow_now);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY > HashTable< Key, Value, Hash, Equal, CAPACITY >::end()
  {
    return HashTableIterator(this, size_, CAPACITY, overflow_now);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > HashTable< Key, Value, Hash, Equal, CAPACITY >::begin() const
  {
    for (size_t i = 0; i < size_; ++i)
    {
      for (size_t j = 0; j < CAPACITY; ++j)
      {
        if (data_[i].node_[j].is_valid_)
        {
          return HashTableIterator(this, i, j, LIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
    }
    LIter< NodeHashTable< Key, Value > > overflow_now = overflow_.begin();
    while(overflow_now.hasNext())
    {
      if ((* overflow_now).is_valid_)
      {
        return HashTableIterator(this, size_, CAPACITY, overflow_now);
      }
    }
    return HashTableIterator(this, size_, CAPACITY, overflow_now);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > HashTable< Key, Value, Hash, Equal, CAPACITY >::end() const
  {
    return HashTableIterator(this, size_, CAPACITY, overflow_now);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  size_t& HashTable< Key, Value, Hash, Equal, CAPACITY >::getSize()
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Bucket< Key, Value, CAPACITY >& HashTable< Key, Value, Hash, Equal, CAPACITY >::getData()
  {
    return data_;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  List< NodeHashTable< Key, Value > >& HashTable< Key, Value, Hash, Equal, CAPACITY >::getOverflow()
  {
    return overflow_;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableIterator():
    hash_table_(nullptr),
    ind_(0),
    ind_backet_(0),
    overflow_iterator_(LIter< NodeHashTable< Key, Value > > (nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableIterator(HashTable < Key, Value, Hash, Equal, CAPACITY >* hash_table, size_t ind, size_t ind_backet, LIter< NodeHashTable< Key, Value > > overflow_it):
    hash_table_(hash_table),
    ind_(ind),
    ind_backet_(ind_backet),
    overflow_iterator_(overflow_it)
  {}


  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value& HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::value()
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY > HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::next() const
  {
    if (overflow_iterator_)
    {
      while (overflow_iterator_.hasNext())
      {
        overflow_iterator_ = overflow_iterator_.next();
        if ((* overflow_iterator_).is_valid_)
        {
          return HashTableIterator(hash_table_, ind_, ind_backet_, overflow_iterator_);
        }
      }
      throw std::runtime_error("No next element");
    }
    else
    {
      for (size_t i = ind_; ind_ < hash_table_->size(); ++i)
      {
        for (size_t j = ind_backet_ + 1; ind_backet_ < CAPACITY; ++j)
        {
          if (hash_table_->getData()[i].node_[j].is_equal_)
          {
            return HashTableIterator(hash_table_, i, j, overflow_iterator_);
          }
        }
      }
      overflow_iterator_ = hash_table_->overflow_.begin();
      while (overflow_iterator_.hasNext())
      {
        overflow_iterator_ = overflow_iterator_.next();
        if ((* overflow_iterator_).is_valid_)
        {
          return HashTableIterator(hash_table_, ind_, ind_backet_, overflow_iterator_);
        }
      }
      throw std::runtime_error("No next element");
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::hasNext() const
  {
    try
    {
      next();
    }
    catch(...)
    {
      return false;
    }
    return true;
  }
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::operator++()
  {
    (* this) = next(* this);
  }
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  NodeHashTable< Key, Value >& HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::operator->() const
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_];
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  NodeHashTable< Key, Value >& HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::operator*() const
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_];
    }
  }







  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableConstIterator():
    hash_table_(nullptr),
    ind_(0),
    ind_backet_(0),
    overflow_iterator_(LIter< NodeHashTable< Key, Value > > (nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableConstIterator(HashTable< Key, Value, Hash, Equal, CAPACITY >* hash_table, size_t ind, size_t ind_backet, LIter< NodeHashTable< Key, Value > > overflow_it):
    hash_table_(hash_table),
    ind_(ind),
    ind_backet_(ind_backet),
    overflow_iterator_(overflow_it)
  {}


  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const Value& HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::value() const
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::next() const
  {
    if (overflow_iterator_)
    {
      while (overflow_iterator_.hasNext())
      {
        overflow_iterator_ = overflow_iterator_.next();
        if ((* overflow_iterator_).is_valid_)
        {
          return HashTableIterator(hash_table_, ind_, ind_backet_, overflow_iterator_);
        }
      }
      throw std::runtime_error("No next element");
    }
    else
    {
      for (size_t i = ind_; ind_ < hash_table_->size(); ++i)
      {
        for (size_t j = ind_backet_ + 1; ind_backet_ < CAPACITY; ++j)
        {
          if (hash_table_->getData()[i].node_[j].is_equal_)
          {
            return HashTableIterator(hash_table_, i, j, overflow_iterator_);
          }
        }
      }
      overflow_iterator_ = hash_table_->overflow_.begin();
      while (overflow_iterator_.hasNext())
      {
        overflow_iterator_ = overflow_iterator_.next();
        if ((* overflow_iterator_).is_valid_)
        {
          return HashTableIterator(hash_table_, ind_, ind_backet_, overflow_iterator_);
        }
      }
      throw std::runtime_error("No next element");
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::hasNext() const
  {
    try
    {
      next();
    }
    catch(...)
    {
      return false;
    }
    return true;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const NodeHashTable< Key, Value >& HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::operator->() const
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_];
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const NodeHashTable< Key, Value >& HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::operator*() const
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_];
    }
  }
}

#endif
