#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <myvector.hpp>
#include "structs.hpp"

namespace goltsov
{
  void parsingAdd(std::istream&);
  void parsingAddProtected(std::istream&);
  void parsingRemove(std::istream&);
  void parsingList(std::istream&);
  void parsingMerge(std::istream&);
  void parsingShowUnplanned(std::istream&);
  void parsingUnplannedRemove(std::istream&);
  void parsingUnplannedForce(std::istream&);
  void parsingMergeScheduleOtherContext(std::istream&);
  void parsingAddScheduleOtherContext(std::istream&);
  void parsingAddForceScheduleOtherContext(std::istream&);
  void parsingSwitchSchedule(std::istream&);
  void parsingSwitchContext(std::istream&);
  void parsingStats(std::istream&);
  void parsingNewSchedule(std::istream&);
  void parsingNewContext(std::istream&);
  void parsingLoadSchedule(std::istream&);
  void parsingSaveSchedule(std::istream&);
  void parsingLoadContext(std::istream&);
  void parsingSaveContext(std::istream&);
  void parsingFindGap(std::istream&);
  void parsingFindGapOnInterval(std::istream&);
  void parsingFindCommonGap(std::istream&);
  void parsingFindCommonGapOnInterval(std::istream&);
  void parsingExit(std::istream&);

  void add(std::istream&, const goltsov::Task&);
  void addProtected(std::istream&, const goltsov::Task&);
  void remove(std::istream&, const std::string&);
  void list(std::istream&);
  void merge(std::istream&, const std::string&);
  void showUnplanned(std::istream&);
  void unplannedRemove(std::istream&, const std::string&);
  void unplannedForce(std::istream&, const std::string&);
  void mergeScheduleOtherContext(std::istream&, const std::string&, const std::string&);
  void addScheduleOtherContext(std::istream&, const std::string&, const std::string&);
  void addForceScheduleOtherContext(std::istream&, const std::string&, const std::string&);
  void switchSchedule(std::istream&, const std::string&);
  void switchContext(std::istream&, const std::string&);
  void stats(std::istream&, const goltsov::DateTime&, const goltsov::DateTime&);
  void newSchedule(std::istream&, const std::string&);
  void newContext(std::istream&, const std::string&);
  void loadSchedule(std::istream&, const std::string&, const std::string&);
  void saveSchedule(std::istream&, const std::string&, const std::string&);
  void loadContext(std::istream&, const std::string&, const std::string&);
  void saveContext(std::istream&, const std::string&, const std::string&);
  void findGap(std::istream&, const goltsov::TimeInterval&);
  void findGapOnInterval(std::istream&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&);
  void findCommonGap(std::istream&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&);
  void findCommonGapOnInterval(std::istream&, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&);
  void exit(std::istream&);
}

#endif
