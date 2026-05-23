#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <string>
#include <mymap.hpp>

struct goltsov::TimeInterval;
struct goltsov::DateTime;
struct goltsov::Task;
struct goltsov::Schedule;
struct goltsov::Context;

namespace detail
{
  struct CompareTasks
  {
    bool operator()(const goltsov::Task&, const goltsov::Task&);
  };
}

namespace goltsov
{
  struct TimeInterval
  {
    size_t hours_, minutes_, seconds_;
  };

  struct DateTime
  {
    size_t year_, month_, day_;
    size_t hour_, minute_, second_;
  };

  struct Task
  {
    std::string id_;
    std::string title_, description_;
    DateTime left_boundary_time_, right_boundary_time_;
    DateTime strart_time_, end_time_;
    size_t priority_;
    bool is_protected_;
  };

  struct Schedule
  {
    std::string name_schedule_;
    goltsov::RBTree< std::string, Task, detail::CompareTasks > tasks_tree_;
  };

  struct Context
  {
    std::string name_context_;
    goltsov::RBTree< std::string, Schedule, std::less< std::string > > schedules_tree_;
  };
}

#endif
