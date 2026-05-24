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

  struct Delimeter
  {
    char expected;
  };
  std::istream& operator>>(std::istream&, const Delimeter&);

  struct IOGuard
  {
    explicit IOGuard(std::basic_ios< char >& s):
      s_(s),
      precision_(s.precision()),
      width_(s.width()),
      flags_(s.flags()),
      fill_(s.fill())
    {}
    ~IOGuard()
    {
      s_.precision(precision_);
      s_.width(width_);
      s_.flags(flags_);
      s_.fill(fill_);
    }
  private:
    std::basic_ios< char >& s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };
}

namespace goltsov
{
  struct TimeInterval
  {
    size_t hours_, minutes_, seconds_;
  };
  std::istream& operator>>(std::istream&, TimeInterval&);

  struct DateTime
  {
    size_t year_, month_, day_;
    size_t hour_, minute_, second_;
  };
  std::istream& operator>>(std::istream&, DateTime&);

  struct Task
  {
    std::string id_;
    std::string title_, description_;
    DateTime left_boundary_time_, right_boundary_time_;
    DateTime start_time_, end_time_;
    size_t priority_;
    bool is_protected_;
  };
  std::istream& operator>>(std::istream&, Task&);

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
