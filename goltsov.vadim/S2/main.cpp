#include <iostream>
#include "../Common/mylist.hpp"

int main()
{
  goltsov::List< int > a;
  a.insert(a.begin(), 5);
  std::cout << (* a.begin());
}