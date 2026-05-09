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
    all_sets.push(name_set, goltsov::BSTree< long long, std::string, ComparatorString > ());
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

void goltsov::print_ds(std::ostream& out, goltsov::bst_s_bst_is& all_sets, std::string dataset)
{
  try
  {
    if (all_sets.get(dataset).height() == 0)
    {
      std::cout << "<EMPTY>\n";
      return;
    }
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::cout << dataset;
  for (goltsov::bst_is_iterator it = all_sets.get(dataset).begin(); it != all_sets.get(dataset).end(); ++it)
  {
    out << " " << (* it).first << " " << (* it).second;
  }
  out << "\n";
}


void goltsov::complement_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  all_sets.push(new_dataset, goltsov::BSTree< long long, std::string, ComparatorString > ());
  for (goltsov::bst_is_iterator it_1 = all_sets.get(dataset_1).begin(); it_1 != all_sets.get(dataset_1).end(); ++it_1)
  {
    goltsov::bst_is_iterator it_2 = all_sets.get(dataset_2).begin();
    while (it_2 != all_sets.get(dataset_2).end() && (* it_2).first < (* it_1).first)
    {
      ++it_2;
    }
    if (it_2 != all_sets.get(dataset_2).end() && (* it_2).first == (* it_1).first)
    {
      ++it_2;
    }
    else
    {
      all_sets.get(new_dataset).push((* it_1).first, (* it_1).second);
    }
  }
}

void goltsov::intersect_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  all_sets.push(new_dataset, goltsov::BSTree< long long, std::string, ComparatorString > ());
  for (goltsov::bst_is_iterator it_1 = all_sets.get(dataset_1).begin(); it_1 != all_sets.get(dataset_1).end(); ++it_1)
  {
    goltsov::bst_is_iterator it_2 = all_sets.get(dataset_2).begin();
    while (it_2 != all_sets.get(dataset_2).end() && (* it_2).first < (* it_1).first)
    {
      ++it_2;
    }
    if (it_2 != all_sets.get(dataset_2).end() && (* it_2).first == (* it_1).first)
    {
      all_sets.get(new_dataset).push((* it_1).first, (* it_1).second);
      ++it_2;
    }
  }
}

void goltsov::union_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  all_sets.push(new_dataset, goltsov::BSTree< long long, std::string, ComparatorString > ());
  for (goltsov::bst_is_iterator it_1 = all_sets.get(dataset_1).begin(); it_1 != all_sets.get(dataset_1).end(); ++it_1)
  {
    goltsov::bst_is_iterator it_2 = all_sets.get(dataset_2).begin();
    while (it_2 != all_sets.get(dataset_2).end() && (* it_2).first < (* it_1).first)
    {
      all_sets.get(new_dataset).push((* it_2).first, (* it_2).second);
      ++it_2;
    }
    if (it_2 != all_sets.get(dataset_2).end() && (* it_2).first == (* it_1).first)
    {
      all_sets.get(new_dataset).push((* it_1).first, (* it_1).second);
      ++it_2;
    }
    else
    {
      all_sets.get(new_dataset).push((* it_1).first, (* it_1).second);
    }
  }
}
