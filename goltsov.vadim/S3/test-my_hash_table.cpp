#include <boost/test/unit_test.hpp>
#include "my_hash_table.hpp"

BOOST_AUTO_TEST_SUITE(HashTable_suite)

BOOST_AUTO_TEST_CASE(constructor_destructor_test)
{
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t;
  BOOST_CHECK(t.size() == 1 && t.countValid() == 0 && t.count() == 4);
  t['a'] = 1;
  t['b'] = 2;
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t1 (t);
  BOOST_CHECK(t1.size() == 1 && t1.countValid() == 2 && t1.count() == 4);
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t2 (std::move(t));
  BOOST_CHECK(t2.size() == 1 && t2.countValid() == 2 && t2.count() == 4);
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t3;
  t['a'] = 1;
  t['b'] = 2;
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t4 = t3;
  BOOST_CHECK(t4.size() == 1 && t4.countValid() == 2 && t4.count() == 4);
  goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t5 = std::move(t3);
  BOOST_CHECK(t5.size() == 1 && t5.countValid() == 2 && t5.count() == 4);
  {
    goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char >, 4 > t6;
    BOOST_CHECK(t.size() == 1 && t.countValid() == 0 && t.count() == 4);
  }
}

BOOST_AUTO_TEST_SUITE_END()