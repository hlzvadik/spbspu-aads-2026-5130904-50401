#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "mylist.hpp"
#include <string>
namespace goltsov
{
  void getData(List< std::pair< std::string, List< unsigned long long > > >& data,
      std::istream& in, size_t& size);
  void push_back(size_t** sums, size_t& n, size_t a);
  std::ostream& printResult(std::ostream& a, goltsov::List< std::pair< std::string,
    goltsov::List< unsigned long long > > >& data, size_t size);
}
#endif
