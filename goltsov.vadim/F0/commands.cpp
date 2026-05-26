#include <stdexcept>
#include "commands.hpp"
#include "structs.hpp"

namespace detail
{
  std::istream& skipSpaces(std::istream& is)
  {
    while (is.peek() == ' ')
    {
      is.get();
    }
    return is;
  }
  bool isEndOfLine(std::istream& is)
  {
    if (is.peek() == '\n' || is.eof())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  bool pushTask(goltsov::State& current_state, goltsov::Task& task, const goltsov::TimeInterval& duration)
  {
    goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > current = current_state.current_schedule_.tasks_tree_.find(detail::FindTask {task});
    if (current == current_state.current_schedule_.tasks_tree_.end() || current == current_state.current_schedule_.tasks_tree_.begin())
    {
      if (current == current_state.current_schedule_.tasks_tree_.end() || ((* current).first) > task.left_boundary_time_ && ((* current).first) - task.left_boundary_time_ >= duration)
      {
        task.start_time_ = task.left_boundary_time_;
        task.end_time_ = task.left_boundary_time_ + duration;
        current_state.current_schedule_.tasks_tree_.push(task.start_time_, task);
        current_state.id_start_time_[task.id_] = task.start_time_;
        return true;
      }
    }
    goltsov::DateTime start = task.left_boundary_time_;
    while (current == current_state.current_schedule_.tasks_tree_.end() || (* current).first + duration < task.right_boundary_time_)
    {
      if (current == current_state.current_schedule_.tasks_tree_.end())
      {
        if (start + duration <= task.right_boundary_time_)
        {
          task.start_time_ = start;
          task.end_time_ = task.start_time_ + duration;
          current_state.current_schedule_.tasks_tree_.push(task.start_time_, task);
          current_state.id_start_time_[task.id_] = task.start_time_;
          return true;
        }
        else
        {
          return false;
        }
      }
      else if (((* current).first) > task.left_boundary_time_ && ((* current).first) - std::max(task.left_boundary_time_, (* current.prev()).first) >= duration)
      {
        task.start_time_ = std::max(task.left_boundary_time_, (* current.prev()).first);
        task.end_time_ = task.start_time_ + duration;
        current_state.current_schedule_.tasks_tree_.push(task.start_time_, task);
        current_state.id_start_time_[task.id_] = task.start_time_;
        return true;
      }
      else
      {
        if ((* current).second.is_protected_ || (* current).second.priority_ >= task.priority_)
        {
          start = (* current).second.end_time_;
          current = current.next();
        }
        else
        {
          current_state.current_schedule_.unplanned_tasks_.insert({current->second.id_, current->second});
          goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > temp = current;
          current = current.next();
          current_state.current_schedule_.tasks_tree_.drop((* temp).first);
        }
      }
    }
    return false;
  }
  goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > pushProtectedTask(goltsov::State& current_state, goltsov::Task& task)
  {
    goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > current = current_state.current_schedule_.tasks_tree_.find(detail::FindTask {task});
    while (current != current_state.current_schedule_.tasks_tree_.end() && (* current).first < task.start_time_)
    {
      if ((* current).second.is_protected_)
      {
        detail::pushUnplanned(current_state);
        return current;
      }
      else
      {
        current_state.current_schedule_.unplanned_tasks_.insert({current->second.id_, current->second});
        goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > temp = current;
        current = current.next();
        current_state.current_schedule_.tasks_tree_.drop((* current).first);
      }
    }
    detail::pushTask(current_state, task, task.right_boundary_time_ - task.left_boundary_time_);
    detail::pushUnplanned(current_state);
    return current_state.current_schedule_.tasks_tree_.end();
  }
  void pushUnplanned(goltsov::State& current_state)
  {
    for (goltsov::MapIterator< std::string, goltsov::Task > it = current_state.current_schedule_.unplanned_tasks_.begin(); it != current_state.current_schedule_.unplanned_tasks_.end(); ++it)
    {
      if (detail::pushTask(current_state, it->second, it->second.end_time_ - it->second.start_time_))
      {
        current_state.current_schedule_.unplanned_tasks_.erase(it->second.id_);
      }
    }
  }
  std::pair< size_t, size_t > mergeInterval(goltsov::State& current_state, goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > s, goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > e)
  {
    size_t added = 0;
    size_t conflicts = 0;
    for (; s != e; ++s)
    {
      if (!s->second.is_protected_)
      {
        if (pushTask(current_state, s->second, s->second.end_time_ - s->second.start_time_))
        {
          added += 1;
        }
        else
        {
          conflicts += 1;
        }
      }
      else
      {
        if (pushProtectedTask(current_state, s->second) == current_state.current_schedule_.tasks_tree_.end())
        {
          added += 1;
        }
        else
        {
          conflicts += 1;
        }
      }
    }
    return {added, conflicts};
  }
}

namespace goltsov
{
  void parsingAdd(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string id;
    std::string title, description;
    goltsov::DateTime left_boundary_time, right_boundary_time;
    goltsov::TimeInterval duration;
    size_t priority;
    is >> id;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> title;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> description;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> left_boundary_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> right_boundary_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> duration;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> priority;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    if (right_boundary_time < left_boundary_time || right_boundary_time - left_boundary_time < duration)
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    add(os, current_state, id, title, description, left_boundary_time, right_boundary_time, duration, priority);
  }
  void parsingAddProtected(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string id;
    std::string title, description;
    goltsov::DateTime start_time, end_time;
    is >> id;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> title;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> description;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> start_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> end_time;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    if (end_time < start_time)
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    addProtected(os, current_state, id, title, description, start_time, end_time);
  }
  void parsingRemove(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string id;
    is >> id;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    remove(os, current_state, id);
  }
  void parsingList(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    list(os, current_state);
  }
  void parsingMerge(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name;
    is >> schedule_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    merge(os, current_state, schedule_name);
  }
  void parsingShowUnplanned(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    showUnplanned(os, current_state);
  }
  void parsingUnplannedRemove(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string id;
    is >> id;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    unplannedRemove(os, current_state, id);
  }
  void parsingUnplannedForce(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string id;
    is >> id;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    unplannedForce(os, current_state, id);
  }
  void parsingMergeScheduleOtherContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name, context_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> context_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    mergeScheduleOtherContext(os, current_state, schedule_name, context_name);
  }
  void parsingAddScheduleOtherContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name, context_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> context_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    addScheduleOtherContext(os, current_state, schedule_name, context_name);
  }
  void parsingAddForceScheduleOtherContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name, context_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> context_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    addForceScheduleOtherContext(os, current_state, schedule_name, context_name);
  }
  void parsingSwitchSchedule(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name;
    is >> schedule_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    switchSchedule(os, current_state, schedule_name);
  }
  void parsingSwitchContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string context_name;
    is >> context_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    switchContext(os, current_state, context_name);
  }
  void parsingStats(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    goltsov::DateTime start_time, end_time;
    is >> start_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> end_time;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    stats(os, current_state, start_time, end_time);
  }
  void parsingNewSchedule(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string name_schedule;
    is >> name_schedule;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    newSchedule(os, current_state, name_schedule);
  }
  void parsingNewContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string name_context;
    is >> name_context;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    newContext(os, current_state, name_context);
  }
  void parsingLoadSchedule(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name, file_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> file_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    loadSchedule(os, current_state, schedule_name, file_name);
  }
  void parsingSaveSchedule(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string schedule_name, file_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> file_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    saveSchedule(os, current_state, schedule_name, file_name);
  }
  void parsingLoadContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string context_name, file_name;
    is >> context_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> file_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    loadContext(os, current_state, context_name, file_name);
  }
  void parsingSaveContext(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    std::string context_name, file_name;
    is >> context_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> file_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    saveContext(os, current_state, context_name, file_name);
  }
  void parsingFindGap(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    goltsov::TimeInterval gap;
    is >> gap;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    findGap(os, current_state, gap);
  }
  void parsingFindGapOnInterval(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    goltsov::DateTime start_time, end_time;
    goltsov::TimeInterval gap;
    is >> start_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> end_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> gap;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    findGapOnInterval(os, current_state, start_time, end_time, gap);
  }
  void parsingFindCommonGap(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    goltsov::TimeInterval gap;
    size_t count;
    topit::Vector< std::string > names_schedules;
    is >> gap;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> count;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    for (size_t i = 0; i + 1 < count; ++i)
    {
      std::string schedule_name;
      is >> schedule_name;
      if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
      {
        throw std::runtime_error("<INVALID COMMAND>");
      }
      names_schedules.pushBack(schedule_name);
    }
    std::string schedule_name;
    is >> schedule_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    names_schedules.pushBack(schedule_name);
    findCommonGap(os, current_state, gap, count, names_schedules);
  }
  void parsingFindCommonGapOnInterval(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    goltsov::DateTime start_time, end_time;
    goltsov::TimeInterval gap;
    size_t count;
    topit::Vector< std::string > names_schedules;
    is >> start_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> end_time;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> gap;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is >> count;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    for (size_t i = 0; i + 1 < count; ++i)
    {
      std::string schedule_name;
      is >> schedule_name;
      if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
      {
        throw std::runtime_error("<INVALID COMMAND>");
      }
      names_schedules.pushBack(schedule_name);
    }
    std::string schedule_name;
    is >> schedule_name;
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    names_schedules.pushBack(schedule_name);
    findCommonGap(os, current_state, gap, count, names_schedules);
  }
  void parsingExit(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    exit(os, current_state);
  }

  void add(std::ostream& os, goltsov::State& current_state, const std::string& id, const std::string& title, const std::string& description,
    const goltsov::DateTime& left_boundary_time, const goltsov::DateTime& right_boundary_time, const goltsov::TimeInterval& duration, const size_t& priority)
  {
    Task temp {id, title, description, left_boundary_time, right_boundary_time, left_boundary_time, left_boundary_time + duration, priority, false};
    bool is_planned = detail::pushTask(current_state, temp, duration);
    if (is_planned)
    {
      goltsov::DateTime k = current_state.id_start_time_[id];
      os << "<TASK ADDED: Scheduled at " << current_state.current_schedule_.tasks_tree_.get(k)->second.start_time_ << ' ' << current_state.current_schedule_.tasks_tree_.get(k)->second.end_time_ << '\n';
    }
    else
    {
      current_state.current_schedule_.unplanned_tasks_.insert({temp.id_, temp});
      os << "<TASK UNPLANNED>\n";
    }
    detail::pushUnplanned(current_state);
  }
  void addProtected(std::ostream& os, goltsov::State& current_state, const std::string& id, const std::string& title, const std::string& description,
    const goltsov::DateTime& left_boundary_time, const goltsov::DateTime& right_boundary_time)
  {
    Task task {id, title, description, left_boundary_time, right_boundary_time, left_boundary_time, right_boundary_time, 0, true};
    RBTIterator< goltsov::DateTime, goltsov::Task > res = detail::pushProtectedTask(current_state, task);
    if (res == current_state.current_schedule_.tasks_tree_.end())
    {
      os << "<TASK ADDED: Scheduled at " << left_boundary_time << ' ' << right_boundary_time << '\n';
    }
    else
    {
      os << "<TASK DID NOT ADDED: Allready scheduled " << (* res).second.title_ << " at " << (* res).second.start_time_ << ' ' << (* res).second.end_time_ << '\n';
    }
  }
  void remove(std::ostream& os, goltsov::State& current_state, const std::string& id)
  {
    goltsov::DateTime k = current_state.id_start_time_[id];
    current_state.current_schedule_.tasks_tree_.drop(k);
    os << "<TASK REMOVED: ID " << id << '\n';
  }
  void list(std::ostream& os, goltsov::State& current_state)
  {
    for (RBTIterator< goltsov::DateTime, goltsov::Task > it = current_state.current_schedule_.tasks_tree_.begin(); it != current_state.current_schedule_.tasks_tree_.end(); ++it)
    {
      os << (* it).second << '\n';
    }
  }
  void merge(std::ostream& os, goltsov::State& current_state, const std::string& name_other_schedule)
  {
    goltsov::Schedule* other_schedule;
    try
    {
      other_schedule = & ((* current_state.current_context_.schedules_tree_.get(name_other_schedule)).second);
    }
    catch (...)
    {
      os << "<INVALID COMMAND>\n";
    }
    size_t added = 0;
    for (RBTIterator< goltsov::DateTime, goltsov::Task > it = other_schedule->tasks_tree_.begin(); it != other_schedule->tasks_tree_.end(); ++it)
    {
      if (it->second.is_protected_)
      {
        if (detail::pushTask(current_state, it->second, it->second.end_time_ - it->second.start_time_))
        {
          added += 1;
        }
      }
      else
      {
        if (detail::pushProtectedTask(current_state, it->second) == other_schedule->tasks_tree_.end())
        {
          added += 1;
        }
      }
    }
    os << "<MERGE DONE. Added: " << added << ", Conflicts: " << current_state.current_schedule_.unplanned_tasks_.size() << ">\n";
  }
  void showUnplanned(std::ostream& os, goltsov::State& current_state)
  {
    for (MapIterator< std::string, goltsov::Task > it = current_state.current_schedule_.unplanned_tasks_.begin(); it != current_state.current_schedule_.unplanned_tasks_.end(); ++it)
    {
      goltsov::Task& a = it->second;
      os << "<UNPLANNED: id=" << a.id_ << " Task=\"" << a.title_ << "\", left_boundary_time=" << a.left_boundary_time_
        << ", right_boundary_time=" << a.right_boundary_time_ << ", duration="
        << a.right_boundary_time_ - a.left_boundary_time_ << ", priority=" << a.priority_ << ", is_protected="
        << a.is_protected_ << ">\n";
    }
  }
  void unplannedRemove(std::ostream& os, goltsov::State& current_state, const std::string& id)
  {
    try
    {
      current_state.current_schedule_.unplanned_tasks_.erase(id);
    }
    catch (...)
    {
      os << "<INVALID COMMAND>\n";
    }
  }
  void unplannedForce(std::ostream& os, goltsov::State& current_state, const std::string& id)
  {
    goltsov::Task task;
    try
    {
      task = current_state.current_schedule_.unplanned_tasks_.at(id);
      current_state.current_schedule_.unplanned_tasks_.erase(id);
      size_t now_size_unplanned = current_state.current_schedule_.unplanned_tasks_.size();
      RBTIterator< goltsov::DateTime, goltsov::Task > res = detail::pushProtectedTask(current_state, task);
      if (res == current_state.current_schedule_.tasks_tree_.end())
      {
        os << "<FORCED ADD: " << task.title_ << ", NEW UNPLANNED: " << current_state.current_schedule_.unplanned_tasks_.size() - now_size_unplanned << ">\n";
      }
      else
      {
        os << "<FORCED ADD FAILED: " << res->second.title_ << " allready scheduled at " << res->second.start_time_ << ' ' << res->second.end_time_ << ">\n";
      }
    }
    catch (...)
    {
      os << "<NO TASK>\n";
    }
  }
  void mergeScheduleOtherContext(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name, const std::string& context_name)
  {
    goltsov::Context context;
    goltsov::Schedule schedule;
    try
    {
      context = current_state.contexts_tree_.get(context_name)->second;
    }
    catch (...)
    {
      os << "<NO CONTEXT>\n";
      return;
    }
    try
    {
      schedule = context.schedules_tree_.get(schedule_name)->second;
    }
    catch (...)
    {
      os << "<NO SCHEDULE>\n";
      return;
    }
    std::pair< size_t, size_t > res = detail::mergeInterval(current_state, schedule.tasks_tree_.begin(), schedule.tasks_tree_.end());
    os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">";
  }
  void addScheduleOtherContext(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name, const std::string& context_name)
  {
    goltsov::Context context;
    goltsov::Schedule schedule;
    try
    {
      context = current_state.contexts_tree_.get(context_name)->second;
    }
    catch (...)
    {
      os << "<NO CONTEXT>\n";
      return;
    }
    try
    {
      schedule = context.schedules_tree_.get(schedule_name)->second;
    }
    catch (...)
    {
      os << "<NO SCHEDULE>\n";
      return;
    }
    try
    {
      current_state.current_context_.schedules_tree_.get(schedule_name);
      os << "<SCHEDULE ALLREADY EXISTS>\n";
    }
    catch (...)
    {
      current_state.current_context_.schedules_tree_.push(schedule_name, schedule);
      os << "<ADD DONE. Added: " << schedule.tasks_tree_.size() << ", Conflicts: " << schedule.unplanned_tasks_.size() << ">\n";
    }
  }
  void addForceScheduleOtherContext(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name, const std::string& context_name)
  {
    try
    {
      current_state.current_context_.schedules_tree_.get(schedule_name);
      current_state.current_context_.schedules_tree_.drop(schedule_name);
    }
    catch (...)
    {}
    addScheduleOtherContext(os, current_state, schedule_name, context_name);
  }
  void switchSchedule(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name)
  {
    try
    {
      goltsov::Schedule schedule = current_state.current_context_.schedules_tree_.get(schedule_name)->second;
      current_state.current_schedule_ = schedule;
      os << "<SCHEDULE SWITCHED>\n";
    }
    catch (...)
    {
      os << "<NO SCHEDULE>\n";
    }
  }
  void switchContext(std::ostream& os, goltsov::State& current_state, const std::string& context_name)
  {
    try
    {
      goltsov::Context context = current_state.contexts_tree_.get(context_name)->second;
      current_state.current_context_ = context;
      current_state.current_schedule_ = current_state.current_context_.schedules_tree_.begin()->second;
      os << "<CONTEXT SWITCHED>\n";
    }
    catch (...)
    {
      os << "<NO CONTEXT>\n";
    }
  }
  void stats(std::ostream&, goltsov::State& current_state, const goltsov::DateTime&, const goltsov::DateTime&)
  {}
  void newSchedule(std::ostream&, goltsov::State& current_state, const std::string&)
  {}
  void newContext(std::ostream&, goltsov::State& current_state, const std::string&)
  {}
  void loadSchedule(std::ostream&, goltsov::State& current_state, const std::string&, const std::string&)
  {}
  void saveSchedule(std::ostream&, goltsov::State& current_state, const std::string&, const std::string&)
  {}
  void loadContext(std::ostream&, goltsov::State& current_state, const std::string&, const std::string&)
  {}
  void saveContext(std::ostream&, goltsov::State& current_state, const std::string&, const std::string&)
  {}
  void findGap(std::ostream&, goltsov::State& current_state, const goltsov::TimeInterval&)
  {}
  void findGapOnInterval(std::ostream&, goltsov::State& current_state, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&)
  {}
  void findCommonGap(std::ostream&, goltsov::State& current_state, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&)
  {}
  void findCommonGapOnInterval(std::ostream&, goltsov::State& current_state, const goltsov::DateTime&, const goltsov::DateTime&, const goltsov::TimeInterval&, const size_t&, const topit::Vector< std::string >&)
  {}
  void exit(std::ostream&, goltsov::State& current_state)
  {}
}
