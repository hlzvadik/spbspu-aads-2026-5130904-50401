#include "commands.hpp"
#include "structs.hpp"

namespace detail
{
  std::istream& skipSpaces(std::istream& is)
  {
    while (is.peek() == ' ')
    {
      is.get();
    }
    return is;
  }
  bool isEndOfLine(std::istream& is)
  {
    if (is.peek() == '\n' || is.eof())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

namespace goltsov
{
  void parsingAdd(std::istream& is, std::ostream& os)
  {
    std::string id;
    std::string title, description;
    goltsov::DateTime start_time, end_time;
    goltsov::TimeInterval duration;
    size_t priority;
    is >> id;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> title;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> description;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> start_time;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> end_time;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> duration;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> priority;
    if (!is || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    add(os, id, title, description, start_time, end_time, duration, priority);
  }
  void parsingAddProtected(std::istream& is, std::ostream& os)
  {
    std::string id;
    std::string title, description;
    goltsov::DateTime start_time, end_time;
    is >> id;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> title;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> description;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> start_time;
    if (!is || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> end_time;
    if (!is || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    addProtected(os, id, title, description, start_time, end_time);
  }
  void parsingRemove(std::istream&, std::ostream& os)
  {}
  void parsingList(std::istream&, std::ostream& os)
  {}
  void parsingMerge(std::istream&, std::ostream& os)
  {}
  void parsingShowUnplanned(std::istream&, std::ostream& os)
  {}
  void parsingUnplannedRemove(std::istream&, std::ostream& os)
  {}
  void parsingUnplannedForce(std::istream&, std::ostream& os)
  {}
  void parsingMergeScheduleOtherContext(std::istream&, std::ostream& os)
  {}
  void parsingAddScheduleOtherContext(std::istream&, std::ostream& os)
  {}
  void parsingAddForceScheduleOtherContext(std::istream&, std::ostream& os)
  {}
  void parsingSwitchSchedule(std::istream&, std::ostream& os)
  {}
  void parsingSwitchContext(std::istream&, std::ostream& os)
  {}
  void parsingStats(std::istream&, std::ostream& os)
  {}
  void parsingNewSchedule(std::istream&, std::ostream& os)
  {}
  void parsingNewContext(std::istream&, std::ostream& os)
  {}
  void parsingLoadSchedule(std::istream&, std::ostream& os)
  {}
  void parsingSaveSchedule(std::istream&, std::ostream& os)
  {}
  void parsingLoadContext(std::istream&, std::ostream& os)
  {}
  void parsingSaveContext(std::istream&, std::ostream& os)
  {}
  void parsingFindGap(std::istream&, std::ostream& os)
  {}
  void parsingFindGapOnInterval(std::istream&, std::ostream& os)
  {}
  void parsingFindCommonGap(std::istream&, std::ostream& os)
  {}
  void parsingFindCommonGapOnInterval(std::istream&, std::ostream& os)
  {}
  void parsingExit(std::istream&, std::ostream& os)
  {}

  void add(std::ostream&, const std::string&, const std::string&, const std::string&,
    const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&)
  {}
  void addProtected(std::ostream&, const std::string&, const std::string&, const std::string&,
    const goltsov::DateTime&, const goltsov::DateTime&)
  {}
  void remove(std::ostream&, const std::string&)
  {}
  void list(std::ostream&)
  {}
  void merge(std::ostream&, const std::string&)
  {}
  void showUnplanned(std::ostream&)
  {}
  void unplannedRemove(std::ostream&, const std::string&)
  {}
  void unplannedForce(std::ostream&, const std::string&)
  {}
  void mergeScheduleOtherContext(std::ostream&, const std::string&, const std::string&)
  {}
  void addScheduleOtherContext(std::ostream&, const std::string&, const std::string&)
  {}
  void addForceScheduleOtherContext(std::ostream&, const std::string&, const std::string&)
  {}
  void switchSchedule(std::ostream&, const std::string&)
  {}
  void switchContext(std::ostream&, const std::string&)
  {}
  void stats(std::ostream&, const goltsov::DateTime&, const goltsov::DateTime&)
  {}
  void newSchedule(std::ostream&, const std::string&)
  {}
  void newContext(std::ostream&, const std::string&)
  {}
  void loadSchedule(std::ostream&, const std::string&, const std::string&)
  {}
  void saveSchedule(std::ostream&, const std::string&, const std::string&)
  {}
  void loadContext(std::ostream&, const std::string&, const std::string&)
  {}
  void saveContext(std::ostream&, const std::string&, const std::string&)
  {}
  void findGap(std::ostream&, const goltsov::TimeInterval&)
  {}
  void findGapOnInterval(std::ostream&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&)
  {}
  void findCommonGap(std::ostream&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&)
  {}
  void findCommonGapOnInterval(std::ostream&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&)
  {}
  void exit(std::ostream&)
  {}
}
