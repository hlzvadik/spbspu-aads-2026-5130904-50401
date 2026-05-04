#include <iostream>
#include <boost/uuid/detail/sha1.hpp>
#include "my_hash_table.hpp"

int main()
{
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t;
  t['a'] = 1;
  t['b'] = 2;
  std::cout << t['a'] << ' ' << t['b'] << '\n';
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t1 = t;
  std::cout << t1['a'] << ' ' << t1['b'] << '\n';
}