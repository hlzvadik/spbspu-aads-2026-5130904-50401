#include <iostream>
#include <string>
#include "mybst.hpp"
#include "sets_functions.hpp"

void goltsov::readSets(std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  while (true)
  {
    std::string name_set;
    if (!(in >> name_set))
    {
      break;
    }
    all_sets.push(name_set, goltsov::BSTree< long long, std::string, Comparator< long long > > ());
    while (true)
    {
      long long key;
      std::string value;
      if (in.peek() == '\n' || in.peek() == EOF)
      {
        break;
      }
      if (!(in >> key))
      {
        break;
      }
      if (in.peek() == '\n' || in.peek() == EOF)
      {
        break;
      }
      if (!(in >> value))
      {
        break;
      }
      all_sets.get(name_set).push(key, value);
      if (in.peek() == '\n' || in.peek() == EOF)
      {
        break;
      }
    }
  }
}

void skipSpaces(std::istream& in)
{
  while(in.peek() == ' ')
  {
    char a;
    in.get(a);
  }
}

bool checkEndLine(std::istream& in)
{
  if (in.peek() == '\n' || in.peek() == EOF)
  {
    return true;
  }
  return false;
}

void goltsov::printParsing(std::ostream& out, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  std::string name_dataset;
  if (!(in >> name_dataset))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (!checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  print_ds(out, all_sets, name_dataset);
}

void goltsov::complementParsing(std::ostream& out, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string new_dataset, dataset_1, dataset_2;
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> new_dataset))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> dataset_1))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> dataset_2))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (!checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  complement_ds(out, all_sets, new_dataset, dataset_1, dataset_2);
}

void goltsov::intersectParsing(std::ostream& out, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string new_dataset, dataset_1, dataset_2;
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> new_dataset))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> dataset_1))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> dataset_2))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (!checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  intersect_ds(out, all_sets, new_dataset, dataset_1, dataset_2);
}

void goltsov::unionParsing(std::ostream& out, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string new_dataset, dataset_1, dataset_2;
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> new_dataset))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> dataset_1))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  if (!(in >> dataset_2))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  skipSpaces(in);
  if (!checkEndLine(in))
  {
    out << "<INVALID COMMAND\n";
    return;
  }
  union_ds(out, all_sets, new_dataset, dataset_1, dataset_2);
}

void goltsov::print_ds(std::ostream& out, goltsov::bst_s_bst_is& all_sets, std::string dataset)
{
  try
  {
    if (all_sets.get(dataset).height() == 0)
    {
      out << "<EMPTY>\n";
      return;
    }
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  out << dataset;
  for (goltsov::bst_is_iterator it = all_sets.get(dataset).begin(); it != all_sets.get(dataset).end(); ++it)
  {
    out << " " << (* it).first << " " << (* it).second;
  }
  out << "\n";
}

void goltsov::complement_ds(
  std::ostream& out, bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  goltsov::BSTree< long long, std::string, Comparator< long long > > new_set;
  try
  {
    all_sets.get(dataset_1);
    all_sets.get(dataset_2);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  goltsov::bst_is_iterator it_1 = all_sets.get(dataset_1).begin();
  goltsov::bst_is_iterator it_2 = all_sets.get(dataset_2).begin();
  while (it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end())
  {
    if (it_1 != all_sets.get(dataset_1).end() || it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first == (* it_2).first)
    {
      ++it_1;
      ++it_2;
    }
    if ((it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first < (* it_2).first)
      || (it_1 != all_sets.get(dataset_1).end() && it_2 == all_sets.get(dataset_2).end()))
    {
      new_set.push((* it_1).first, (* it_1).second);
      ++it_1;
    }
    if ((it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first > (* it_2).first)
      || (it_1 == all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()))
    {
      ++it_2;
    }
  }
  try
  {
    all_sets.push(new_dataset, new_set);
  }
  catch (...)
  {
    all_sets.get(new_dataset) = new_set;
  }
}

void goltsov::intersect_ds(
  std::ostream& out, bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  goltsov::BSTree< long long, std::string, Comparator< long long > > new_set;
  try
  {
    all_sets.get(dataset_1);
    all_sets.get(dataset_2);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  goltsov::bst_is_iterator it_1 = all_sets.get(dataset_1).begin();
  goltsov::bst_is_iterator it_2 = all_sets.get(dataset_2).begin();
  while (it_1 != all_sets.get(dataset_1).end() || it_2 != all_sets.get(dataset_2).end())
  {
    if (it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first == (* it_2).first)
    {
      new_set.push((* it_1).first, (* it_1).second);
      ++it_1;
      ++it_2;
    }
    if ((it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first < (* it_2).first)
      || (it_1 != all_sets.get(dataset_1).end() && it_2 == all_sets.get(dataset_2).end()))
    {
      ++it_1;
    }
    if ((it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first > (* it_2).first)
      || (it_1 == all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()))
    {
      ++it_2;
    }
  }
  try
  {
    all_sets.push(new_dataset, new_set);
  }
  catch (...)
  {
    all_sets.get(new_dataset) = new_set;
  }
}

void goltsov::union_ds(
  std::ostream& out, bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  goltsov::BSTree< long long, std::string, Comparator< long long > > new_set;
  try
  {
    all_sets.get(dataset_1);
    all_sets.get(dataset_2);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  goltsov::bst_is_iterator it_1 = all_sets.get(dataset_1).begin();
  goltsov::bst_is_iterator it_2 = all_sets.get(dataset_2).begin();
  while (it_1 != all_sets.get(dataset_1).end() || it_2 != all_sets.get(dataset_2).end())
  {
    if (it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first == (* it_2).first)
    {
      new_set.push((* it_1).first, (* it_1).second);
      ++it_1;
      ++it_2;
    }
    if ((it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first < (* it_2).first)
      || (it_1 != all_sets.get(dataset_1).end() && it_2 == all_sets.get(dataset_2).end()))
    {
      new_set.push((* it_1).first, (* it_1).second);
      ++it_1;
    }
    if ((it_1 != all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()
      && (* it_1).first > (* it_2).first)
      || (it_1 == all_sets.get(dataset_1).end() && it_2 != all_sets.get(dataset_2).end()))
    {
      new_set.push((* it_2).first, (* it_2).second);
      ++it_2;
    }
  }
  try
  {
    all_sets.push(new_dataset, new_set);
  }
  catch (...)
  {
    all_sets.get(new_dataset) = new_set;
  }
}
