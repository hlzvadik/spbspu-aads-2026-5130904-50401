#include <iostream>
#include <cstring>
#include <fstream>
#include "graph_functions.hpp"
#include "my_hash_table.hpp"

namespace goltsov
{

  void graphsParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
    graphs(g);
  }

  void vertexesParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    vertexes(g, graph_name);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void outboundParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_name;
    if (!(in >> graph_name >> vertexe_name))
    {
      throw std::runtime_error("Bad input");
    }
    outbound(g, graph_name, vertexe_name);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void inboundParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_name;
    if (!(in >> graph_name >> vertexe_name))
    {
      throw std::runtime_error("Bad input");
    }
    inbound(g, graph_name, vertexe_name);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void bindParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_a, vertexe_b;
    size_t weight;
    if (!(in >> graph_name >> vertexe_a >> vertexe_b >> weight))
    {
      throw std::runtime_error("Bad input");
    }
    bind(g, graph_name, vertexe_a, vertexe_b, weight);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void cutParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_a, vertexe_b;
    size_t weight;
    if (!(in >> graph_name >> vertexe_a >> vertexe_b >> weight))
    {
      throw std::runtime_error("Bad input");
    }
    cut(g, graph_name, vertexe_a, vertexe_b, weight);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void createParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    size_t count;
    if (!(in >> graph_name >> count))
    {
      throw std::runtime_error("Bad input");
    }
    topit::Vector< std::string > vertexes_names;
    for (size_t i = 0; i < count; ++i)
    {
      std::string name;
      if (!(in >> name))
      {
        throw std::runtime_error("Bad input");
      }
      vertexes_names.pushBack(name);
      while(in.peek() == ' ')
      {
        char a;
        in.get(a);
      }
      if ((in.peek() == '\n' || in.peek() == EOF) && i + 1 < count)
      {
        throw std::runtime_error("Bad input");
      }
    }
    create(g, graph_name, count, vertexes_names);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void mergeParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string new_graph;
    std::string old_graph_1, old_graph_2;
    if (!(in >> new_graph >> old_graph_1 >> old_graph_2))
    {
      throw std::runtime_error("Bad input");
    }
    merge(g, new_graph, old_graph_1, old_graph_2);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void extractParsing(ht_graphs& g, std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      throw std::runtime_error("Bad input");
    }
    std::string new_graph;
    std::string old_graph;
    size_t count;
    if (!(in >> new_graph >> old_graph >> count))
    {
      throw std::runtime_error("Bad input");
    }
    topit::Vector< std::string > vertexes_names;
    for (size_t i = 0; i < count; ++i)
    {
      std::string name;
      if (!(in >> name))
      {
        throw std::runtime_error("Bad input");
      }
      vertexes_names.pushBack(name);
      while(in.peek() == ' ')
      {
        char a;
        in.get(a);
      }
      if ((in.peek() == '\n' || in.peek() == EOF) && i + 1 < count)
      {
        throw std::runtime_error("Bad input");
      }
    }
    extract(g, new_graph, old_graph, count, vertexes_names);
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::runtime_error("Bad input");
    }
  }

  void sortStringVector(topit::Vector< std::string >& vec)
  {
    if (vec.getSize() == 0)
    {
      return;
    }
    for (size_t i = 0; i < vec.getSize() - 1; ++i)
    {
      for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
      {
        if (vec[j] > vec[j + 1])
        {
          std::string temp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = temp;
        }
      }
    }
  }

  void sortSizeTVector(topit::Vector< size_t >& vec)
  {
    if (vec.getSize() == 0)
    {
      return;
    }
    for (size_t i = 0; i < vec.getSize() - 1; ++i)
    {
      for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
      {
        if (vec[j] > vec[j + 1])
        {
          size_t temp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = temp;
        }
      }
    }
  }

  void sortPairStringSizeTVector(topit::Vector< std::pair< std::string, topit::Vector< size_t > > >& vec)
  {
    if (vec.getSize() == 0)
    {
      return;
    }
    for (size_t i = 0; i < vec.getSize() - 1; ++i)
    {
      for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
      {
        if (vec[j].first > vec[j + 1].first)
        {
          std::pair< std::string, topit::Vector< size_t > > temp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = temp;
        }
      }
    }
    for (size_t i = 0; i < vec.getSize(); ++i)
    {
      sortSizeTVector(vec[i].second);
    }
  }

  void read_graphs(std::istream& in, ht_graphs& graphs)
  {
    std::string name_graph;
    size_t count_vertexes;
    while(in >> name_graph >> count_vertexes)
    {
      graphs[name_graph];
      for (size_t i = 0; i < count_vertexes; ++i)
      {
        std::string name_vertex1, name_vertex2;
        size_t weight;
        if (!(in >> name_vertex1 >> name_vertex2 >> weight))
        {
          throw std::runtime_error("Bad input file");
        }
        graphs[name_graph][name_vertex1][name_vertex2].pushBack(weight);
        graphs[name_graph][name_vertex2];
      }
    }
  }

  void graphs(ht_graphs& graphs)
  {
    topit::Vector< std::string > name_graphs;
    for (ht_it_graphs it = graphs.begin(); it != graphs.end(); ++it)
    {
      name_graphs.pushBack(it.key());
    }
    sortStringVector(name_graphs);
    for (size_t i = 0; i < name_graphs.getSize(); ++i)
    {
      std::cout << name_graphs[i] << '\n';
    }
  }

  void vertexes(ht_graphs& graphs, std::string name_graph)
  {
    if (!graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    topit::Vector< std::string > name_vertexes;
    for (ht_it_vertexes it = graphs[name_graph].begin(); it != graphs[name_graph].end(); ++it)
    {
      name_vertexes.pushBack(it.key());
    }
    sortStringVector(name_vertexes);
    for (size_t i = 0; i < name_vertexes.getSize(); ++i)
    {
      std::cout << name_vertexes[i] << '\n';
    }
  }

  void outbound(ht_graphs& graphs, std::string name_graph, std::string name_vertex)
  {
    if (!graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs[name_graph].has(name_vertex))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > name_weight_vertexes;
    for (ht_it_vertexes_vertexes it = graphs[name_graph][name_vertex].begin();
      it != graphs[name_graph][name_vertex].end(); ++it)
    {
      name_weight_vertexes.pushBack({it.key(), it.value()});
    }
    sortPairStringSizeTVector(name_weight_vertexes);
    for (size_t i = 0; i < name_weight_vertexes.getSize(); ++i)
    {
      for (size_t j = 0; j < name_weight_vertexes[i].second.getSize(); ++j)
      {
        std::cout << name_weight_vertexes[i].first << " " << name_weight_vertexes[i].second[j] << '\n';
      }
    }
  }

  void inbound(ht_graphs& graphs, std::string name_graph, std::string name_vertex)
  {
    if (!graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs[name_graph].has(name_vertex))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    goltsov::HashTable< std::string, topit::Vector< size_t >,
      goltsov::Sha1Hasher< std::string >, EqualString > name_weight_vertexes;
    for (ht_it_vertexes it = graphs[name_graph].begin(); it != graphs[name_graph].end(); ++it)
    {
      if (it.value().has(name_vertex))
      {
        name_weight_vertexes[it.key()].pushBackRange(it.value()[name_vertex].begin(),
          it.value()[name_vertex].getSize());
      }
    }
    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > res;
    for (goltsov::HashTableIterator< std::string, topit::Vector< size_t >, goltsov::Sha1Hasher< std::string >,
           EqualString > it = name_weight_vertexes.begin(); it != name_weight_vertexes.end(); ++it)
    {
      sortSizeTVector(it.value());
      res.pushBack({it.key(), it.value()});
    }
    sortPairStringSizeTVector(res);
    for (size_t i = 0; i < res.getSize(); ++i)
    {
      std::cout << res[i].first << ' ';
      for (size_t j = 0; j < res[i].second.getSize(); ++j)
      {
        std::cout << res[i].second[j];
        if (j == res[i].second.getSize() - 1)
        {
          std::cout << '\n';
        }
        else
        {
          std::cout << ' ';
        }
      }
    }
  }

  void bind(ht_graphs& graphs, std::string name_graph, std::string name_vertex1,
    std::string name_vertex2, size_t weight)
  {
    if (!graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    graphs[name_graph][name_vertex1][name_vertex2].pushBack(weight);
    graphs[name_graph][name_vertex2][name_vertex1];
  }

  void cut(ht_graphs& graphs, std::string name_graph, std::string name_vertex1,
    std::string name_vertex2, size_t weight)
  {
    if (!graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    if (!graphs[name_graph].has(name_vertex1))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    if (!graphs[name_graph][name_vertex1].has(name_vertex2))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    for (size_t i = 0; i < graphs[name_graph][name_vertex1][name_vertex2].getSize(); ++i)
    {
      if (graphs[name_graph][name_vertex1][name_vertex2][i] == weight)
      {
        graphs[name_graph][name_vertex1][name_vertex2].erase(i);
        return;
      }
    }
    std::cout << "<INVALID COMMAND>\n";
  }

  void create(ht_graphs& graphs, std::string name_graph, size_t count, topit::Vector< std::string > vertexes_names)
  {
    if (graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    graphs[name_graph];
    for (size_t i = 0; i < count; ++i)
    {
      graphs[name_graph][vertexes_names[i]];
    }
  }

  void merge(ht_graphs& graphs, std::string name_new_graph, std::string name_graph1, std::string name_graph2)
  {
    if (graphs.has(name_new_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    if (!graphs.has(name_graph1))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    if (!graphs.has(name_graph2))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    for (ht_it_vertexes it = graphs[name_graph1].begin(); it != graphs[name_graph1].end(); ++it)
    {
      for (ht_it_vertexes_vertexes jt = it.value().begin(); jt != it.value().end(); ++jt)
      {
        graphs[name_new_graph][it.key()][jt.key()].pushBackRange(
          graphs[name_graph1][it.key()][jt.key()].begin(), graphs[name_graph1][it.key()][jt.key()].getSize());
      }
    }
    for (ht_it_vertexes it = graphs[name_graph2].begin(); it != graphs[name_graph2].end(); ++it)
    {
      for (ht_it_vertexes_vertexes jt = it.value().begin(); jt != it.value().end(); ++jt)
      {
        graphs[name_new_graph][it.key()][jt.key()].pushBackRange(
          graphs[name_graph2][it.key()][jt.key()].begin(), graphs[name_graph2][it.key()][jt.key()].getSize());
      }
    }
  }

  void extract(ht_graphs& graphs, std::string name_new_graph, std::string name_old_graph,
    size_t count_vertexes, topit::Vector< std::string > vertexes)
  {
    if (graphs.has(name_new_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs.has(name_old_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    for (size_t i = 0; i < count_vertexes; ++i)
    {
      if (!graphs[name_old_graph].has(vertexes[i]))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
    }
    for (size_t i = 0; i < count_vertexes; ++i)
    {
      for (size_t j = 0; j < count_vertexes; ++j)
      {
        if (i != j)
        {
          graphs[name_new_graph][vertexes[i]][vertexes[j]].pushBackRange(
            graphs[name_old_graph][vertexes[i]][vertexes[j]].begin(),
            graphs[name_old_graph][vertexes[i]][vertexes[j]].getSize());
        }
      }
    }
  }
}
