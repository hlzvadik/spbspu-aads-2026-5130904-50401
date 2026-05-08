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
  bst_si b1;
  b1.push("c", 3);
  b1.push("b", 2);
  b1.push("a", 1);
  std::cout << b1.height();
}
