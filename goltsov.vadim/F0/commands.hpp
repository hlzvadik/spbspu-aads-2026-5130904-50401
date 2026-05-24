#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <myvector.hpp>
#include "structs.hpp"

namespace detail
{
  std::istream& skipSpaces(std::istream&);
  bool isEndOfLine(std::istream&);
}

namespace goltsov
{
  void parsingAdd(std::istream&, std::ostream&);
  void parsingAddProtected(std::istream&, std::ostream&);
  void parsingRemove(std::istream&, std::ostream&);
  void parsingList(std::istream&, std::ostream&);
  void parsingMerge(std::istream&, std::ostream&);
  void parsingShowUnplanned(std::istream&, std::ostream&);
  void parsingUnplannedRemove(std::istream&, std::ostream&);
  void parsingUnplannedForce(std::istream&, std::ostream&);
  void parsingMergeScheduleOtherContext(std::istream&, std::ostream&);
  void parsingAddScheduleOtherContext(std::istream&, std::ostream&);
  void parsingAddForceScheduleOtherContext(std::istream&, std::ostream&);
  void parsingSwitchSchedule(std::istream&, std::ostream&);
  void parsingSwitchContext(std::istream&, std::ostream&);
  void parsingStats(std::istream&, std::ostream&);
  void parsingNewSchedule(std::istream&, std::ostream&);
  void parsingNewContext(std::istream&, std::ostream&);
  void parsingLoadSchedule(std::istream&, std::ostream&);
  void parsingSaveSchedule(std::istream&, std::ostream&);
  void parsingLoadContext(std::istream&, std::ostream&);
  void parsingSaveContext(std::istream&, std::ostream&);
  void parsingFindGap(std::istream&, std::ostream&);
  void parsingFindGapOnInterval(std::istream&, std::ostream&);
  void parsingFindCommonGap(std::istream&, std::ostream&);
  void parsingFindCommonGapOnInterval(std::istream&, std::ostream&);
  void parsingExit(std::istream&, std::ostream&);

  void add(std::ostream&, const std::string&, const std::string&, const std::string&,
    const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&);
  void addProtected(std::ostream&, const std::string&, const std::string&, const std::string&,
    const goltsov::DateTime&, const goltsov::DateTime&);
  void remove(std::ostream&, const std::string&);
  void list(std::ostream&);
  void merge(std::ostream&, const std::string&);
  void showUnplanned(std::ostream&);
  void unplannedRemove(std::ostream&, const std::string&);
  void unplannedForce(std::ostream&, const std::string&);
  void mergeScheduleOtherContext(std::ostream&, const std::string&, const std::string&);
  void addScheduleOtherContext(std::ostream&, const std::string&, const std::string&);
  void addForceScheduleOtherContext(std::ostream&, const std::string&, const std::string&);
  void switchSchedule(std::ostream&, const std::string&);
  void switchContext(std::ostream&, const std::string&);
  void stats(std::ostream&, const goltsov::DateTime&, const goltsov::DateTime&);
  void newSchedule(std::ostream&, const std::string&);
  void newContext(std::ostream&, const std::string&);
  void loadSchedule(std::ostream&, const std::string&, const std::string&);
  void saveSchedule(std::ostream&, const std::string&, const std::string&);
  void loadContext(std::ostream&, const std::string&, const std::string&);
  void saveContext(std::ostream&, const std::string&, const std::string&);
  void findGap(std::ostream&, const goltsov::TimeInterval&);
  void findGapOnInterval(std::ostream&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&);
  void findCommonGap(std::ostream&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&);
  void findCommonGapOnInterval(std::ostream&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&);
  void exit(std::ostream&);
}

#endif
