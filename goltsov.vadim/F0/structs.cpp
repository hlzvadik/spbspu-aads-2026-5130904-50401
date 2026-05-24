#include "structs.hpp"

namespace detail
{
  std::istream& operator>>(std::istream& is, const Delimeter& a)
  {
    char c;
    is.get(c);
    if (c != a.expected)
    {
      is.setstate(std::ios_base::failbit);
    }
  }
}

namespace goltsov
{
  std::istream& operator>>(std::istream& is, TimeInterval& a)
  {
    std::istream::sentry s (is);
    if (!s)
    {
      return is;
    }
    detail::IOGuard guard (is);
    using d_t = detail::Delimeter;
    size_t hours, minutes, seconds;
    is >> hours >> d_t {':'} >> minutes >> d_t {':'} >> seconds;
    a.hours_ = hours;
    a.minutes_ = minutes;
    a.seconds_ = seconds;
    return is;
  }
  std::istream& operator>>(std::istream& is, DateTime& a)
  {
    std::istream::sentry s (is);
    if (!s)
    {
      return is;
    }
    detail::IOGuard guard (is);
    using d_t = detail::Delimeter;
    size_t year, month, day;
    size_t hour, minute, second;
    is >> year >> d_t {'-'} >> month >> d_t {'-'} >> day;
    is >> d_t {'_'};
    is >> hour >> d_t {':'} >> minute >> d_t {':'} >> second;
    a.year_ = year;
    a.month_ = month;
    a.day_ = day;
    a.hour_ = hour;
    a.minute_ = minute;
    a.second_ = second;
    return is;
  }
  std::istream& operator>>(std::istream& is, Task& a)
  {
    std::istream::sentry s (is);
    if (!s)
    {
      return is;
    }
    detail::IOGuard guard (is);
    using d_t = detail::Delimeter;
    std::string id;
    std::string title, description;
    DateTime left_boundary_time, right_boundary_time;
    DateTime start_time, end_time;
    size_t priority;
    bool is_protected;
    is >> id >> d_t {' '} >> title >> d_t {' '} >> description >> d_t {' '}  >> left_boundary_time
      >> d_t {' '} >> right_boundary_time >> d_t {' '} >> start_time >> d_t {' '} >> end_time
      >> d_t {' '} >> priority >> d_t {' '} >> is_protected;
    a.id_ = id;
    a.title_ = title;
    a.description_ = description;
    a.left_boundary_time_ = left_boundary_time;
    a.right_boundary_time_ = right_boundary_time;
    a.start_time_ = start_time;
    a.end_time_ = end_time;
    a.priority_ = priority;
    return is;
  }
}
