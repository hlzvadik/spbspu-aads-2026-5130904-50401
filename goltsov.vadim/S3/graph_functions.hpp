#include <cstddef>
#include <string>
#include <myvector.hpp>
#include "my_hash_table.hpp"

namespace goltsov
{
  using ht_graphs = goltsov::HashTable< std::string, goltsov::HashTable< std::pair< std::string, std::string >,
    goltsov::Vector< size_t >, goltsov::Sha1Hasher< std::pair< std::string, std::string > >, std::equal_to< std::pair< std::string, std::string > > >,
    goltsov::Sha1Hasher< std::string >, std::equal_to< std::string > >;

  using ht_it_graphs = goltsov::HashTableIterator< std::string, goltsov::HashTable< std::pair< std::string, std::string >,
    goltsov::Vector< size_t >, goltsov::Sha1Hasher< std::pair< std::string, std::string > >, std::equal_to< std::pair< std::string, std::string > > >,
    goltsov::Sha1Hasher< std::string >, std::equal_to< std::string > >;

    using ht_it_pairs = goltsov::HashTableIterator< std::pair< std::string, std::string >, goltsov::Vector< size_t >,
      goltsov::Sha1Hasher< std::pair< std::string, std::string > >, std::equal_to< std::pair< std::string, std::string > > >;

  using ht_functions = goltsov::HashTable< std::string, void (*)(ht_graphs&, std::istream&),
    goltsov::Sha1Hasher< std::string >, std::equal_to< std::string > >;

  void graphsParsing(ht_graphs& g, std::istream& in);
  void vertexesParsing(ht_graphs& g, std::istream& in);
  void outboundParsing(ht_graphs& g, std::istream& in);
  void inboundParsing(ht_graphs& g, std::istream& in);
  void bindParsing(ht_graphs& g, std::istream& in);
  void cutParsing(ht_graphs& g, std::istream& in);
  void createParsing(ht_graphs& g, std::istream& in);
  void mergeParsing(ht_graphs& g, std::istream& in);
  void extractParsing(ht_graphs& g, std::istream& in);

  void sortStringVector(goltsov::Vector< std::string >& vec);
  void sortSizeTVector(goltsov::Vector< size_t >& vec);
  void sortPairStringSizeTVector(goltsov::Vector< std::pair< std::string, goltsov::Vector< size_t > > >& vec);
  void read_graphs(std::istream& in, ht_graphs& graphs);
  void graphs(ht_graphs& graphs);
  void vertexes(ht_graphs& graphs, std::string name_graph);
  void outbound(ht_graphs& graphs, std::string name_graph, std::string name_vertex);
  void inbound(ht_graphs& graphs, std::string name_graph, std::string name_vertex);
  void bind(ht_graphs& graphs, std::string name_graph,
    std::string name_vertex1, std::string name_vertex2, size_t weight);
  void cut(ht_graphs& graphs, std::string name_graph,
    std::string name_vertex1, std::string name_vertex2, size_t weight);
  void create(ht_graphs& graphs, std::string name_graph,
    size_t count, goltsov::Vector< std::string > vertexes_names);
  void merge(ht_graphs& graphs, std::string name_new_graph,
    std::string name_graph1, std::string name_graph2);
  void extract(ht_graphs& graphs, std::string name_new_graph, std::string name_old_graph,
    size_t count_vertexes, goltsov::Vector< std::string > vertexes);
}
