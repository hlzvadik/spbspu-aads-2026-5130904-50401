#include <iostream>
#include <mymap.hpp>
#include "structs.hpp"
#include "commands.hpp"

int main()
{
  goltsov::DateTime current_time;
  std::cout << "Enter the current time: ";
  std::cin >> current_time;
  while (std::cin.eof() || std::cin.fail())
  {
    if (std::cin.eof())
    {
      return 0;
    }
    std::cin.clear();
    std::cout << "Incorrect format, correct format: \"YYYY-MM-DD_HH-MM-SS\". Enter again: ";
    std::cin >> current_time;
  }
  goltsov::Schedule current_schedule;
  goltsov::Context current_context;
  goltsov::RBTree< std::string, goltsov::Context, std::less< std::string > > contexts_tree {};
  contexts_tree.push("Base context", current_context);
  contexts_tree.get("Base context")->second.schedules_tree_.push("Base schedule", current_schedule);
  goltsov::State current_state {contexts_tree.get("Base context")->second.schedules_tree_.get("Base schedule")->second, contexts_tree.get("Base context")->second, contexts_tree, current_time};

  goltsov::Map< std::string, void (*)(std::istream&, std::ostream&, goltsov::State&) > commands;
  commands["add"] = goltsov::parsingAdd;
  commands["add_protected"] = goltsov::parsingAddProtected;
  commands["remove"] = goltsov::parsingRemove;
  commands["list"] = goltsov::parsingList;
  commands["merge"] = goltsov::parsingMerge;
  commands["show_unplanned"] = goltsov::parsingShowUnplanned;
  commands["unplanned_remove"] = goltsov::parsingUnplannedRemove;
  commands["unplanned_force"] = goltsov::parsingUnplannedForce;
  commands["merge_schedule_other_context"] = goltsov::parsingMergeScheduleOtherContext;
  commands["add_schedule_other_context"] = goltsov::parsingAddScheduleOtherContext;
  commands["add_force_schedule_other_context"] = goltsov::parsingAddForceScheduleOtherContext;
  commands["switch_schedule"] = goltsov::parsingSwitchSchedule;
  commands["switch_context"] = goltsov::parsingSwitchContext;
  commands["stats"] = goltsov::parsingStats;
  commands["new_schedule"] = goltsov::parsingNewSchedule;
  commands["new_context"] = goltsov::parsingNewContext;
  commands["load_schedule"] = goltsov::parsingLoadSchedule;
  commands["save_schedule"] = goltsov::parsingSaveSchedule;
  commands["load_context"] = goltsov::parsingLoadContext;
  commands["save_context"] = goltsov::parsingSaveContext;
  commands["find_gap"] = goltsov::parsingFindGap;
  commands["find_gap_on_interval"] = goltsov::parsingFindGapOnInterval;
  commands["find_common_gap"] = goltsov::parsingFindCommonGap;
  commands["find_common_gap_on_interval"] = goltsov::parsingFindCommonGapOnInterval;
  commands["exit"] = goltsov::parsingExit;

  while(!std::cin.eof())
  {
    std::string command;
    std::cin >> command;
    if (!commands.count(command))
    {
      std::cout << "INCORRECT SYNTAX\n";
    }
    commands[command](std::cin, std::cout, current_state);
  }
}
