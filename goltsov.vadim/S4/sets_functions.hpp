#include <iostream>
#include <string>
#include "mybst.hpp"

namespace goltsov
{
  struct ComparatorString
  {
    bool operator()(const std::string& a, const std::string& b)
    {
      return a < b;
    }
  };

  using bst_s_bst_is = BSTree< std::string, BSTree< long long, std::string, ComparatorString >, ComparatorString >;
  using bst_is_iterator = BSTIterator< long long, std::string >;

  void readSets(std::istream& in, bst_s_bst_is& all_sets);

  void print_ds(std::ostream& out, bst_s_bst_is& all_sets, std::string dataset);
  void complement_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2);
  void intersect_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2);
  void union_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2);
}