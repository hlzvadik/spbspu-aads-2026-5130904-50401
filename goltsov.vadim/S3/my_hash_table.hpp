#include <cstddef>

namespace goltsov
{
  template< class Key, class Value >
  struct Node
  {
    Key key_;
    Value value_;
    bool is_valid_;
  };

  template< class Key, class Value, size_t CAPACITY >
  struct Bucket
  {
    size_t size_;
    Node< Key, Value > node_[CAPACITY];
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableIterator
  {
    HashTable < Key, Value, Hash, Equal, CAPACITY >* hash_table_;
    size_t ind_;
    size_t ind_backet_;
  public:
    Value& value();
    const Value& value() const;
    HashTableIterator< Key, Value, CAPACITY > next() const;
    bool hasNext() const;
    HashTableIterator< Key, Value, CAPACITY > prev() const;
    bool hasPrev() const;

    void operator++();
    void operator--();

    Value& operator->();
    Value& operator*();

    const Value& operator->() const;
    const Value& operator*() const;
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTableConstIterator
  {
    const HashTable< class Key, class Value, class Hash, class Equal, size_t CAPACITY >* hash_table_;
    size_t ind_;
    size_t ind_backet_;
  public:
    const Value& value() const;
    HashTableConstIterator< Key, Value, CAPACITY > next() const;
    bool hasNext() const;
    HashTableConstIterator< Key, Value, CAPACITY > prev() const;
    bool hasPrev() const;

    void operator++();
    void operator--();

    const Value& operator->() const;
    const Value& operator*() const;
  };


  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  class HashTable
  {
    size_t size_;
    Bucket< Key, Value, CAPACITY > data_[];
  public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable< Key, Value, Hash, Equal, CAPACITY >&);
    HashTable(HashTable< Key, Value, Hash, Equal, CAPACITY >&&);
    HashTable< Key, Value, Hash, Equal, CAPACITY >& operator=(const HashTable< Key, Value, Hash, Equal, CAPACITY >&);
    HashTable< Key, Value, Hash, Equal, CAPACITY >& operator=(HashTable< Key, Value, Hash, Equal, CAPACITY >&&);
    HashTable(size_t);
    void add(const Key&, const Value&);
    void add(const Key&, Value&&);
    Value drop(const Key&);
    bool has(const Key&);
    void rehash(const size_t&);

    size_t size() const;
    size_t count() const;

    Value& operator[](const Key&);
    const Value& operator[](const Key&) const;

    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > begin();
    HashTableIterator< Key, Value, Hash, Equal, CAPACITY > end();

    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > begin() const;
    HashTableConstIterator< Key, Value, Hash, Equal, CAPACITY > end() const;
  };

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator==(HashTableIterator< Key, Value, Hash, Equal, CAPACITY > lhs, HashTableIterator< Key, Value, Hash, Equal, CAPACITY > rhs);

  template< class Key, class Value, class Hash, class Equal, size_t CAPACITY >
  bool operator!=(HashTableIterator< Key, Value, Hash, Equal, CAPACITY > lhs, HashTableIterator< Key, Value, Hash, Equal, CAPACITY > rhs);
}
