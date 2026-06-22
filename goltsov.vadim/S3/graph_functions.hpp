#include <cstddef>
#include <string>
#include <myvector.hpp>
#include "my_hash_table.hpp"

namespace goltsov
{
  namespace detail
  {
    void sortStringVector(goltsov::Vector< std::string >&);
    void sortSizeTVector(goltsov::Vector< size_t >&);
    void sortPairStringSizeTVector(goltsov::Vector< std::pair< std::string, goltsov::Vector< size_t > > >&);
  }

  using ht_graphs = goltsov::HashTable< std::string, goltsov::HashTable< std::pair< std::string, std::string >,
    goltsov::Vector< size_t >, goltsov::Sha1Hasher< std::pair< std::string, std::string > >,
    std::equal_to< std::pair< std::string, std::string > > >, goltsov::Sha1Hasher< std::string >,
    std::equal_to< std::string > >;

  using ht_it_graphs = goltsov::HashTableIterator< std::string,
    goltsov::HashTable< std::pair< std::string, std::string >, goltsov::Vector< size_t >,
    goltsov::Sha1Hasher< std::pair< std::string, std::string > >,
    std::equal_to< std::pair< std::string, std::string > > >,
    goltsov::Sha1Hasher< std::string >, std::equal_to< std::string > >;

    using ht_it_pairs = goltsov::HashTableIterator< std::pair< std::string, std::string >, goltsov::Vector< size_t >,
      goltsov::Sha1Hasher< std::pair< std::string, std::string > >,
      std::equal_to< std::pair< std::string, std::string > > >;

  using ht_functions = goltsov::HashTable< std::string, void (*)(ht_graphs&, std::istream&),
    goltsov::Sha1Hasher< std::string >, std::equal_to< std::string > >;

  void graphsParsing(ht_graphs&, std::istream&);
  void vertexesParsing(ht_graphs&, std::istream&);
  void outboundParsing(ht_graphs&, std::istream&);
  void inboundParsing(ht_graphs&, std::istream&);
  void bindParsing(ht_graphs&, std::istream&);
  void cutParsing(ht_graphs&, std::istream&);
  void createParsing(ht_graphs&, std::istream&);
  void mergeParsing(ht_graphs&, std::istream&);
  void extractParsing(ht_graphs&, std::istream&);

  void read_graphs(std::istream&, ht_graphs&);
  void graphs(ht_graphs&);
  void vertexes(ht_graphs&, std::string);
  void outbound(ht_graphs&, std::string, std::string);
  void inbound(ht_graphs&, std::string, std::string);
  void bind(ht_graphs&, std::string,
    std::string, std::string, size_t);
  void cut(ht_graphs&, std::string,
    std::string, std::string, size_t);
  void create(ht_graphs&, std::string,
    size_t, goltsov::Vector< std::string >);
  void merge(ht_graphs&, std::string,
    std::string, std::string);
  void extract(ht_graphs&, std::string, std::string,
    size_t, goltsov::Vector< std::string >);
}
