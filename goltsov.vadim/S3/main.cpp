#include <iostream>
#include <boost/uuid/detail/sha1.hpp>
#include "my_hash_table.hpp"

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

int main()
{
  goltsov::HashTable< char, int, Sha1Hasher< char >, Equal< char >, 4 > t;
  t['a'] = 1;
  t['b'] = 2;
  std::cout << t['a'] << ' ' << t['b'] << '\n';
}