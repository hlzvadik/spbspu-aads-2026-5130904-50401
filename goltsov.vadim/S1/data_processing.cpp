#include "data_processing.hpp"
#include <iostream>
#include <limits>

namespace goltsov
{
  size_t sumWithCheck(size_t a, size_t b)
  {
    size_t max_size_t = std::numeric_limits< size_t >::max();
    if (a <= max_size_t - b)
    {
      return a + b;
    }
    else
    {
      throw(std::overflow_error("The sum is too big"));
    }
  }

  bool isCorrectNumber(const std::string& num)
  {
    std::string max_num = std::to_string(std::numeric_limits< size_t >::max());
    if (max_num.size() < num.size())
    {
      return false;
    }
    if (max_num.size() > num.size())
    {
      return true;
    }
    return num <= max_num;
  }

  size_t fromStringToST(const std::string& num)
  {
    size_t result = 0;
    for (size_t i = 0; i < num.size(); ++i)
    {
      result = result * 10 + (num[i] - '0');
    }
    return result;
  }

  void getData(std::istream& in, List< std::pair< std::string, List< size_t > > >& result, size_t& size)
  {
    LIter< std::pair< std::string, List< size_t > > > i (nullptr);
    std::string name;
    while (in >> name)
    {
      size += 1;
      List< size_t > numbers;
      size_t number = 0;
      char next = in.peek();
      if (next == '\n')
      {
        i = result.insert(i, {name, numbers});
        continue;
      }
      LIter< size_t > j (nullptr);
      while (in >> number && !in.eof())
      {
        j = numbers.insert(j, number);
        char next = in.peek();
        if (next == '\n')
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

  void push_back(size_t** sums, size_t& n, size_t a)
  {
    size_t* new_sums = new size_t[n + 1];
    for (size_t i = 0; i < n; ++i)
    {
      new_sums[i] = sums[0][i];
    }
    new_sums[n] = a;
    delete[] (* sums);
    sums[0] = new_sums;
    n += 1;
  }

  std::ostream& printResult(std::ostream& out, List< std::pair< std::string, List< size_t > > >& data, size_t size)
  {
    LIter< std::pair< std::string, List< size_t > > > it = data.begin();
    size_t* sums = nullptr;
    size_t n = 0;
    LIter< size_t >* its = new LIter< size_t >[size];
    try
    {
      out << (* it).first;
      its[0] = (* it).second.begin();
    }
    catch (...)
    {
      delete[] its;
      delete[] sums;
      throw;
    }
    it = it.next();
    for (size_t i = 1; i < size; ++i)
    {
      try
      {
        out << ' ' << (* it).first;
        its[i] = (* it).second.begin();
      }
      catch (...)
      {
        delete[] its;
        delete[] sums;
        throw;
      }
      it = it.next();
    }
    out << '\n';
    it = data.begin();
    bool all = 0;
    bool without_overflows = 1;
    while (!all)
    {
      all = 1;
      size_t sum = 0;
      for (size_t i = 0; i < size; ++i)
      {
        if (its[i].hasNext())
        {
          if (all == 1)
          {
            out << (* its[i]);
            try
            {
              sum = sumWithCheck(sum, (* its[i]));
            }
            catch(...)
            {
              without_overflows = 0;
            }
          }
          else
          {
            out << ' ' << (* its[i]);
            try
            {
              sum = sumWithCheck(sum, (* its[i]));
            }
            catch(...)
            {
              without_overflows = 0;
            }
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
        catch (...)
        {
          delete[] sums;
          delete[] its;
          throw;
        }
        out << '\n';
      }
    }
    if (!without_overflows)
    {
      delete[] its;
      delete[] sums;
      throw(std::overflow_error("The sum is too big"));
    }
    if (n != 0)
    {
      out << sums[0];
      for (size_t i = 1; i < n; ++i)
      {
        out << ' ' << sums[i];
      }
      out << '\n';
    }
    else
    {
      std::cout << 0 << '\n';
    }
    delete[] its;
    delete[] sums;
    return out;
  }
}
