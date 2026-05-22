#ifndef MYMAP_HPP
#define MYMAP_HPP

#include "myrbt.hpp"
#include <utility>
#include <cstddef>

namespace goltsov
{
  template< class Key, class Value >
  using MapIterator = RBTIterator< Key, Value >;
  
  template< class Key, class Value >
  using MapConstIterator = RBTConstIterator< Key, Value >;

  template< class Key, class Value >
  class Map
  {
  public:
    Map() = default;
    Map(const Map&) = default;
    Map(Map&&) noexcept = default;
    ~Map() = default;

    Map& operator=(const Map&) = default;
    Map& operator=(Map&&) noexcept = default;

    Value& operator[](const Key&);
    Value& at(const Key&);
    const Value& at(const Key&) const;

    std::pair< MapIterator< Key, Value >, bool > insert(const std::pair< Key, Value >&);
    void erase(const Key&);
    void clear();

    MapIterator<Key, Value> find(const Key&);
    MapConstIterator<Key, Value> find(const Key&) const;
    bool contains(const Key&) const;
    size_t count(const Key&) const;

    bool empty() const;
    size_t size() const;

    MapIterator<Key, Value> begin();
    MapConstIterator<Key, Value> begin() const;
    MapConstIterator<Key, Value> cbegin() const;
    MapIterator<Key, Value> end();
    MapConstIterator<Key, Value> end() const;
    MapConstIterator<Key, Value> cend() const;

    void swap(Map&) noexcept;
  private:
    goltsov::RBTree< Key, Value, std::less< Key > > data_;
  };

  template< class Key, class Value >
  Value& Map< Key, Value>::operator[](const Key& k)
  {
    try
    {
      return data_.get(k);
    }
    catch (std::logic_error& e)
    {
      return (* data_.push(k, Value {})).second;
    }
  }
  template< class Key, class Value >
  Value& Map< Key, Value >::at(const Key& k)
  {
    return data_.get(k);
  }
  template< class Key, class Value >
  const Value& Map< Key, Value >::at(const Key&) const
  {
    return data_.get(k);
  }
}

#endif