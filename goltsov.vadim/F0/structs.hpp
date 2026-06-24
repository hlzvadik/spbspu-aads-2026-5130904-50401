#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <string>
#include "mymap.hpp"
#include <myvector.hpp>

namespace goltsov
{
  struct TimeInterval;
  struct DateTime;
  struct Task;
  struct Schedule;
  struct Context;
  namespace detail
  {
    struct FindTaskHasETLessLB
    {
      const goltsov::Task& a;
      bool operator()(const std::pair< goltsov::DateTime, goltsov::Task >&);
    };
    struct FindTaskHasEnoughET
    {
      const goltsov::Task& a;
      const goltsov::TimeInterval& duration;
      bool operator()(const std::pair< goltsov::DateTime, goltsov::Task >&);
    };
    struct FindDateTime
    {
      const goltsov::DateTime& a;
      bool operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >&);
    };
    struct FindDateTime2
    {
      const goltsov::DateTime& a;
      bool operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >&);
    };
    struct FindDateTime3
    {
      const goltsov::DateTime& a;
      bool operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >&);
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
    bool isLeapYear(const size_t& year);
    size_t getDaysInMonth(const size_t& year, const size_t& month);
    long long TimeIntervalToSeconds(const TimeInterval& interval);
  }

  struct TimeInterval
  {
    size_t years_, months_, days_;
    size_t hours_, minutes_, seconds_;
  };
  TimeInterval operator-(const TimeInterval&, const TimeInterval&);
  TimeInterval operator+(const TimeInterval&, const TimeInterval&);
  double operator/(const TimeInterval&, const TimeInterval&);
  std::istream& operator>>(std::istream&, TimeInterval&);
  std::ostream& operator<<(std::ostream&, const TimeInterval&);
  bool operator<(const TimeInterval&, const TimeInterval&);
  bool operator>(const TimeInterval&, const TimeInterval&);
  bool operator==(const TimeInterval&, const TimeInterval&);
  bool operator!=(const TimeInterval&, const TimeInterval&);
  bool operator<=(const TimeInterval&, const TimeInterval&);
  bool operator>=(const TimeInterval&, const TimeInterval&);

  struct DateTime
  {
    size_t year_, month_, day_;
    size_t hour_, minute_, second_;
  };
  std::istream& operator>>(std::istream&, DateTime&);
  std::ostream& operator<<(std::ostream&, const DateTime&);
  bool operator<(const DateTime&, const DateTime&);
  bool operator>(const DateTime&, const DateTime&);
  bool operator==(const DateTime&, const DateTime&);
  bool operator!=(const DateTime&, const DateTime&);
  bool operator<=(const DateTime&, const DateTime&);
  bool operator>=(const DateTime&, const DateTime&);
  TimeInterval operator-(const DateTime&, const DateTime&);
  DateTime operator-(const DateTime&, const TimeInterval&);
  DateTime operator+(const DateTime&, const TimeInterval&);

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
  std::ostream& operator<<(std::ostream&, const Task&);

  struct Schedule
  {
    std::string name_schedule_;
    goltsov::Map< DateTime, Task > tasks_;
    goltsov::Map< std::string, goltsov::Task > unplanned_tasks_;
    goltsov::Map< std::string, goltsov::DateTime > id_start_time_;
  };
  std::istream& operator>>(std::istream&, Schedule&);
  std::ostream& operator<<(std::ostream&, Schedule&);

  struct Context
  {
    std::string name_context_;
    goltsov::Map< std::string, Schedule > schedules_;
  };
  std::istream& operator>>(std::istream&, Context&);
  std::ostream& operator<<(std::ostream&, Context&);

  struct State
  {
    Schedule* current_schedule_;
    Context* current_context_;
    goltsov::Map< std::string, Context > contexts_;
    goltsov::DateTime current_time;
  };
}

#endif
