#include "functions.hpp"
#include <iostream>
#include <limits>

namespace goltsov
{
  bool isCorrectNumber(const std::string& num)
  {
    std::string max_num = std::to_string(std::numeric_limits< unsigned long long >::max());
    if (max_num.size() < num.size())
    {
      return false;
    }
    if (max_num.size() > num.size())
    {
      return true;
    }
    return num < max_num;
  }

  unsigned long long fromStringToULL(const std::string& num)
  {
    unsigned long long result = 0;
    for (size_t i = 0; i < num.size(); ++i)
    {
      result = result * 10 + (num[i] - '0');
    }
    return result;
  }

  void getData(List< std::pair< std::string, List< unsigned long long > > >& result,
    std::istream& in, size_t& size)
  {
    LIter< std::pair< std::string, List< unsigned long long > > > i (nullptr);
    std::string name;
    while (in >> name)
    {
      size += 1;
      List< unsigned long long > numbers;
      std::string number;
      char next = in.peek();
      if (next == '\n')
      {
        i = result.insert(i, {name, numbers});
        continue;
      }
      if (next == EOF)
      {
        i = result.insert(i, {name, numbers});
        break;
      }
      LIter< unsigned long long > j (nullptr);
      while (in >> number)
      {
        if (!isCorrectNumber(number))
        {
          i = result.insert(i, {name, numbers});
          throw std::overflow_error("The number is too big");
        }
        size_t real_number = fromStringToULL(number);
        j = numbers.insert(j, real_number);
        char next = in.peek();
        if (next == '\n' || next == EOF)
        {
          break;
        }
      }
      if (!in.eof())
      {
        in.clear();
      }
      i = result.insert(i, {name, numbers});
    }
  }

  void push_back(int** sums, size_t& n, size_t a)
  {
    int* new_sums = new int[n + 1];
    for (size_t i = 0; i < n; ++i)
    {
      new_sums[i] = sums[0][i];
    }
    new_sums[n] = a;
    delete[] (* sums);
    sums[0] = new_sums;
    n += 1;
  }

  std::ostream& printResult(std::ostream& out, List< std::pair< std::string, List< unsigned long long > > >& data, size_t size)
  {
    LIter< std::pair< std::string, List< unsigned long long > > > it = data.begin();
    int* sums = nullptr;
    size_t n = 0;
    LIter< unsigned long long >* its = new LIter< unsigned long long >[size];
    for (size_t i = 0; i < size; ++i)
    {
      if (i != size - 1)
      {
        out << (* it).first << ' ';
      }
      else
      {
        out << (* it).first << '\n';
      }
      try
      {
        its[i] = (* it).second.begin();
      }
      catch(...)
      {
        delete[] its;
        delete[] sums;
        throw;
      }
      it = it.next();
    }
    it = data.begin();
    bool all = 0;
    while (!all)
    {
      all = 1;
      unsigned long long sum = 0;
      for (size_t i = 0; i < size; ++i)
      {
        if (its[i].hasNext())
        {
          if (all == 1)
          {
            out << (* its[i]);
            sum += (* its[i]);
          }
          else
          {
            out << ' ' << (* its[i]);
            sum += (* its[i]);
          }
          all = 0;
          try
          {
            its[i] = its[i].next();
          }
          catch(...)
          {
            delete[] its;
            delete[] sums;
            throw;
          }
        }
      }
      if (!all)
      {
        try
        {
          push_back(& sums, n, sum);
        }
        catch(...)
        {
          delete[] sums;
          delete[] its;
          throw;
        }
        out << '\n';
      }
    }
    for (size_t i = 0; i < n; ++i)
    {
      if (i != n - 1)
      {
        out << sums[i] << ' ';
      }
      else
      {
        out << sums[i] << '\n';
      }
    }
    if (n == 0)
    {
      std::cout << 0 << '\n';
    }
    delete[] its;
    delete[] sums;
    return out;
  }
}
