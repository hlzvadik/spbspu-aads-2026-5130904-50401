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
    return is;
  }

  bool CompareTasks::operator()(const goltsov::DateTime& lhs, const goltsov::DateTime& rhs)
  {
    return lhs < rhs;
  }

  bool FindTask::operator()(const std::pair< goltsov::DateTime, goltsov::Task >& current)
  {
    return (current.second.end_time_ < a.left_boundary_time_);
  }

  bool FindDateTime::operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >& curent)
  {
    return curent.second.first <= a && a <= curent.second.second;
  }

  bool isLeapYear(const size_t& year)
  {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
  }
  static size_t getDaysInMonth(const size_t& year, const size_t& month)
  {
    if (month == 2)
    {
      return isLeapYear(year) ? 29 : 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
      return 30;
    }
    return 31;
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
    size_t years, months, days;
    size_t hours, minutes, seconds;
    is >> years >> d_t{'-'} >> months >> d_t{'-'} >> days;
    is >> d_t{'_'};
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
  std::istream& operator>>(std::istream& is, Schedule& a)
  {
    size_t count;
    is >> count;
    for (size_t i = 0; i < count; ++i)
    {
      goltsov::Task task;
      is >> task;
      a.tasks_tree_.push(task.start_time_, task);
    }
    return is;
  }
  std::istream& operator>>(std::istream& is, Context& a)
  {
    size_t count;
    is >> count;
    for (size_t i = 0; i < count; ++i)
    {
      goltsov::Schedule schedule;
      std::string name_schedule;
      is >> name_schedule >> schedule;
      a.schedules_tree_.push(name_schedule, schedule);
    }
    return is;
  }
  std::ostream& operator<<(std::ostream& os, const TimeInterval& a)
  {
    os << a.years_ << '-' << a.months_ << '-' << a.days_ << '_' << a.hours_ << ':' << a.minutes_ << ':' << a.seconds_;
    return os;
  }
  std::ostream& operator<<(std::ostream& os, const DateTime& a)
  {
    os << a.year_ << '-' << a.month_ << '-' << a.day_ << '_' << a.hour_ << ':' << a.minute_ << ':' << a.second_;
    return os;
  }
  std::ostream& operator<<(std::ostream& os, const Task& a)
  {
    os << "(id: " << a.id_ << " (" << a.start_time_ << " " << a.end_time_ << ") " << a.title_ << " (Prio: " << a.priority_ << ") (Is_protected: " << (a.is_protected_ ? 1 : 0) << ")";
    return os;
  }
  std::ostream& operator<<(std::ostream& os, Schedule& a)
  {
    os << a.tasks_tree_.size();
    for (RBTIterator< goltsov::DateTime, goltsov::Task > it = a.tasks_tree_.begin(); it != a.tasks_tree_.end(); ++it)
    {
      os << '\n' << it->second.id_ << ' ' << it->second.title_ << ' ' << it->second.description_ << ' '
         << it->second.left_boundary_time_ << ' ' << it->second.right_boundary_time_ << ' '
          << it->second.start_time_ << ' ' << it->second.end_time_ << ' ' << it->second.priority_ << ' '
           << it->second.is_protected_;
    }
    return os;
  }
  std::ostream& operator<<(std::ostream& os, Context& a)
  {
    os << a.schedules_tree_.size();
    for (RBTIterator< std::string, goltsov::Schedule > it = a.schedules_tree_.begin(); it != a.schedules_tree_.end(); ++it)
    {
      os << '\n' << it->second.name_schedule_ << ' ' << it->second;
    }
    return os;
  }
  bool operator<(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    if (lhs.years_ != rhs.years_)
    {
      return lhs.years_ < rhs.years_;
    }
    if (lhs.months_ != rhs.months_)
    {
      return lhs.months_ < rhs.months_;
    }
    if (lhs.days_ != rhs.days_)
    {
      return lhs.days_ < rhs.days_;
    }
    if (lhs.hours_ != rhs.hours_)
    {
      return lhs.hours_ < rhs.hours_;
    }
    if (lhs.minutes_ != rhs.minutes_)
    {
      return lhs.minutes_ < rhs.minutes_;
    }
    return lhs.seconds_ < rhs.seconds_;
  }
  bool operator> (const TimeInterval& lhs, const TimeInterval& rhs)
  {
    return rhs < lhs;
  }
  bool operator==(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    return !(lhs < rhs) && !(rhs < lhs);
  }
  bool operator!=(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    return !(lhs == rhs);
  }
  bool operator<=(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    return !(lhs > rhs);
  }
  bool operator>=(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    return !(lhs < rhs);
  }
  bool operator<(const DateTime& lhs, const DateTime& rhs)
  {
    if (lhs.year_ != rhs.year_)
    {
      return lhs.year_ < rhs.year_;
    }
    if (lhs.month_ != rhs.month_)
    {
      return lhs.month_ < rhs.month_;
    }
    if (lhs.day_ != rhs.day_)
    {
      return lhs.day_ < rhs.day_;
    }
    if (lhs.hour_ != rhs.hour_)
    {
      return lhs.hour_ < rhs.hour_;
    }
    if (lhs.minute_ != rhs.minute_)
    {
      return lhs.minute_ < rhs.minute_;
    }
    return lhs.second_ < rhs.second_;
  }
  bool operator> (const DateTime& lhs, const DateTime& rhs)
  {
    return rhs < lhs;
  }
  bool operator==(const DateTime& lhs, const DateTime& rhs)
  {
    return !(lhs < rhs) && ! (rhs < lhs);
  }
  bool operator!=(const DateTime& lhs, const DateTime& rhs)
  {
    return !(lhs == rhs);
  }
  bool operator<=(const DateTime& lhs, const DateTime& rhs)
  {
    return !(lhs > rhs);
  }
  bool operator>=(const DateTime& lhs, const DateTime& rhs)
  {
    return !(lhs < rhs);
  }
  TimeInterval operator-(const DateTime& lhs, const DateTime& rhs)
  {
    if (lhs < rhs)
    {
      throw std::logic_error("The first operand must be greater than second");
    }
    TimeInterval result;
    int sec = static_cast<int> (lhs.second_) - static_cast<int> (rhs.second_);
    int borrow_min = 0;
    if (sec < 0)
    {
      sec += 60;
      borrow_min = 1;
    }
    result.seconds_ = static_cast<size_t> (sec);
    int min = static_cast<int> (lhs.minute_) - static_cast<int> (rhs.minute_) - borrow_min;
    int borrow_hour = 0;
    if (min < 0)
    {
      min += 60;
      borrow_hour = 1;
    }
    result.minutes_ = static_cast<size_t> (min);
    int hour = static_cast<int> (lhs.hour_) - static_cast<int> (rhs.hour_) - borrow_hour;
    int borrow_day = 0;
    if (hour < 0)
    {
      hour += 24;
      borrow_day = 1;
    }
    result.hours_ = static_cast<size_t> (hour);
    int day = static_cast<int> (lhs.day_) - static_cast<int> (rhs.day_) - borrow_day;
    int borrow_month = 0;
    if (day < 0)
    {
      size_t prev_month = lhs.month_ == 1 ? 12 : lhs.month_ - 1;
      size_t prev_year = lhs.month_ == 1 ? lhs.year_ - 1 : lhs.year_;
      day += static_cast<int> (detail::getDaysInMonth(prev_year, prev_month));
      borrow_month = 1;
    }
    result.days_ = static_cast<size_t> (day);
    int month = static_cast<int> (lhs.month_) - static_cast<int> (rhs.month_) - borrow_month;
    int borrow_year = 0;
    if (month < 0)
    {
      month += 12;
      borrow_year = 1;
    }
    result.months_ = static_cast<size_t> (month);
    int year = static_cast<int> (lhs.year_) - static_cast<int> (rhs.year_) - borrow_year;
    result.years_ = static_cast<size_t> (year);
    return result;
  }
  DateTime operator+(const DateTime& lhs, const TimeInterval& rhs)
  {
    DateTime result = lhs;
    int sec = static_cast<int> (result.second_) + static_cast<int> (rhs.seconds_);
    int carry_min = sec / 60;
    result.second_ = sec % 60;
    int min = static_cast<int> (result.minute_) + static_cast<int> (rhs.minutes_) + carry_min;
    int carry_hour = min / 60;
    result.minute_ = min % 60;
    int hour = static_cast<int> (result.hour_) + static_cast<int> (rhs.hours_) + carry_hour;
    int carry_day = hour / 24;
    result.hour_ = hour % 24;
    result.year_ += rhs.years_;
    int total_months = static_cast<int> (result.month_) + static_cast<int> (rhs.months_);
    int carry_year_from_month = (total_months - 1) / 12;
    result.year_ += carry_year_from_month;
    result.month_ = (total_months - 1) % 12 + 1;
    int days_to_add = static_cast<int> (rhs.days_) + carry_day;
    while (days_to_add > 0)
    {
      size_t days_in_current_month = detail::getDaysInMonth(result.year_, result.month_);
      int remaining_days_in_month = static_cast<int> (days_in_current_month) - static_cast<int> (result.day_);
      if (days_to_add <= remaining_days_in_month)
      {
        result.day_ += days_to_add;
        days_to_add = 0;
      } else
      {
        days_to_add -= (remaining_days_in_month + 1);
        result.day_ = 1;
        result.month_++;
        if (result.month_ > 12)
        {
          result.month_ = 1;
          result.year_++;
        }
      }
    }
    return result;
  }
  DateTime operator-(const DateTime& lhs, const TimeInterval& rhs)
  {
    DateTime result = lhs;
    int sec = static_cast<int> (result.second_) - static_cast<int> (rhs.seconds_);
    int borrow_min = 0;
    if (sec < 0)
    {
      sec += 60;
      borrow_min = 1;
    }
    result.second_ = static_cast<size_t> (sec);
    int min = static_cast<int> (result.minute_) - static_cast<int> (rhs.minutes_) - borrow_min;
    int borrow_hour = 0;
    if (min < 0)
    {
      min += 60;
      borrow_hour = 1;
    }
    result.minute_ = static_cast<size_t> (min);
    int hour = static_cast<int> (result.hour_) - static_cast<int> (rhs.hours_) - borrow_hour;
    int borrow_day = 0;
    if (hour < 0)
    {
      hour += 24;
      borrow_day = 1;
    }
    result.hour_ = static_cast<size_t> (hour);
    if (rhs.years_ > result.year_)
    {
      throw std::logic_error("Resulting year would be negative");
    }
    result.year_ -= rhs.years_;
    int total_months = static_cast<int> (result.month_) - static_cast<int> (rhs.months_);
    int borrow_year_from_month = 0;
    while (total_months <= 0) {
      total_months += 12;
      borrow_year_from_month++;
    }
    result.month_ = static_cast<size_t> (total_months);
    if (result.year_ < borrow_year_from_month)
    {
      throw std::logic_error("Resulting year would be negative");
    }
    result.year_ -= borrow_year_from_month;
    int days_to_subtract = static_cast<int> (rhs.days_) + borrow_day;
    while (days_to_subtract > 0)
    {
      if (static_cast<int> (result.day_) > days_to_subtract)
      {
        result.day_ -= days_to_subtract;
        days_to_subtract = 0;
      }
      else
      {
        days_to_subtract -= result.day_;
        if (result.month_ == 1)
        {
          result.month_ = 12;
          if (result.year_ == 0)
          {
            throw std::logic_error("Resulting year would be negative");
          }
          result.year_--;
        }
        else
        {
          result.month_--;
        }
        result.day_ = detail::getDaysInMonth(result.year_, result.month_);
      }
    }
    return result;
  }
  long long TimeIntervalToSeconds(const TimeInterval& interval)
  {
    long long sec_in_min = 60;
    long long sec_in_hour = 60 * sec_in_min;
    long long sec_in_day = 24 * sec_in_hour;
    long long sec_ind_mounth = 30 * sec_in_day;
    long long sec_in_year = 365 * sec_in_day;
    long long total_seconds = 0;
    total_seconds += static_cast< long long > (interval.years_) * sec_in_year;
    total_seconds += static_cast< long long > (interval.months_) * sec_ind_mounth;
    total_seconds += static_cast< long long > (interval.days_) * sec_in_day;
    total_seconds += static_cast< long long > (interval.hours_) * sec_in_hour;
    total_seconds += static_cast< long long > (interval.minutes_) * sec_in_min;
    total_seconds += static_cast< long long > (interval.seconds_);
    return total_seconds;
  }
  TimeInterval operator+(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    TimeInterval result;
    long long sec = static_cast< long long > (lhs.seconds_) + static_cast< long long > (rhs.seconds_);
    long long carry_min = sec / 60;
    result.seconds_ = static_cast< size_t > (sec % 60);
    long long min = static_cast< long long > (lhs.minutes_) + static_cast< long long > (rhs.minutes_) + carry_min;
    long long carry_hour = min / 60;
    result.minutes_ = static_cast< size_t > (min % 60);
    long long hour = static_cast< long long > (lhs.hours_) + static_cast< long long > (rhs.hours_) + carry_hour;
    long long carry_day = hour / 24;
    result.hours_ = static_cast< size_t > (hour % 24);
    long long day = static_cast< long long > (lhs.days_) + static_cast< long long > (rhs.days_) + carry_day;
    long long carry_month = day / 30;
    result.days_ = static_cast< size_t > (day % 30);
    long long month = static_cast< long long > (lhs.months_) + static_cast< long long > (rhs.months_) + carry_month;
    long long carry_year = month / 12;
    result.months_ = static_cast< size_t > (month % 12);
    result.years_ = static_cast< size_t > (static_cast< long long > (lhs.years_) + static_cast< long long > (rhs.years_) + carry_year);
    return result;
  }
  TimeInterval operator-(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    TimeInterval result;
    long long sec = static_cast< long long > (lhs.seconds_) - static_cast< long long > (rhs.seconds_);
    long long borrow_min = 0;
    if (sec < 0)
    {
      sec += 60;
      borrow_min = 1;
    }
    result.seconds_ = static_cast< size_t > (sec);
    long long min = static_cast< long long > (lhs.minutes_) - static_cast< long long > (rhs.minutes_) - borrow_min;
    long long borrow_hour = 0;
    if (min < 0)
    {
      min += 60;
      borrow_hour = 1;
    }
    result.minutes_ = static_cast< size_t > (min);
    long long hour = static_cast< long long > (lhs.hours_) - static_cast< long long > (rhs.hours_) - borrow_hour;
    long long borrow_day = 0;
    if (hour < 0)
    {
      hour += 24;
      borrow_day = 1;
    }
    result.hours_ = static_cast< size_t > (hour);
    long long day = static_cast< long long > (lhs.days_) - static_cast< long long > (rhs.days_) - borrow_day;
    long long borrow_month = 0;
    if (day < 0)
    {
      day += 30;
      borrow_month = 1;
    }
    result.days_ = static_cast< size_t > (day);
    long long month = static_cast< long long > (lhs.months_) - static_cast< long long > (rhs.months_) - borrow_month;
    long long borrow_year = 0;
    if (month < 0)
    {
      month += 12;
      borrow_year = 1;
    }
    result.months_ = static_cast< size_t > (month);
    long long year = static_cast< long long > (lhs.years_) - static_cast< long long > (rhs.years_) - borrow_year;
    if (year < 0)
    {
      throw std::logic_error("Resulting interval would be negative");
    }
    result.years_ = static_cast< size_t > (year);
    return result;
  }
  double operator/(const TimeInterval& lhs, const TimeInterval& rhs)
  {
    long long lhs_seconds = TimeIntervalToSeconds(lhs);
    long long rhs_seconds = TimeIntervalToSeconds(rhs);
    if (rhs_seconds == 0)
    {
      throw std::invalid_argument("Division by zero interval");
    }
    return static_cast< double > (lhs_seconds) / static_cast< double > (rhs_seconds);
  }
}
