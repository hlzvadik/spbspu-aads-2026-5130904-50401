#include <iostream>
#include <string>
#include "mybst.hpp"

struct ComparatorString
{
  bool operator()(const std::string& a, const std::string& b)
  {
    return a < b;
  }
};

using bst_si = goltsov::BSTree< std::string, int, ComparatorString >;

int main()
{
  bst_si b;
  b.push("a", 1);
  b.push("b", 2);
  bst_si b2 (b);
  bst_si b3 = b2;
  std::cout << b2.get("a") << b2.get("b") << b3.get("a") << b3.get("b") << b.drop("a") << '\n';
  b.drop("a");
}
