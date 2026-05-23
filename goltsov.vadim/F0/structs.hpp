#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <string>

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
}

#endif