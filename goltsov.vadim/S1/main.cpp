#include <iostream>
#include <string>
#include "mylist.hpp"
#include "functions.hpp"

int main()
{
  size_t size = 0;
  goltsov::List< std::pair< std::string, goltsov::List< unsigned long long > > > data;
  try
  {
    goltsov::getData(data, std::cin, size);
  }
  catch(const std::overflow_error& e)
  {
    goltsov::printResult(std::cout, data, size);
    std::cerr << e.what() << '\n';
    return 1;
  }
  goltsov::printResult(std::cout, data, size);
}
