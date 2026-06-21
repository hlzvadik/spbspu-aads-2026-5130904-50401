#include <iostream>
#include <fstream>
#include <string>
#include <myvector.hpp>
#include "my_hash_table.hpp"
#include "graph_functions.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    return 1;
  }
  std::fstream file_in (argv[1]);

  goltsov::ht_graphs graphs(10, 10);

  goltsov::read_graphs(file_in, graphs);

  goltsov::ht_functions functions_graph(10, 4);
  functions_graph.insert({"graphs", goltsov::graphsParsing});
  functions_graph.insert({"vertexes", goltsov::vertexesParsing});
  functions_graph.insert({"outbound", goltsov::outboundParsing});
  functions_graph.insert({"inbound", goltsov::inboundParsing});
  functions_graph.insert({"bind", goltsov::bindParsing});
  functions_graph.insert({"cut", goltsov::cutParsing});
  functions_graph.insert({"create", goltsov::createParsing});
  functions_graph.insert({"merge", goltsov::mergeParsing});
  functions_graph.insert({"extract", goltsov::extractParsing});

  while(1)
  {
    std::string command;
    if (!(std::cin >> command))
    {
      break;
    }
    if (!functions_graph.contains(command))
    {
      std::cout << "<INVALID COMMAND>\n";
      while (std::cin.peek() != '\n' && std::cin.peek() != EOF)
      {
        char a;
        std::cin.get(a);
      }
    }
    else
    {
      try
      {
        functions_graph[command](graphs, std::cin);
      }
      catch(...)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }

}
