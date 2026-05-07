#include <boost/test/unit_test.hpp>
#include "mybst.hpp"
#include <string>

struct ComparatorString
{
  bool operator()(const std::string& a, const std::string& b)
  {
    return a < b;
  }
};

using bst_si = goltsov::BSTree< std::string, int, ComparatorString >;

BOOST_AUTO_TEST_SUITE(BST_suite)

BOOST_AUTO_TEST_CASE(constructors_test)
{
  bst_si b;
  BOOST_CHECK(b.height() == 0);
  b.push("a", 1);
  b.push("b", 2);
  bst_si b2 (b);
  bst_si b3 = b2;
  BOOST_CHECK(b2.get("a") == 1 && b2.get("b") == 2 && b3.get("a") == 1 && b3.get("b") == 2);
  bst_si b4 (std::move(b2));
  bst_si b5 = std::move(b3);
  BOOST_CHECK(b4.get("a") == 1 && b4.get("b") == 2 && b5.get("a") == 1 && b5.get("b") == 2);
}

BOOST_AUTO_TEST_CASE(push_get_drop_case)
{
  bst_si b;
  b.push("a", 1);
  b.push("b", 2);
  BOOST_CHECK_THROW(b.push("a", 1), std::logic_error);
  BOOST_CHECK(b.get("a") == 1 && b.get("b") == 2);
  BOOST_CHECK_THROW(b.get("c"), std::logic_error);
  BOOST_CHECK(b.drop("a") == 1);
  BOOST_CHECK_THROW(b.drop("a"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  bst_si b;
  b.push("a", 1);
  b.push("b", 2);
  bst_si b1;
  b.swap(b1);
  BOOST_CHECK(b.height() == 0 && b1.get("a") == 1 && b1.get("b") == 2);
}

BOOST_AUTO_TEST_CASE(begin_and_test)
{
  bst_si b;
  b.push("a", 1);
  b.push("b", 2);
  const bst_si bc = b;
  goltsov::BSTIterator< std::string, int > temp = goltsov::BSTIterator< std::string, int > (nullptr);
  goltsov::BSTConstIterator< std::string, int > tempc = goltsov::BSTConstIterator< std::string, int > (nullptr);
  BOOST_CHECK((* b.begin()).second == 1 && b.end() == temp &&
    (* bc.begin()).second == 1 && bc.end() == tempc);
}

BOOST_AUTO_TEST_SUITE_END()
