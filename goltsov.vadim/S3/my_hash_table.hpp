#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP
#include <cstddef>
#include <stdexcept>
#include <boost/uuid/detail/sha1.hpp>
#include <mylist.hpp>

namespace goltsov
{
  template< class T >
  struct Sha1Hasher
  {
    size_t operator()(const T& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(&key, sizeof(T));
      unsigned char digest[20];
      sha1.get_digest(digest);
      size_t hash = 0;
      for (size_t i = 0; i < sizeof(size_t); ++i)
      {
        hash = (hash << 8) | digest[i];
      }
      return hash;
    }
  };

  template <>
  struct Sha1Hasher< std::string >
  {
    size_t operator()(const std::string& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(key.data(), key.size());
      unsigned char digest[20];
      sha1.get_digest(digest);
      size_t hash = 0;
      for (size_t i = 0; i < sizeof(size_t); ++i)
      {
        hash = (hash << 8) | digest[i];
      }
      return hash;
    }
  };

  template< class T >
  struct Equal
  {
    bool operator()(const T& a, const T& b)
    {
      return a == b;
    }
  };
}

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

    NodeHashTable(const Key& k, const Value& v, bool valid):
      key_(k),
      value_(v),
      is_valid_(valid)
    {}
  };

  template< class Key, class Value >
  struct Bucket
  {
    NodeHashTable< Key, Value >* node_;
    size_t capacity_;
    Bucket():
      node_(new NodeHashTable< Key, Value >[1]),
      capacity_(1)
    {}
    Bucket(size_t capacity):
      node_(new NodeHashTable< Key, Value >[capacity]),
      capacity_(capacity)
    {}
    void init(size_t capacity)
    {
      delete[] node_;
      node_ = new NodeHashTable< Key, Value >[capacity];
      capacity_ = capacity;
    }
    ~Bucket()
    {
      delete[] node_;
      node_ = nullptr;
      capacity_ = 0;
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator
  {
    HashTable < Key, Value, Hash, Equal >* hash_table_;
    size_t ind_;
    size_t ind_Bucket_;
    LIter< NodeHashTable< Key, Value > > overflow_iterator_;

    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);

    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);

    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);

    template <class K, class V, class H, class E >
    friend bool operator!=(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);

    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);

    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
  public:
    HashTableIterator() noexcept;
    HashTableIterator(HashTable < Key, Value, Hash, Equal >*, size_t, size_t,
      LIter< NodeHashTable< Key, Value > >) noexcept;

    Key& key() const noexcept;
    Value& value() noexcept;
    HashTableIterator< Key, Value, Hash, Equal > next() const;
    bool hasNext() const noexcept;

    HashTableIterator< Key, Value, Hash, Equal > operator++();

    NodeHashTable< Key, Value >* operator->() const noexcept;
    NodeHashTable< Key, Value >& operator*() const noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator
  {
    const HashTable< Key, Value, Hash, Equal >* hash_table_;
    size_t ind_;
    size_t ind_Bucket_;
    mutable LCIter< NodeHashTable< Key, Value > > overflow_iterator_;

  template< class K, class V, class H, class E >
  friend bool operator==(const HashTableConstIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);

  template< class K, class V, class H, class E >
  friend bool operator!=(const HashTableConstIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);

  template< class K, class V, class H, class E >
  friend bool operator==(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);

  template< class K, class V, class H, class E >
  friend bool operator!=(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);

  template< class K, class V, class H, class E >
  friend bool operator==(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);

  template< class K, class V, class H, class E >
  friend bool operator!=(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
  public:
    HashTableConstIterator() noexcept;
    HashTableConstIterator(const HashTable < Key, Value, Hash, Equal >*, size_t, size_t,
      LCIter< NodeHashTable< Key, Value > >) noexcept;

    const Key& key() const noexcept;
    const Value& value() const noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal > next() const;
    bool hasNext() const noexcept;

    const NodeHashTable< Key, Value >* operator->() const noexcept;
    const NodeHashTable< Key, Value >& operator*() const noexcept;

    HashTableConstIterator< Key, Value, Hash, Equal > operator++();
  };


  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    size_t count_valid_;
    size_t size_;
    size_t capacity_;
    Bucket< Key, Value >* data_;
    List< NodeHashTable< Key, Value > > overflow_;

    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
    friend class HashTableIterator< Key, Value, Hash, Equal >;
  public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable< Key, Value, Hash, Equal >&);
    HashTable(HashTable< Key, Value, Hash, Equal >&&);
    HashTable< Key, Value, Hash, Equal >& operator=(const HashTable< Key, Value, Hash, Equal >&);
    HashTable< Key, Value, Hash, Equal >& operator=(HashTable< Key, Value, Hash, Equal >&&);
    HashTable(const size_t&, const size_t&);

    void swap(HashTable< Key, Value, Hash, Equal >&) noexcept;

    void add(const Key&, const Value&);
    void add(const Key&, Value&&);
    Value drop(const Key&);
    void rehash(const size_t&, const size_t&);
    void clear();

    size_t size() const noexcept;
    size_t count() const noexcept;
    size_t countValid() const noexcept;
    size_t capacity() const noexcept;

    bool has(const Key&) const noexcept;
    Value& get(const Key&) const;
    Value& operator[](const Key&);
    const Value& operator[](const Key&) const;

    HashTableIterator< Key, Value, Hash, Equal > begin();
    HashTableIterator< Key, Value, Hash, Equal > end();

    HashTableConstIterator< Key, Value, Hash, Equal > begin() const;
    HashTableConstIterator< Key, Value, Hash, Equal > end() const;
  };
}

namespace goltsov
{
  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    count_valid_(0),
    size_(1),
    capacity_(1),
    data_(new Bucket< Key, Value >[1]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
    data_ = nullptr;
    overflow_.clear();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable< Key, Value, Hash, Equal >& other):
    count_valid_(0),
    size_(1),
    capacity_(1),
    data_(new Bucket< Key, Value >[1]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {
    HashTable< Key, Value, Hash, Equal > new_table (other.size_, other.capacity_);
    for (HashTableConstIterator< Key, Value, Hash, Equal > it = other.begin(); it != other.end(); ++it)
    {
      new_table.add(it.key(), it.value());
    }
    swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable< Key, Value, Hash, Equal >&& other):
    count_valid_(0),
    size_(1),
    capacity_(1),
    data_(new Bucket< Key, Value >[1]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {
    swap(other);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
    HashTable< Key, Value, Hash, Equal >::operator=(const HashTable< Key, Value, Hash, Equal >& other)
  {
    HashTable< Key, Value, Hash, Equal > new_table (other);
    swap(new_table);
    return * this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
    HashTable< Key, Value, Hash, Equal >::operator=(HashTable< Key, Value, Hash, Equal >&& other)
  {
    count_valid_ = 0;
    size_ = 1;
    capacity_ = 1;
    delete[] data_;
    data_ = new Bucket< Key, Value >[1];
    overflow_ = List< NodeHashTable< Key, Value > >();
    swap(other);
    return * this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const size_t& size, const size_t& capacity):
    count_valid_(0),
    size_(size),
    capacity_(capacity),
    data_(new Bucket< Key, Value >[size]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {
    for (size_t i = 0; i < size; ++i)
    {
      data_[i].init(capacity);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal >& other) noexcept
  {
    std::swap(count_valid_, other.count_valid_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
    overflow_.swap(other.overflow_);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
  {
    Hash hasher;
    Equal e;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && e(data_[ind].node_[i].key_, key))
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!data_[ind].node_[i].is_valid_)
      {
        data_[ind].node_[i].key_ = key;
        data_[ind].node_[i].value_ = value;
        data_[ind].node_[i].is_valid_ = true;
        count_valid_++;
        return;
      }
    }
    LIter< NodeHashTable< Key, Value > > it_now = overflow_.begin();
    LIter< NodeHashTable< Key, Value > > it_now_prev = it_now;
    while (it_now.hasNext())
    {
      if ((*it_now).is_valid_ && e((*it_now).key_, key))
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!(*it_now).is_valid_)
      {
        (*it_now).key_ = key;
        (*it_now).value_ = value;
        (*it_now).is_valid_ = true;
        count_valid_++;
        return;
      }
      it_now = it_now.next();
      if (it_now.hasNext())
      {
        it_now_prev = it_now;
      }
    }
    overflow_.insert(it_now_prev, {key, value, true});
    count_valid_++;
    return;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, Value&& value)
  {
    Hash hasher;
    Equal e;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && e(data_[ind].node_[i].key_, key))
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!data_[ind].node_[i].is_valid_)
      {
        data_[ind].node_[i].key_ = key;
        data_[ind].node_[i].value_ = std::move(value);
        data_[ind].node_[i].is_valid_ = true;
        count_valid_++;
        return;
      }
    }
    LIter< NodeHashTable< Key, Value > > it_now = overflow_.begin();
    LIter< NodeHashTable< Key, Value > > it_now_prev = it_now;
    while (it_now.hasNext())
    {
      if ((*it_now).is_valid_ && e((*it_now).key_, key))
      {
        throw std::logic_error("Key is in table allready.");
      }
      if (!(*it_now).is_valid_)
      {
        (*it_now).key_ = key;
        (*it_now).value_ = std::move(value);
        (*it_now).is_valid_ = true;
        count_valid_++;
        return;
      }
      it_now = it_now.next();
      if (it_now.hasNext())
      {
        it_now_prev = it_now;
      }
    }
    overflow_.insert(it_now_prev, {key, std::move(value), true});
    count_valid_++;
    return;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    Hash hasher;
    Equal e;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && e(data_[ind].node_[i].key_, key))
      {
        data_[ind].node_[i].is_valid_ = false;
        count_valid_--;
        return data_[ind].node_[i].value_;
      }
    }
    LIter< NodeHashTable< Key, Value > > it_now = overflow_.begin();
    while (it_now.hasNext())
    {
      if ((*it_now).is_valid_ && e((*it_now).key_, key))
      {
        (*it_now).is_valid_ = false;
        count_valid_--;
        return (*it_now).value_;
      }
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(const size_t& new_size, const size_t& new_capacity)
  {
    HashTable< Key, Value, Hash, Equal > new_table(new_size, new_capacity);
    for (HashTableIterator< Key, Value, Hash, Equal > it = begin(); it != end(); ++it)
    {
      new_table.add((*it).key_, (*it).value_);
    }
    this->swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    if (size_ == 0)
    {
      size_ = 1;
    }
    delete[] data_;
    data_ = new Bucket< Key, Value >[size_];
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].init(capacity_);
    }
    overflow_.clear();
    count_valid_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_ * capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::count() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::countValid() const noexcept
  {
    return count_valid_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& key) const noexcept
  {
    Hash hasher;
    Equal e;
    if (size_ == 0)
    {
      return false;
    }
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && e(key, data_[ind].node_[i].key_))
      {
        return true;
      }
    }
    LCIter< NodeHashTable< Key, Value > > it = overflow_.begin();
    while (it.hasNext())
    {
      if ((*it).is_valid_ && e(key, (*it).key_))
      {
        return true;
      }
      it = it.next();
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::get(const Key& key) const
  {
    Hash hasher;
    Equal e;
    if (size_ == 0) throw std::logic_error("No such key");
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[ind].node_[i].is_valid_ && e(key, data_[ind].node_[i].key_))
      {
        return const_cast<Value&>(data_[ind].node_[i].value_);
      }
    }
    LCIter< NodeHashTable< Key, Value > > it = overflow_.begin();
    while (it.hasNext())
    {
      if ((*it).is_valid_ && e(key, (*it).key_))
      {
        return const_cast<Value&>((*it).value_);
      }
      it = it.next();
    }
    throw std::logic_error("No such key");
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    if (has(key))
    {
      return get(key);
    }
    else
    {
      add(key, Value{});
      return get(key);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key) const
  {
    return get(key);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      for (size_t j = 0; j < capacity_; ++j)
      {
        if (data_[i].node_[j].is_valid_)
        {
          return HashTableIterator< Key, Value, Hash, Equal >(this, i, j,
            LIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
    }
    LIter< NodeHashTable< Key, Value > > overflow_now = overflow_.begin();
    while(overflow_now.hasNext())
    {
      if ((* overflow_now).is_valid_)
      {
        return HashTableIterator< Key, Value, Hash, Equal >(this, size_, 0, overflow_now);
      }
      overflow_now = overflow_now.next();
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return HashTableIterator< Key, Value, Hash, Equal >(this, size_, 0, overflow_.end());
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    for (size_t i = 0; i < size_; ++i)
    {
      for (size_t j = 0; j < capacity_; ++j)
      {
        if (data_[i].node_[j].is_valid_)
        {
          return HashTableConstIterator< Key, Value, Hash, Equal >(this, i, j,
            LCIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
    }
    LCIter< NodeHashTable< Key, Value > > overflow_now = overflow_.begin();
    while(overflow_now.hasNext())
    {
      if ((* overflow_now).is_valid_)
      {
        return HashTableConstIterator< Key, Value, Hash, Equal >(this, size_, 0, overflow_now);
      }
      overflow_now = overflow_now.next();
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return HashTableConstIterator< Key, Value, Hash, Equal >(this, size_, 0, overflow_.end());
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator() noexcept:
    hash_table_(nullptr),
    ind_(0),
    ind_Bucket_(0),
    overflow_iterator_(LIter< NodeHashTable< Key, Value > > (nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(HashTable< Key, Value, Hash, Equal >* hash_table,
    size_t ind, size_t ind_Bucket, LIter< NodeHashTable< Key, Value > > overflow_it) noexcept:
    hash_table_(hash_table),
    ind_(ind),
    ind_Bucket_(ind_Bucket),
    overflow_iterator_(overflow_it)
  {}

  template< class Key, class Value, class Hash, class Equal >
  Key& HashTableIterator< Key, Value, Hash, Equal >::key() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).key_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_Bucket_].key_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTableIterator< Key, Value, Hash, Equal >::value() noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).value_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_Bucket_].value_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal > HashTableIterator< Key, Value, Hash, Equal >::next() const
  {
    if (overflow_iterator_)
    {
      LIter< NodeHashTable< Key, Value > > it = overflow_iterator_;
      while (it.hasNext())
      {
        it = it.next();
        if (it && (*it).is_valid_)
        {
          return HashTableIterator(hash_table_, hash_table_->count(), 0, it);
        }
      }
      return hash_table_->end();
    }
    else
    {
      for (size_t j = ind_Bucket_ + 1; j < hash_table_->capacity_; ++j)
      {
        if (hash_table_->data_[ind_].node_[j].is_valid_)
        {
          return HashTableIterator(hash_table_, ind_, j, LIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
      for (size_t i = ind_ + 1; i < hash_table_->size_; ++i)
      {
        for (size_t j = 0; j < hash_table_->capacity_; ++j)
        {
          if (hash_table_->data_[i].node_[j].is_valid_)
          {
            return HashTableIterator(hash_table_, i, j, LIter< NodeHashTable< Key, Value > >(nullptr));
          }
        }
      }
      LIter< NodeHashTable< Key, Value > > it = hash_table_->overflow_.begin();
      while (it.hasNext())
      {
        if ((*it).is_valid_)
        {
          return HashTableIterator(hash_table_, hash_table_->size_, 0, it);
        }
        it = it.next();
      }
      return hash_table_->end();
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  bool HashTableIterator< Key, Value, Hash, Equal >::hasNext() const noexcept
  {
    if((* this) == hash_table_->end())
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal > HashTableIterator< Key, Value, Hash, Equal >::operator++()
  {
    (* this) = this->next();
    return (* this);
  }
  template< class Key, class Value, class Hash, class Equal >
  NodeHashTable< Key, Value >* HashTableIterator< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    if (overflow_iterator_)
    {
      return &(* overflow_iterator_);
    }
    else
    {
      return &(hash_table_->data_[ind_].node_[ind_Bucket_]);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  NodeHashTable< Key, Value >& HashTableIterator< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_Bucket_];
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator() noexcept:
    hash_table_(nullptr),
    ind_(0),
    ind_Bucket_(0),
    overflow_iterator_(LCIter< NodeHashTable< Key, Value > > (nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
    const HashTable< Key, Value,Hash, Equal >* hash_table, size_t ind, size_t ind_Bucket,
    LCIter< NodeHashTable< Key, Value > > overflow_it) noexcept:
    hash_table_(hash_table),
    ind_(ind),
    ind_Bucket_(ind_Bucket),
    overflow_iterator_(overflow_it)
  {}

  template< class Key, class Value, class Hash, class Equal >
  const Key& HashTableConstIterator< Key, Value, Hash, Equal >::key() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).key_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_Bucket_].key_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTableConstIterator< Key, Value, Hash, Equal >::value() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).value_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_Bucket_].value_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal > HashTableConstIterator< Key, Value, Hash, Equal >::next() const
  {
    if (overflow_iterator_)
    {
      LCIter< NodeHashTable< Key, Value > > it = overflow_iterator_;
      while (it.hasNext())
      {
        it = it.next();
        if (it && (*it).is_valid_)
        {
          return HashTableConstIterator(hash_table_, hash_table_->count(), 0, it);
        }
      }
      return hash_table_->end();
    }
    else
    {
      for (size_t j = ind_Bucket_ + 1; j < hash_table_->capacity_; ++j)
      {
        if (hash_table_->data_[ind_].node_[j].is_valid_)
        {
          return HashTableConstIterator(hash_table_, ind_, j, LCIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
      for (size_t i = ind_ + 1; i < hash_table_->size_; ++i)
      {
        for (size_t j = 0; j < hash_table_->capacity_; ++j)
        {
          if (hash_table_->data_[i].node_[j].is_valid_)
          {
            return HashTableConstIterator(hash_table_, i, j, LCIter< NodeHashTable< Key, Value > >(nullptr));
          }
        }
      }
      LCIter< NodeHashTable< Key, Value > > it = hash_table_->overflow_.begin();
      while (it.hasNext())
      {
        if ((*it).is_valid_)
        {
          return HashTableConstIterator(hash_table_, hash_table_->count(), 0, it);
        }
        it = it.next();
      }
      return hash_table_->end();
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstIterator< Key, Value, Hash, Equal >::hasNext() const noexcept
  {
    if((* this) == hash_table_->end())
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  const NodeHashTable< Key, Value >* HashTableConstIterator< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    if (overflow_iterator_)
    {
      return &(* overflow_iterator_);
    }
    else
    {
      return &(hash_table_->data_[ind_].node_[ind_Bucket_]);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  const NodeHashTable< Key, Value >& HashTableConstIterator< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_);
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_Bucket_];
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal > HashTableConstIterator< Key, Value, Hash, Equal >::operator++()
  {
    (* this) = this->next();
    return (* this);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs)
  {
    return (lhs.hash_table_ == rhs.hash_table_ && lhs.ind_ == rhs.ind_
      && lhs.ind_Bucket_ == rhs.ind_Bucket_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs)
  {
    return !(rhs == lhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
  {
    return (lhs.hash_table_ == rhs.hash_table_ && lhs.ind_ == rhs.ind_
      && lhs.ind_Bucket_ == rhs.ind_Bucket_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
  {
    return !(rhs == lhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
  {
    return (lhs.hash_table_ == const_cast< HashTable< Key, Value, Hash, Equal >* >(rhs.hash_table_)
    && lhs.ind_ == rhs.ind_ && lhs.ind_Bucket_ == rhs.ind_Bucket_
    && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableIterator< Key, Value, Hash, Equal >& lhs, const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
  {
    return !(rhs == lhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs)
  {
    return (lhs.hash_table_ == const_cast< const HashTable< Key, Value, Hash, Equal >* >(rhs.hash_table_)
    && lhs.ind_ == rhs.ind_ && lhs.ind_Bucket_ == rhs.ind_Bucket_
    && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs)
  {
    return !(rhs == lhs);
  }

}

#endif
