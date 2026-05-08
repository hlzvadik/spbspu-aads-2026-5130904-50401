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

BOOST_AUTO_TEST_CASE(rotates_test)
{
  bst_si b;
  b.push("a", 1);
  b.push("e", 2);
  b.push("g", 3);
  BOOST_CHECK_THROW(b.rotateLeft(b.begin()), std::logic_error);
  BOOST_CHECK(b.rotateLeft(b.begin().next()) == b.begin().next().next() && b.height() == 2);
  b.push("k", 4);
  b.push("i", 5);
  BOOST_CHECK_THROW(b.rotateLeft(b.begin().next().next().next()), std::logic_error);
  BOOST_CHECK(b.rotateLeft(b.begin().next().next().next().next()) ==
    (goltsov::BSTIterator< std::string, int > (nullptr)) && b.height() == 4);
  bst_si b1;
  b1.push("e", 3);
  b1.push("d", 2);
  b1.push("c", 1);
  BOOST_CHECK_THROW(b1.rotateRight(b1.begin().next().next()), std::logic_error);
  BOOST_CHECK(b1.rotateRight(b1.begin().next()) == b1.begin() &&  b1.height() == 2);
  b1.push("a", 4);
  b1.push("b", 5);
  BOOST_CHECK_THROW(b.rotateRight(b.begin().next()), std::logic_error);
  BOOST_CHECK(b.rotateRight(b.begin()) == (goltsov::BSTIterator< std::string, int > (nullptr)) && b.height() == 5);
  bst_si b2;
  b2.push("c", 3);
  b2.push("a", 1);
  b2.push("b", 2);
  BOOST_CHECK(b2.rotateLargeRight(b2.begin().next()) == (goltsov::BSTConstIterator< std::string, int > (nullptr)) && b2.height() == 2);
  bst_si b3;
  b3.push("a", 1);
  b3.push("c", 3);
  b3.push("b", 2);
  BOOST_CHECK(b3.rotateLargeLeft(b3.begin().next()) == (goltsov::BSTConstIterator< std::string, int > (nullptr)) && b3.height() == 2);
}

BOOST_AUTO_TEST_CASE(height_test)
{
  bst_si b;
  b.push("a", 1);
  b.push("e", 2);
  b.push("g", 3);
  b.push("k", 4);
  b.push("i", 5);
  BOOST_CHECK((b.height() == 5) && (b.height(b.begin().next()) == 4));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BSTIterators_suite)

using nb_si = goltsov::NodeBST< std::string, int >;
using bi_si = goltsov::BSTIterator< std::string, int >;
using bci_si = goltsov::BSTConstIterator< std::string, int >;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    nb_si* node = new goltsov::NodeBST< std::string, int > {{"a", 5}, nullptr, nullptr, nullptr, 1};
    bi_si it1, it2 (node), it3 (it2), it4 (std::move(it3)), it5 (it2);
    bci_si cit1, cit2 (node), cit3 (cit2), cit4 (std::move(cit3)), cit5 (cit2);

    BOOST_CHECK(it1.getPtr() == nullptr && cit1.getPtr() == nullptr);
    BOOST_CHECK(it2.getPtr() == node && cit2.getPtr() == node);
    BOOST_CHECK(it5.getPtr() == node && cit5.getPtr() == node);
    BOOST_CHECK(it4.getPtr() == node && cit4.getPtr() == node);
    BOOST_CHECK(it3.getPtr() == nullptr && cit3.getPtr() == nullptr);

    it1 = it2;
    cit1 = cit2;
    BOOST_CHECK(it1.getPtr() == node && cit1.getPtr() == node);

    it1 = std::move(it2);
    cit1 = std::move(cit2);
    BOOST_CHECK(it1.getPtr() == node && cit1.getPtr() == node);
    BOOST_CHECK(it2.getPtr() == nullptr && cit2.getPtr() == nullptr);

    delete node;
}

BOOST_AUTO_TEST_CASE(test_next_prev_has)
{
    nb_si* node = new nb_si {{"a", 5}, nullptr, nullptr, nullptr, 1};
    bi_si it(node);
    bci_si cit(node);

    BOOST_CHECK(it.hasNext() && cit.hasNext());
    BOOST_CHECK(!it.hasPrev() && !cit.hasPrev());
    BOOST_CHECK(!it.next().getPtr() && !cit.next().getPtr());
    BOOST_CHECK(!it.prev().getPtr() && !cit.prev().getPtr());

    bi_si it_null;
    bci_si cit_null;
    BOOST_CHECK(!it_null.hasNext() && !cit_null.hasNext());
    BOOST_CHECK_THROW(it_null.next(), std::logic_error);
    BOOST_CHECK_THROW(cit_null.next(), std::logic_error);

    delete node;
}

BOOST_AUTO_TEST_CASE(test_operators)
{
    nb_si* n1 = new nb_si {{"a", 5}, nullptr, nullptr, nullptr, 1};
    nb_si* n2 = new nb_si {{"b", 3}, nullptr, nullptr, nullptr, 1};

    bi_si it1 (n1), it2 (n1), it3 (n2);
    bci_si cit1 (n1), cit2 (n1), cit3 (n2);

    BOOST_CHECK((++it1).getPtr() == it1.getPtr());
    BOOST_CHECK((++cit1).getPtr() == cit1.getPtr());

    BOOST_CHECK(& (* it2) == it2.operator->());
    BOOST_CHECK(& (* cit2) == cit2.operator->());

    BOOST_CHECK(it1 == ++it2 && cit1 == ++cit2);
    BOOST_CHECK(it1 != it3 && cit1 != cit3);
    BOOST_CHECK(it2 == cit2 && cit2 == it2);
    BOOST_CHECK(it2 != cit3 && cit3 != it2);

    bci_si cit_from_it = it3;
    BOOST_CHECK(cit_from_it.getPtr() == n2);

    delete n1;
    delete n2;
}

BOOST_AUTO_TEST_CASE(test_getPtr)
{
    nb_si* node = new nb_si {{"a", 5}, nullptr, nullptr, nullptr, 1};
    bi_si it(node);
    bci_si cit(node);
    bi_si it2;
    bci_si cit2;

    BOOST_CHECK(it.getPtr() == node && cit.getPtr() == node);
    BOOST_CHECK(it2.getPtr() == nullptr && cit2.getPtr() == nullptr);

    delete node;
}


BOOST_AUTO_TEST_SUITE_END()
