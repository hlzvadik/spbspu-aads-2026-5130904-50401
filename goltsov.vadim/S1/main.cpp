#include <iostream>
#include "mylist.hpp"

int main()
{
  goltsov::List< int > a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  goltsov::LIter< int > ai = a.begin();
  ai = ai.next();
  a.insert(ai, 10);
  ai = a.begin();
  ai = a.begin();
  ai.insert(20);
  while(ai.hasNext())
  {
    std::cout << *ai << '\n';
    ai = ai.next();
  }
  std::cout << *ai << '\n';
}
