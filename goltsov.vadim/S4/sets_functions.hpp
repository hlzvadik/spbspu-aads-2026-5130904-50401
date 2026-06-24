#include <iostream>
#include <string>
#include "mybst.hpp"

namespace goltsov
{
  using bst_s_bst_is = BSTree< std::string,
    BSTree< long long, std::string, std::less< long long > >, std::less< std::string > >;
  using bst_is_iterator = BSTIterator< long long, std::string >;
  using bst_func = BSTree< std::string,
    void (*)(std::ostream&, std::istream&, bst_s_bst_is&), std::less< std::string > >;

  void readSets(std::istream& in, bst_s_bst_is& all_sets);

  void printParsing(std::ostream& out, std::istream& in, bst_s_bst_is& all_sets);
  void complementParsing(std::ostream& out, std::istream& in, bst_s_bst_is& all_sets);
  void intersectParsing(std::ostream& out, std::istream& in, bst_s_bst_is& all_sets);
  void unionParsing(std::ostream& out, std::istream& in, bst_s_bst_is& all_sets);

  void print_ds(std::ostream& out, bst_s_bst_is& all_sets, std::string dataset);
  void complement_ds(std::ostream& out, bst_s_bst_is& all_sets, std::string new_dataset,
    std::string dataset_1, std::string dataset_2);
  void intersect_ds(std::ostream& out, bst_s_bst_is& all_sets, std::string new_dataset,
    std::string dataset_1, std::string dataset_2);
  void union_ds(std::ostream& out, bst_s_bst_is& all_sets, std::string new_dataset,
    std::string dataset_1, std::string dataset_2);
}
