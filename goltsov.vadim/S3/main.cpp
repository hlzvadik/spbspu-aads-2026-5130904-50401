#include <iostream>
#include <fstream>
#include <string>
#include <limits>
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
  functions_graph.insert(std::pair{"graphs", goltsov::graphsParsing});
  functions_graph.insert(std::pair{"vertexes", goltsov::vertexesParsing});
  functions_graph.insert(std::pair{"outbound", goltsov::outboundParsing});
  functions_graph.insert(std::pair{"inbound", goltsov::inboundParsing});
  functions_graph.insert(std::pair{"bind", goltsov::bindParsing});
  functions_graph.insert(std::pair{"cut", goltsov::cutParsing});
  functions_graph.insert(std::pair{"create", goltsov::createParsing});
  functions_graph.insert(std::pair{"merge", goltsov::mergeParsing});
  functions_graph.insert(std::pair{"extract", goltsov::extractParsing});

  std::string command;
  while(std::cin >> command)
  {
    if (!functions_graph.contains(command))
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
