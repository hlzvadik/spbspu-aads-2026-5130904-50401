#include <iostream>
#include <string>
#include <fstream>
#include "mybst.hpp"
#include "sets_functions.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    return 1;
  }

  std::fstream file_in (argv[1]);

  goltsov::bst_s_bst_is all_sets;

  goltsov::readSets(file_in, all_sets);

  goltsov::bst_func functions;
  functions.push("print", goltsov::printParsing);
  functions.push("complement", goltsov::complementParsing);
  functions.push("intersect", goltsov::intersectParsing);
  functions.push("union", goltsov::unionParsing);

  while(1)
  {
    std::string command;
    if (!(std::cin >> command))
    {
      break;
    }
    try
    {
      functions.get(command)(std::cout, std::cin, all_sets);
    }
    catch (std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      while (std::cin.peek() != '\n' && std::cin.peek() != EOF)
      {
        char a;
        std::cin.get(a);
      }
    }
  }
}
