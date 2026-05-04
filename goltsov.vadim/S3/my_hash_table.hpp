#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP
#include <cstddef>
#include <stdexcept>
#include <boost/uuid/detail/sha1.hpp>
#include "../Common/mylist.hpp"

namespace goltsov
{
  template< class T >
  struct Sha1Hasher
  {
    size_t operator()(const T& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(&key, sizeof(T));
      unsigned int digest[5];
      sha1.get_digest(digest);
      size_t hash = static_cast< size_t >(digest[0]);
      return hash;
    }
  };

  template <>
  struct Sha1Hasher<std::string>
  {
    size_t operator()(const std::string& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(key.data(), key.size());
      unsigned int digest[5];
      sha1.get_digest(digest);
      return static_cast<size_t>(digest[0]);
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

  template< class Key, class Value, size_t CAPACITY >
  struct Bucket
  {
    NodeHashTable< Key, Value > node_[CAPACITY];
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableIterator;

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableIterator
  {
    HashTable < Key, Value, Hash, Equal, CAPACITY >* hash_table_;
    size_t ind_;
    size_t ind_backet_;
    LIter< NodeHashTable< Key, Value > > overflow_iterator_;

    template<class K, class V, class H, class E, size_t C>
    friend bool operator==(const HashTableIterator<K,V,H,E,C>&, const HashTableIterator<K,V,H,E,C>&);

    template<class K, class V, class H, class E, size_t C>
    friend bool operator!=(const HashTableIterator<K,V,H,E,C>&, const HashTableIterator<K,V,H,E,C>&);

    template<class K, class V, class H, class E, size_t C>
    friend bool operator==(const HashTableIterator<K,V,H,E,C>&, const HashTableConstIterator<K,V,H,E,C>&);

    template<class K, class V, class H, class E, size_t C>
    friend bool operator!=(const HashTableIterator<K,V,H,E,C>&, const HashTableConstIterator<K,V,H,E,C>&);

    template<class K, class V, class H, class E, size_t C>
    friend bool operator==(const HashTableConstIterator<K,V,H,E,C>&, const HashTableIterator<K,V,H,E,C>&);

    template<class K, class V, class H, class E, size_t C>
    friend bool operator!=(const HashTableConstIterator<K,V,H,E,C>&, const HashTableIterator<K,V,H,E,C>&);
  public:
    HashTableIterator() noexcept;
    HashTableIterator(HashTable < Key, Value, Hash, Equal, CAPACITY >*, size_t, size_t, LIter< NodeHashTable< Key, Value > >) noexcept;

    Key& key() const noexcept;
    Value& value() noexcept;
    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > next() const;
    bool hasNext() const noexcept;

    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > operator++();

    NodeHashTable< Key, Value >* operator->() const noexcept;
    NodeHashTable< Key, Value >& operator*() const noexcept;
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableConstIterator
  {
    const HashTable< Key, Value, Hash, Equal, CAPACITY >* hash_table_;
    size_t ind_;
    size_t ind_backet_;
    mutable LCIter< NodeHashTable< Key, Value > > overflow_iterator_;

  template<class K, class V, class H, class E, size_t C>
  friend bool operator==(const HashTableConstIterator<K,V,H,E,C>&, const HashTableConstIterator<K,V,H,E,C>&);

  template<class K, class V, class H, class E, size_t C>
  friend bool operator!=(const HashTableConstIterator<K,V,H,E,C>&, const HashTableConstIterator<K,V,H,E,C>&);

  template<class K, class V, class H, class E, size_t C>
  friend bool operator==(const HashTableIterator<K,V,H,E,C>&, const HashTableConstIterator<K,V,H,E,C>&);

  template<class K, class V, class H, class E, size_t C>
  friend bool operator!=(const HashTableIterator<K,V,H,E,C>&, const HashTableConstIterator<K,V,H,E,C>&);

  template<class K, class V, class H, class E, size_t C>
  friend bool operator==(const HashTableConstIterator<K,V,H,E,C>&, const HashTableIterator<K,V,H,E,C>&);

  template<class K, class V, class H, class E, size_t C>
  friend bool operator!=(const HashTableConstIterator<K,V,H,E,C>&, const HashTableIterator<K,V,H,E,C>&);
  public:
    HashTableConstIterator() noexcept;
    HashTableConstIterator(const HashTable < Key, Value, Hash, Equal, CAPACITY >*, size_t, size_t, LCIter< NodeHashTable< Key, Value > >) noexcept;

    const Key& key() const noexcept;
    const Value& value() const noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > next() const;
    bool hasNext() const noexcept;

    const NodeHashTable< Key, Value >* operator->() const noexcept;
    const NodeHashTable< Key, Value >& operator*() const noexcept;

    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > operator++();
  };


  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTable
  {
    size_t count_valid_;
    size_t size_;
    Bucket< Key, Value, CAPACITY >* data_;
    List< NodeHashTable< Key, Value > > overflow_;

    friend class HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >;
    friend class HashTableIterator< Key, Value, Hash, Equal, CAPACITY >;
  public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable< Key, Value, Hash, Equal, CAPACITY >&);
    HashTable(HashTable< Key, Value, Hash, Equal, CAPACITY >&&);
    HashTable< Key, Value, Hash, Equal, CAPACITY >& operator=(const HashTable< Key, Value, Hash, Equal, CAPACITY >&);
    HashTable< Key, Value, Hash, Equal, CAPACITY >& operator=(HashTable< Key, Value, Hash, Equal, CAPACITY >&&);
    HashTable(size_t);

    void swap(HashTable< Key, Value, Hash, Equal, CAPACITY >&) noexcept;

    void add(const Key&, const Value&);
    void add(const Key&, Value&&);
    Value drop(const Key&);
    void rehash(const size_t&);
    void clear();

    size_t size() const noexcept;
    size_t count() const noexcept;
    size_t countValid() const noexcept;

    bool has(const Key&) const noexcept;
    Value& get(const Key&) const;
    Value& operator[](const Key&);
    const Value& operator[](const Key&) const;

    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > begin();
    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > end();

    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > begin() const;
    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > end() const;
  };
}

namespace goltsov
{
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable():
    count_valid_(0),
    size_(1),
    data_(new Bucket< Key, Value, CAPACITY >[1]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::~HashTable()
  {
    delete[] data_;
    data_ = nullptr;
    overflow_.clear();
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable(const HashTable< Key, Value, Hash, Equal, CAPACITY >& other):
    count_valid_(0),
    size_(1),
    data_(new Bucket< Key, Value, CAPACITY >[1]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {
    HashTable< Key, Value, Hash, Equal, CAPACITY > new_table (other.count());
    for (HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > it = other.begin(); it != other.end(); ++it)
    {
      new_table.add(it.key(), it.value());
    }
    swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable(HashTable< Key, Value, Hash, Equal, CAPACITY >&& other):
    count_valid_(0),
    size_(1),
    data_(new Bucket< Key, Value, CAPACITY >[1]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {
    swap(other);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator=(const HashTable< Key, Value, Hash, Equal, CAPACITY >& other)
  {
    HashTable< Key, Value, Hash, Equal, CAPACITY > new_table (other);
    swap(new_table);
    return * this;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator=(HashTable< Key, Value, Hash, Equal, CAPACITY >&& other)
  {
    count_valid_ = other.count_valid_;
    size_ = other.count();
    data_ = (* other.data_);
    * other.data_ = nullptr;
    other.size_ = 0;
    overflow_ = std::move(other.overflow_);
    return * this;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTable< Key, Value, Hash, Equal, CAPACITY >::HashTable(size_t size):
    count_valid_(0),
    size_(size),
    data_(new Bucket< Key, Value, CAPACITY >[size]),
    overflow_(List< NodeHashTable< Key, Value > >())
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable<Key, Value, Hash, Equal, CAPACITY >::swap(HashTable< Key, Value, Hash, Equal, CAPACITY >& other) noexcept
  {
    std::swap(count_valid_, other.count_valid_);
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
    overflow_.swap(other.overflow_);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::add(const Key& key, const Value& value)
  {
    Hash hasher;
    Equal e;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
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

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::add(const Key& key, Value&& value)
  {
    Hash hasher;
    Equal e;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
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

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value HashTable< Key, Value, Hash, Equal, CAPACITY >::drop(const Key& key)
  {
    Hash hasher;
    Equal e;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
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

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::rehash(const size_t& new_size)
  {
    HashTable< Key, Value, Hash, Equal, CAPACITY > new_table(new_size);
    for (HashTableIterator< Key, Value, Hash, Equal, CAPACITY > it = begin(); it != end(); ++it)
    {
      new_table.add((*it).key_, (*it).value_);
    }
    this->swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  void HashTable< Key, Value, Hash, Equal, CAPACITY >::clear()
  {
    if (size_ == 0)
    {
      size_ = 1;
    }
    delete[] data_;
    data_ = new Bucket< Key, Value, CAPACITY >[size_];
    overflow_.clear();
    count_valid_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  size_t HashTable< Key, Value, Hash, Equal, CAPACITY >::size() const noexcept
  {
    return size_ * CAPACITY;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  size_t HashTable< Key, Value, Hash, Equal, CAPACITY >::count() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  size_t HashTable< Key, Value, Hash, Equal, CAPACITY >::countValid() const noexcept
  {
    return count_valid_;
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool HashTable< Key, Value, Hash, Equal, CAPACITY >::has(const Key& key) const noexcept
  {
    Hash hasher;
    Equal e;
    if (size_ == 0) return false;
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
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

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value& HashTable< Key, Value, Hash, Equal, CAPACITY >::get(const Key& key) const
  {
    Hash hasher;
    Equal e;
    if (size_ == 0) throw std::logic_error("No such key");
    size_t ind = hasher(key) % size_;
    for (size_t i = 0; i < CAPACITY; ++i)
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

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value& HashTable< Key, Value, Hash, Equal, CAPACITY >::operator[](const Key& key)
  {
    try
    {
      Value& res = get(key);
      return res;
    }
    catch (...)
    {
      add(key, Value{});
      return get(key);
    }
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
          return HashTableIterator< Key, Value, Hash, Equal, CAPACITY >(this, i, j, LIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
    }
    LIter< NodeHashTable< Key, Value > > overflow_now = overflow_.begin();
    while(overflow_now.hasNext())
    {
      if ((* overflow_now).is_valid_)
      {
        return HashTableIterator< Key, Value, Hash, Equal, CAPACITY >(this, size_, 0, overflow_now);
      }
      overflow_now = overflow_now.next();
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY > HashTable< Key, Value, Hash, Equal, CAPACITY >::end()
  {
    return HashTableIterator< Key, Value, Hash, Equal, CAPACITY >(this, size_, 0, overflow_.end());
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
          return HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >(this, i, j, LCIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
    }
    LCIter< NodeHashTable< Key, Value > > overflow_now = overflow_.begin();
    while(overflow_now.hasNext())
    {
      if ((* overflow_now).is_valid_)
      {
        return HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >(this, size_, 0, overflow_now);
      }
      overflow_now = overflow_now.next();
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > HashTable< Key, Value, Hash, Equal, CAPACITY >::end() const
  {
    return HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >(this, size_, 0, overflow_.end());
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableIterator() noexcept:
    hash_table_(nullptr),
    ind_(0),
    ind_backet_(0),
    overflow_iterator_(LIter< NodeHashTable< Key, Value > > (nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableIterator(HashTable< Key, Value, Hash, Equal, CAPACITY >* hash_table, size_t ind, size_t ind_backet, LIter< NodeHashTable< Key, Value > > overflow_it) noexcept:
    hash_table_(hash_table),
    ind_(ind),
    ind_backet_(ind_backet),
    overflow_iterator_(overflow_it)
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Key& HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::key() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).key_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_].key_;
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  Value& HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::value() noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).value_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_].value_;
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY > HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::next() const
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
      for (size_t j = ind_backet_ + 1; j < CAPACITY; ++j)
      {
        if (hash_table_->data_[ind_].node_[j].is_valid_)
        {
          return HashTableIterator(hash_table_, ind_, j, LIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
      for (size_t i = ind_ + 1; i < hash_table_->count(); ++i)
      {
        for (size_t j = 0; j < CAPACITY; ++j)
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
          return HashTableIterator(hash_table_, hash_table_->count(), 0, it);
        }
        it = it.next();
      }
      return hash_table_->end();
    }
  }
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::hasNext() const noexcept
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
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableIterator< Key, Value, Hash, Equal, CAPACITY > HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::operator++()
  {
    (* this) = this->next();
    return (* this);
  }
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  NodeHashTable< Key, Value >* HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::operator->() const noexcept
  {
    if (overflow_iterator_)
    {
      return &(* overflow_iterator_);
    }
    else
    {
      return &(hash_table_->data_[ind_].node_[ind_backet_]);
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  NodeHashTable< Key, Value >& HashTableIterator< Key, Value, Hash, Equal, CAPACITY >::operator*() const noexcept
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
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableConstIterator() noexcept:
    hash_table_(nullptr),
    ind_(0),
    ind_backet_(0),
    overflow_iterator_(LCIter< NodeHashTable< Key, Value > > (nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::HashTableConstIterator(const HashTable< Key, Value, Hash, Equal, CAPACITY >* hash_table, size_t ind, size_t ind_backet, LCIter< NodeHashTable< Key, Value > > overflow_it) noexcept:
    hash_table_(hash_table),
    ind_(ind),
    ind_backet_(ind_backet),
    overflow_iterator_(overflow_it)
  {}

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const Key& HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::key() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).key_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_].key_;
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const Value& HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::value() const noexcept
  {
    if (overflow_iterator_)
    {
      return (* overflow_iterator_).value_;
    }
    else
    {
      return hash_table_->data_[ind_].node_[ind_backet_].value_;
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::next() const
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
      for (size_t j = ind_backet_ + 1; j < CAPACITY; ++j)
      {
        if (hash_table_->data_[ind_].node_[j].is_valid_)
        {
          return HashTableConstIterator(hash_table_, ind_, j, LCIter< NodeHashTable< Key, Value > >(nullptr));
        }
      }
      for (size_t i = ind_ + 1; i < hash_table_->count(); ++i)
      {
        for (size_t j = 0; j < CAPACITY; ++j)
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
  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::hasNext() const noexcept
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

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const NodeHashTable< Key, Value >* HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::operator->() const noexcept
  {
    if (overflow_iterator_)
    {
      return &(* overflow_iterator_);
    }
    else
    {
      return &(hash_table_->data_[ind_].node_[ind_backet_]);
    }
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  const NodeHashTable< Key, Value >& HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::operator*() const noexcept
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
  HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >::operator++()
  {
    (* this) = this->next();
    return (* this);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator==(const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return (lhs.hash_table_ == rhs.hash_table_ && lhs.ind_ == rhs.ind_ && lhs.ind_backet_ == rhs.ind_backet_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator!=(const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return !(rhs == lhs);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator==(const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return (lhs.hash_table_ == rhs.hash_table_ && lhs.ind_ == rhs.ind_ && lhs.ind_backet_ == rhs.ind_backet_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator!=(const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return !(rhs == lhs);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator==(const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return (lhs.hash_table_ == const_cast< HashTable< Key, Value, Hash, Equal, CAPACITY >* >(rhs.hash_table_) && lhs.ind_ == rhs.ind_ && lhs.ind_backet_ == rhs.ind_backet_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator!=(const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return !(rhs == lhs);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator==(const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return (lhs.hash_table_ == const_cast< const HashTable< Key, Value, Hash, Equal, CAPACITY >* >(rhs.hash_table_) && lhs.ind_ == rhs.ind_ && lhs.ind_backet_ == rhs.ind_backet_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
  }

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator!=(const HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY >& lhs, const HashTableIterator< Key, Value, Hash, Equal, CAPACITY >& rhs)
  {
    return !(rhs == lhs);
  }

}

#endif
