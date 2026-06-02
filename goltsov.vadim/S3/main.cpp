#include <iostream>
#include <fstream>
#include <string>
#include "my_hash_table.hpp"
#include <myvector.hpp>
#include "graph_functions.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    return 1;
  }
  std::fstream file_in (argv[1]);

  goltsov::ht_graphs graphs (10, 10);

  goltsov::read_graphs(file_in, graphs);

  goltsov::ht_functions functions_graph(10, 4);
  functions_graph.add("graphs", goltsov::graphsParsing);
  functions_graph.add("vertexes", goltsov::vertexesParsing);
  functions_graph.add("outbound", goltsov::outboundParsing);
  functions_graph.add("inbound", goltsov::inboundParsing);
  functions_graph.add("bind", goltsov::bindParsing);
  functions_graph.add("cut", goltsov::cutParsing);
  functions_graph.add("create", goltsov::createParsing);
  functions_graph.add("merge", goltsov::mergeParsing);
  functions_graph.add("extract", goltsov::extractParsing);

  while(1)
  {
    std::string command;
    if (!(std::cin >> command))
    {
      break;
    }
    if (!functions_graph.has(command))
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
