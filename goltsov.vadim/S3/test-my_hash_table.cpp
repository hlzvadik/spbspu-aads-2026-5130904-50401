#include <boost/test/unit_test.hpp>
#include "my_hash_table.hpp"

using ht_ci4 = goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char > >;
using ht_ci4_it = goltsov::HashTableIterator< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char > >;
using ht_ci4_cit = goltsov::HashTableConstIterator< char, int, goltsov::Sha1Hasher< char >, goltsov::Equal< char > >;

BOOST_AUTO_TEST_SUITE(HashTable_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  ht_ci4 t;
  BOOST_CHECK(t.size() == 1 && t.countValid() == 0 && t.count() == 1);
  t['a'] = 1;
  t['b'] = 2;
  ht_ci4 t1 (t);
  BOOST_CHECK(t1.size() == 1 && t1.countValid() == 2 && t1.count() == 1);
  ht_ci4 t2 (std::move(t));
  BOOST_CHECK(t2.size() == 1 && t2.countValid() == 2 && t2.count() == 1);
  ht_ci4 t3;
  t3['a'] = 1;
  t3['b'] = 2;
  ht_ci4 t4 = t3;
  BOOST_CHECK(t4.size() == 1 && t4.countValid() == 2 && t4.count() == 1);
  ht_ci4 t5 = std::move(t3);
  BOOST_CHECK(t5.size() == 1 && t5.countValid() == 2 && t5.count() == 1);
  ht_ci4 t6 (5, 4);
  BOOST_CHECK(t6.size() == 20 && t6.countValid() == 0 && t6.count() == 5);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  ht_ci4 t;
  t['a'] = 1;
  t['b'] = 2;
  ht_ci4 t1 (2, 2);
  t1['a'] = 1;
  t.swap(t1);
  BOOST_CHECK(t1.size() == 1 && t1.countValid() == 2 && t1.count() == 1 && t.size() == 4 && t.countValid() == 1 && t.count() == 2);
}

BOOST_AUTO_TEST_CASE(add_test)
{
  ht_ci4 t;
  t.add('a', 1);
  BOOST_CHECK(t['a'] == 1);
  BOOST_CHECK_THROW(t.add('a', 1), std::logic_error);
  int q = 2;
  t.add('b', q);
  BOOST_CHECK(t['b'] == q);
  BOOST_CHECK_THROW(t.add('b', q), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  ht_ci4 t;
  t.add('a', 1);
  t.drop('a');
  BOOST_CHECK(!t.has('a'));
  BOOST_CHECK_THROW(t.drop('a'), std::logic_error);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  ht_ci4 t;
  t['a'] = 1;
  t.rehash(4, 10);
  BOOST_CHECK(t.size() == 40 && t.count() == 4 && t['a'] == 1);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  ht_ci4 t (4, 4);
  t['a'] = 1;
  t['b'] = 2;
  t.clear();
  BOOST_CHECK(t.count() == 4 && t.countValid() == 0);
}

BOOST_AUTO_TEST_CASE(size_count_countValid_capacity_test)
{
  ht_ci4 t (4, 4);
  t['a'] = 1;
  t['b'] = 2;
  BOOST_CHECK(t.size() == 16 && t.count() == 4 && t.countValid() == 2 && t.capacity() == 4);
}

BOOST_AUTO_TEST_CASE(has_test)
{
  ht_ci4 t;
  t['a'] = 1;
  BOOST_CHECK(t.has('a') && !t.has('b'));
}

BOOST_AUTO_TEST_CASE(get_test)
{
  ht_ci4 t;
  t['a'] = 1;
  BOOST_CHECK(t.get('a') == 1);
  BOOST_CHECK_THROW(t.get('b'), std::logic_error);
}

BOOST_AUTO_TEST_CASE(opertor_square_brackets_test)
{
  ht_ci4 t;
  t['a'] = 1;
  char q = 'a';
  char q1 = 'b';
  BOOST_CHECK(t['a'] == 1 && t[q] == 1);
  BOOST_CHECK_NO_THROW(t['b'] && t[q1]);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4 t1;
  const ht_ci4 tc = t;
  const ht_ci4 tc1 = t1;
  BOOST_CHECK(t.begin().value() == 1 && t.begin().key() == 'a' && t.begin().hasNext() && t1.begin() == t1.end() && !t1.begin().hasNext());
  BOOST_CHECK(tc.begin().value() == 1 && tc.begin().key() == 'a' && tc.begin().hasNext() && tc1.begin() == tc1.end() && !tc1.begin().hasNext());
}

BOOST_AUTO_TEST_CASE(end_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4 t1;
  const ht_ci4 tc = t;
  const ht_ci4 tc1 = t1;
  BOOST_CHECK(t.begin() != t.end() && t1.begin() == t1.end());
  BOOST_CHECK(tc.begin() != t.end() && tc1.begin() == tc1.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Iterators_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.begin();
  BOOST_CHECK(i.value() == 1 && ci.value() == 1);
}

BOOST_AUTO_TEST_CASE(key_value_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.begin();
  BOOST_CHECK(i.value() == 1 && ci.value() == 1 && i.key() == 'a' && ci.key() == 'a');
}

BOOST_AUTO_TEST_CASE(next_hasNext_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.begin();
  BOOST_CHECK(i.next() == t.end() && ci.next() == t1.end());
  BOOST_CHECK(i.hasNext() && !i.next().hasNext() && ci.hasNext() && !ci.next().hasNext());
}

BOOST_AUTO_TEST_CASE(operator_plus_plus_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  ht_ci4_it i_next = i.next();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.begin();
  ht_ci4_cit ci_next = ci.next();
  BOOST_CHECK(i_next == ++i && i == t.end() && ci_next == ++ci && ci == t1.end());
}

BOOST_AUTO_TEST_CASE(dereference_operators_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.begin();
  BOOST_CHECK((* i).value_ == 1 && i->value_ == 1 && (* ci).value_ == 1 && ci->value_ == 1);
}

BOOST_AUTO_TEST_SUITE_END()
