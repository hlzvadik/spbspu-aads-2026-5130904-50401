#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <myvector.hpp>
#include "structs.hpp"

namespace detail
{
  std::istream& skipSpaces(std::istream&);
  bool isEndOfLine(std::istream&);
  bool pushTask(std::ostream&, goltsov::State&, goltsov::Task&);
}

namespace goltsov
{
  void parsingAdd(std::istream&, std::ostream&, goltsov::State&);
  void parsingAddProtected(std::istream&, std::ostream&, goltsov::State&);
  void parsingRemove(std::istream&, std::ostream&, goltsov::State&);
  void parsingList(std::istream&, std::ostream&, goltsov::State&);
  void parsingMerge(std::istream&, std::ostream&, goltsov::State&);
  void parsingShowUnplanned(std::istream&, std::ostream&, goltsov::State&);
  void parsingUnplannedRemove(std::istream&, std::ostream&, goltsov::State&);
  void parsingUnplannedForce(std::istream&, std::ostream&, goltsov::State&);
  void parsingMergeScheduleOtherContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingAddScheduleOtherContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingAddForceScheduleOtherContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingSwitchSchedule(std::istream&, std::ostream&, goltsov::State&);
  void parsingSwitchContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingStats(std::istream&, std::ostream&, goltsov::State&);
  void parsingNewSchedule(std::istream&, std::ostream&, goltsov::State&);
  void parsingNewContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingLoadSchedule(std::istream&, std::ostream&, goltsov::State&);
  void parsingSaveSchedule(std::istream&, std::ostream&, goltsov::State&);
  void parsingLoadContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingSaveContext(std::istream&, std::ostream&, goltsov::State&);
  void parsingFindGap(std::istream&, std::ostream&, goltsov::State&);
  void parsingFindGapOnInterval(std::istream&, std::ostream&, goltsov::State&);
  void parsingFindCommonGap(std::istream&, std::ostream&, goltsov::State&);
  void parsingFindCommonGapOnInterval(std::istream&, std::ostream&, goltsov::State&);
  void parsingExit(std::istream&, std::ostream&, goltsov::State&);

  void add(std::ostream&, goltsov::State&, const std::string&, const std::string&, const std::string&,
    const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&);
  void addProtected(std::ostream&, const std::string&, goltsov::State&, const std::string&, const std::string&,
    const goltsov::DateTime&, const goltsov::DateTime&);
  void remove(std::ostream&, goltsov::State&, const std::string&);
  void list(std::ostream&, goltsov::State&);
  void merge(std::ostream&, goltsov::State&, const std::string&);
  void showUnplanned(std::ostream&, goltsov::State&);
  void unplannedRemove(std::ostream&, goltsov::State&, const std::string&);
  void unplannedForce(std::ostream&, goltsov::State&, const std::string&);
  void mergeScheduleOtherContext(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void addScheduleOtherContext(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void addForceScheduleOtherContext(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void switchSchedule(std::ostream&, goltsov::State&, const std::string&);
  void switchContext(std::ostream&, goltsov::State&, const std::string&);
  void stats(std::ostream&, goltsov::State&, const goltsov::DateTime&, const goltsov::DateTime&);
  void newSchedule(std::ostream&, goltsov::State&, const std::string&);
  void newContext(std::ostream&, goltsov::State&, const std::string&);
  void loadSchedule(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void saveSchedule(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void loadContext(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void saveContext(std::ostream&, goltsov::State&, const std::string&, const std::string&);
  void findGap(std::ostream&, goltsov::State&, const goltsov::TimeInterval&);
  void findGapOnInterval(std::ostream&, goltsov::State&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&);
  void findCommonGap(std::ostream&, goltsov::State&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&);
  void findCommonGapOnInterval(std::ostream&, goltsov::State&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&);
  void exit(std::ostream&, goltsov::State&);
}

#endif
