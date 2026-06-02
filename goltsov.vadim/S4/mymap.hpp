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

    MapIterator< Key, Value > find(const Key&);
    MapConstIterator< Key, Value > find(const Key&) const;
    bool contains(const Key&) const;
    size_t count(const Key&) const;

    bool empty() const;
    size_t size() const;

    MapIterator< Key, Value > begin();
    MapConstIterator< Key, Value > begin() const;
    MapConstIterator< Key, Value > cbegin() const;
    MapIterator< Key, Value > end();
    MapConstIterator< Key, Value > end() const;
    MapConstIterator< Key, Value > cend() const;

    void swap(Map< Key, Value >&) noexcept;
  private:
    goltsov::RBTree< Key, Value, std::less< Key > > data_;
  };

  template< class Key, class Value >
  Value& Map< Key, Value>::operator[](const Key& k)
  {
    try
    {
      return (* data_.get(k)).second;
    }
    catch (std::logic_error& e)
    {
      return (* data_.push(k, Value {})).second;
    }
  }
  template< class Key, class Value >
  Value& Map< Key, Value >::at(const Key& k)
  {
    return (* data_.get(k)).second;
  }
  template< class Key, class Value >
  const Value& Map< Key, Value >::at(const Key& k) const
  {
    return (* data_.get(k)).second;
  }
  template< class Key, class Value >
  std::pair< MapIterator< Key, Value >, bool > Map< Key, Value >::insert(const std::pair< Key, Value >& k_v_)
  {
    try
    {
      MapIterator< Key, Value > res = data_.push(k_v_.first, k_v_.second);
      return {res, true};
    }
    catch (std::logic_error& e)
    {
      return {data_.get(), false};
    }
  }
  template< class Key, class Value >
  void Map< Key, Value >::erase(const Key& k)
  {
    data_.drop(k);
  }
  template< class Key, class Value >
  void Map< Key, Value >::clear()
  {
    data_.clear();
  }
  template< class Key, class Value >
  MapIterator<Key, Value> Map< Key, Value >::find(const Key& k)
  {
    return data_.get(k);
  }
  template< class Key, class Value >
  MapConstIterator<Key, Value> Map< Key, Value >::find(const Key&) const
  {
    return data_.get(k);
  }
  template< class Key, class Value >
  bool Map< Key, Value >::contains(const Key& k) const
  {
    try
    {
      data_.get(k);
      return true;
    }
    catch (...)
    {
      return false;
    }
  }
  template< class Key, class Value >
  size_t Map< Key, Value >::count(const Key& k) const
  {
    return (contains(k) ? 1 : 0);
  }
  template< class Key, class Value >
  MapIterator<Key, Value> Map< Key, Value >::begin()
  {
    return data_.begin();
  }
  template< class Key, class Value >
  MapConstIterator<Key, Value> Map< Key, Value >::begin() const
  {
    return data_.begin();
  }
  template< class Key, class Value >
  MapConstIterator<Key, Value> Map< Key, Value >::cbegin() const
  {
    return data_.begin();
  }
  template< class Key, class Value >
  MapIterator<Key, Value> Map< Key, Value >::end()
  {
    return data_.end();
  }
  template< class Key, class Value >
  MapConstIterator<Key, Value> Map< Key, Value >::end() const
  {
    return data_.end();
  }
  template< class Key, class Value >
  MapConstIterator<Key, Value> Map< Key, Value >::cend() const
  {
    return data_.end();
  }
  template< class Key, class Value >
  void Map< Key, Value >::swap(Map< Key, Value >& other) noexcept
  {
    data_.swap(other.data_);
  }
}

#endif
