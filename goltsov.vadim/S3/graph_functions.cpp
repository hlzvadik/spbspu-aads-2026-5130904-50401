#include <iostream>
#include <cstring>
#include <fstream>
#include "graph_functions.hpp"
#include "my_hash_table.hpp"

namespace goltsov
{
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

  void graphsParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    graphs(g);
  }

  void vertexesParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    vertexes(g, graph_name);
  }

  void outboundParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_name;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> vertexe_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    outbound(g, graph_name, vertexe_name);
  }

  void inboundParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_name;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> vertexe_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    inbound(g, graph_name, vertexe_name);
  }

  void bindParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_a, vertexe_b;
    size_t weight;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> vertexe_a))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> vertexe_b))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> weight))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    bind(g, graph_name, vertexe_a, vertexe_b, weight);
  }

  void cutParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    std::string vertexe_a, vertexe_b;
    size_t weight;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in  >> vertexe_a))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> vertexe_b))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> weight))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    cut(g, graph_name, vertexe_a, vertexe_b, weight);
  }

  void createParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string graph_name;
    size_t count;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> count))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in) && count != 0)
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
      skipSpaces(in);
      if (checkEndLine(in) && i + 1 < count)
      {
        throw std::runtime_error("Bad input");
      }
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    create(g, graph_name, count, vertexes_names);
  }

  void mergeParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string new_graph;
    std::string old_graph_1, old_graph_2;
    if (!(in >> new_graph))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> old_graph_1))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> old_graph_2))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    merge(g, new_graph, old_graph_1, old_graph_2);
  }

  void extractParsing(ht_graphs& g, std::istream& in)
  {
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    std::string new_graph;
    std::string old_graph;
    size_t count;
    if (!(in >> new_graph))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> old_graph))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    if (!(in >> count))
    {
      throw std::runtime_error("Bad input");
    }
    skipSpaces(in);
    if (checkEndLine(in) && count != 0)
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
      skipSpaces(in);
      if (checkEndLine(in) && i + 1 < count)
      {
        throw std::runtime_error("Bad input");
      }
    }
    skipSpaces(in);
    if (!checkEndLine(in))
    {
      throw std::runtime_error("Bad input");
    }
    extract(g, new_graph, old_graph, count, vertexes_names);
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
    bool has_output = false;
    topit::Vector< std::string > name_graphs;
    for (ht_it_graphs it = graphs.begin(); it != graphs.end(); ++it)
    {
      name_graphs.pushBack(it.key());
    }
    sortStringVector(name_graphs);
    for (size_t i = 0; i < name_graphs.getSize(); ++i)
    {
      has_output = true;
      std::cout << name_graphs[i] << '\n';
    }
    if (!has_output)
    {
      std::cout << '\n';
    }
  }

  void vertexes(ht_graphs& graphs, std::string name_graph)
  {
    bool has_output = false;
    if (!graphs.has(name_graph))
    {
      has_output = true;
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    topit::Vector< std::string > name_vertexes;
    for (ht_it_vertexes it = graphs[name_graph].begin(); it != graphs[name_graph].end(); ++it)
    {
      name_vertexes.pushBack(it.key());
    }
    sortStringVector(name_vertexes);
    for (size_t i = 0; i < name_vertexes.getSize(); ++i)
    {
      has_output = true;
      std::cout << name_vertexes[i] << '\n';
    }
    if (!has_output)
    {
      std::cout << '\n';
    }
  }

  void outbound(ht_graphs& graphs, std::string name_graph, std::string name_vertex)
  {
    bool has_output = false;
    if (!graphs.has(name_graph))
    {
      has_output = true;
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs[name_graph].has(name_vertex))
    {
      has_output = true;
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
        has_output = true;
        std::cout << name_weight_vertexes[i].first << " " << name_weight_vertexes[i].second[j] << '\n';
      }
    }
    if (!has_output)
    {
      std::cout << '\n';
    }
  }

  void inbound(ht_graphs& graphs, std::string name_graph, std::string name_vertex)
  {
    bool has_output = false;
    if (!graphs.has(name_graph))
    {
      has_output = true;
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs[name_graph].has(name_vertex))
    {
      has_output = true;
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
      has_output = true;
      std::cout << res[i].first << ' ';
      for (size_t j = 0; j < res[i].second.getSize(); ++j)
      {
        has_output = true;
        std::cout << res[i].second[j];
        if (j == res[i].second.getSize() - 1)
        {
          has_output = true;
          std::cout << '\n';
        }
        else
        {
          has_output = true;
          std::cout << ' ';
        }
      }
    }
    if (!has_output)
    {
      std::cout << '\n';
    }
  }

  void bind(ht_graphs& graphs, std::string name_graph, std::string name_vertex1,
    std::string name_vertex2, size_t weight)
  {
    if (!graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
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
      return;
    }
    if (!graphs[name_graph].has(name_vertex1))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs[name_graph][name_vertex1].has(name_vertex2))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    for (size_t i = 0; i < graphs[name_graph][name_vertex1][name_vertex2].getSize(); ++i)
    {
      if (graphs[name_graph][name_vertex1][name_vertex2][i] == weight)
      {
        graphs[name_graph][name_vertex1][name_vertex2].erase(i);
        if (graphs[name_graph][name_vertex1][name_vertex2].getSize() == 0)
        {
          graphs[name_graph][name_vertex1].drop(name_vertex2);
        }
        return;
      }
    }
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  void create(ht_graphs& graphs, std::string name_graph, size_t count, topit::Vector< std::string > vertexes_names)
  {
    if (graphs.has(name_graph))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
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
      return;
    }
    if (!graphs.has(name_graph1))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphs.has(name_graph2))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
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
