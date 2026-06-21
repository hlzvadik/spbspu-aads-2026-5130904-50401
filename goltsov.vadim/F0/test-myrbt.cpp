#include <boost/test/unit_test.hpp>
#include <string>
#include "myrbt.hpp"

using rbt_is = goltsov::RBTree< int, std::string, std::less< int > >;
using rbt_is_it = goltsov::RBTIterator< int, std::string >;
using rbt_is_cit = goltsov::RBTConstIterator< int, std::string >;

BOOST_AUTO_TEST_SUITE(RBTree_suite)

BOOST_AUTO_TEST_CASE(constructor_empty_test)
{
  rbt_is tree;
  BOOST_CHECK(tree.size() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.height() == 0);
  BOOST_CHECK(tree.blackHeight() == 0);
}

BOOST_AUTO_TEST_CASE(push_and_size_test)
{
  rbt_is tree;
  rbt_is_it it1 = tree.push(10, "ten");
  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK((*it1).second == "ten");
  tree.push(20, "twenty");
  BOOST_CHECK(tree.size() == 2);
  tree.push(5, "five");
  BOOST_CHECK(tree.size() == 3);
  BOOST_CHECK_THROW(tree.push(10, "duplicate"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(get_test)
{
  rbt_is tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  tree.push(5, "five");
  rbt_is_it it = tree.get(20);
  BOOST_CHECK(it->second == "twenty");
  const rbt_is const_tree = tree;
  rbt_is_cit const_it = const_tree.get(5);
  BOOST_CHECK(const_it->second == "five");
  BOOST_CHECK_THROW(tree.get(100), std::logic_error);
  BOOST_CHECK_THROW(const_tree.get(100), std::logic_error);
}

BOOST_AUTO_TEST_CASE(get_last_test)
{
  rbt_is tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  tree.push(5, "five");
  rbt_is_it it = tree.getLast();
  BOOST_CHECK(it->first == 20);
  const rbt_is const_tree = tree;
  rbt_is_cit const_it = const_tree.getLast();
  BOOST_CHECK(const_it->first == 20);
}

BOOST_AUTO_TEST_CASE(iterator_traversal_test)
{
  rbt_is tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  tree.push(5, "five");
  rbt_is_it it = tree.begin();
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->first == 5);
  ++it;
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->first == 10);
  ++it;
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->first == 20);
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  rbt_is tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  std::string dropped = tree.drop(10);
  BOOST_CHECK(dropped == "ten");
  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK_THROW(tree.get(10), std::logic_error);
  BOOST_CHECK_THROW(tree.drop(100), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  rbt_is tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  BOOST_CHECK(tree.size() == 2);
  tree.clear();
  BOOST_CHECK(tree.size() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.height() == 0);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  rbt_is tree1;
  tree1.push(10, "ten");
  rbt_is tree2;
  tree2.push(20, "twenty");
  tree2.push(30, "thirty");
  tree1.swap(tree2);
  BOOST_CHECK(tree1.size() == 2);
  BOOST_CHECK(tree2.size() == 1);
  BOOST_CHECK(tree1.begin()->first == 20);
  BOOST_CHECK(tree2.begin()->first == 10);
}

BOOST_AUTO_TEST_CASE(copy_move_test)
{
  rbt_is tree1;
  tree1.push(10, "ten");
  tree1.push(20, "twenty");
  rbt_is tree2(tree1);
  BOOST_CHECK(tree2.size() == 2);
  BOOST_CHECK(tree2.get(10)->second == "ten");
  rbt_is tree3;
  tree3 = tree1;
  BOOST_CHECK(tree3.size() == 2);
  rbt_is tree4(std::move(tree2));
  BOOST_CHECK(tree4.size() == 2);
  BOOST_CHECK(tree2.size() == 0);
  rbt_is tree5;
  tree5 = std::move(tree3);
  BOOST_CHECK(tree5.size() == 2);
  BOOST_CHECK(tree3.size() == 0);
}

BOOST_AUTO_TEST_CASE(height_properties_test)
{
  rbt_is tree;
  tree.push(10, "10");
  BOOST_CHECK(tree.height() == 1);
  BOOST_CHECK(tree.blackHeight() == 1);
  tree.push(5, "5");
  tree.push(20, "20");
  BOOST_CHECK(tree.height() >= 2);
  BOOST_CHECK(tree.blackHeight() >= 1);
  rbt_is_it it = tree.begin();
  BOOST_CHECK(tree.height(it) == 1);
  BOOST_CHECK(tree.blackHeight(it) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
