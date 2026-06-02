#include <stdexcept>
#include <iomanip>
#include <fstream>
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
    if (current_state.current_schedule_->tasks_tree_.size() == 0)
    {
      task.start_time_ = task.left_boundary_time_;
      task.end_time_ = task.start_time_ + duration;
      current_state.current_schedule_->tasks_tree_.push(task.start_time_, task);
      current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
      return true;
    }
    goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > current = current_state.current_schedule_->tasks_tree_.rfind(detail::FindTask {task});
    if (current == current_state.current_schedule_->tasks_tree_.end())
    {
      current = current_state.current_schedule_->tasks_tree_.begin();
    }
    else
    {
      current = current.next();
    }
    goltsov::DateTime start = task.left_boundary_time_;
    while (current != current_state.current_schedule_->tasks_tree_.end() && start + duration > current->second.start_time_ && start + duration <= task.right_boundary_time_)
    {
      if (task.priority_ > current->second.priority_ && !current->second.is_protected_)
      {
        current_state.current_schedule_->unplanned_tasks_.insert({current->second.id_, current->second});
        goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > temp = current;
        ++current;
        current_state.current_schedule_->id_start_time_.erase(temp->second.id_);
        current_state.current_schedule_->tasks_tree_.drop(temp->first);
      }
      else
      {
        start = current->second.end_time_;
        ++current;
      }
    }
    if (current == current_state.current_schedule_->tasks_tree_.end() && start + duration <= task.right_boundary_time_
        || current != current_state.current_schedule_->tasks_tree_.end() && start + duration <= current->second.start_time_ && start + duration <= task.right_boundary_time_)
    {
      task.start_time_ = start;
      task.end_time_ = start + duration;
      current_state.current_schedule_->tasks_tree_.push(task.start_time_, task);
      current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
      return true;
    }
    else
    {
      return false;
    }
  }
  goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > pushProtectedTask(goltsov::State& current_state, goltsov::Task& task)
  {
    goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > current = current_state.current_schedule_->tasks_tree_.rfind(detail::FindTask {task});
    if (current == current_state.current_schedule_->tasks_tree_.end())
    {
      current = current_state.current_schedule_->tasks_tree_.begin();
    }
    else
    {
      current = current.next();
    }
    while (current != current_state.current_schedule_->tasks_tree_.end() && task.right_boundary_time_ > current->second.start_time_)
    {
      if (current->second.is_protected_)
      {
        current_state.current_schedule_->unplanned_tasks_.insert({task.id_, task});
        return current;
      }
      else
      {
        current_state.current_schedule_->unplanned_tasks_.insert({current->second.id_, current->second});
        goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > temp = current;
        ++current;
        current_state.current_schedule_->id_start_time_.erase(temp->second.id_);
        current_state.current_schedule_->tasks_tree_.drop(temp->first);
      }
    }
    task.start_time_ = task.left_boundary_time_;
    task.end_time_ = task.right_boundary_time_;
    current_state.current_schedule_->tasks_tree_.push(task.start_time_, task);
    current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
    return current_state.current_schedule_->tasks_tree_.end();
  }
  goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > pushProtectedTaskForce(goltsov::State& current_state, goltsov::Task& task)
  {
    goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > current = current_state.current_schedule_->tasks_tree_.rfind(detail::FindTask {task});
    if (current == current_state.current_schedule_->tasks_tree_.end())
    {
      current = current_state.current_schedule_->tasks_tree_.begin();
    }
    else
    {
      current = current.next();
    }
    while (current != current_state.current_schedule_->tasks_tree_.end() && task.right_boundary_time_ > current->second.start_time_)
    {
      current_state.current_schedule_->unplanned_tasks_.insert({current->second.id_, current->second});
      goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > temp = current;
      ++current;
      current_state.current_schedule_->id_start_time_.erase(temp->second.id_);
      current_state.current_schedule_->tasks_tree_.drop(temp->first);
    }
    task.start_time_ = task.left_boundary_time_;
    task.end_time_ = task.right_boundary_time_;
    current_state.current_schedule_->tasks_tree_.push(task.start_time_, task);
    current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
    return current_state.current_schedule_->tasks_tree_.end();
  }
  void pushUnplanned(goltsov::State& current_state)
  {
    size_t count_pushed = 1;
    while (count_pushed)
    {
      count_pushed = 0;
      for (goltsov::MapIterator< std::string, goltsov::Task > it = current_state.current_schedule_->unplanned_tasks_.begin(); it != current_state.current_schedule_->unplanned_tasks_.end(); ++it)
      {
        if (detail::pushTask(current_state, it->second, it->second.end_time_ - it->second.start_time_))
        {
          current_state.current_schedule_->unplanned_tasks_.erase(it->second.id_);
          ++count_pushed;
        }
      }
    }
  }
  std::pair< size_t, size_t > mergeInterval(goltsov::State& current_state, goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > s, goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > e)
  {
    size_t count_planned0 = current_state.current_schedule_->tasks_tree_.size();
    for (; s != e; ++s)
    {
      if (!s->second.is_protected_)
      {
        pushTask(current_state, s->second, s->second.end_time_ - s->second.start_time_);
      }
      else
      {
        pushProtectedTask(current_state, s->second) == current_state.current_schedule_->tasks_tree_.end();
      }
    }
    detail::pushUnplanned(current_state);
    size_t count_unplanned = current_state.current_schedule_->tasks_tree_.size();
    size_t count_planned1 = current_state.current_schedule_->tasks_tree_.size();
    return {count_planned1 - count_planned0, count_unplanned};
  }
std::pair< goltsov::DateTime, goltsov::DateTime > findCommonGapInVector(
    goltsov::State& current_state, 
    const goltsov::DateTime& start_time, 
    const goltsov::DateTime& end_time, 
    const goltsov::TimeInterval& duration, 
    const topit::Vector< std::string >& names_schedules)
  {
    topit::Vector< goltsov::Task > all_tasks;
    for (size_t i = 0; i < names_schedules.getSize(); ++i)
    {
      goltsov::Schedule& current_schedule = current_state.current_context_->schedules_tree_.get(names_schedules[i])->second;
      
      for (auto it = current_schedule.tasks_tree_.begin(); it != current_schedule.tasks_tree_.end(); ++it)
      {
        if (it->second.end_time_ > start_time && it->second.start_time_ < end_time)
        {
          all_tasks.pushBack(it->second);
        }
      }
    }
    for (size_t i = 0; i < all_tasks.getSize(); ++i)
    {
      for (size_t j = i + 1; j < all_tasks.getSize(); ++j)
      {
        if (all_tasks[j].start_time_ < all_tasks[i].start_time_)
        {
          std::swap(all_tasks[i], all_tasks[j]);
        }
      }
    }
    goltsov::DateTime current = start_time;
    for (size_t i = 0; i < all_tasks.getSize(); ++i)
    {
      goltsov::Task& task = all_tasks[i];
      goltsov::DateTime task_start = (task.start_time_ < start_time) ? start_time : task.start_time_;
      goltsov::DateTime task_end = (task.end_time_ > end_time) ? end_time : task.end_time_;
      if (task_start > current)
      {
        if (task_start - current >= duration)
        {
          return { current, current + duration };
        }
      }
      if (task_end > current)
      {
        current = task_end;
      }
    }
    if (end_time > current)
    {
      if (end_time - current >= duration)
      {
        return { current, current + duration };
      }
    }
    throw std::runtime_error("<GAP DID NOT FOUND>");
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
    findCommonGapOnInterval(os, current_state, start_time, end_time, gap, count, names_schedules);
  }
  void parsingExit(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    is.setstate(std::ios_base::eofbit);
  }
  void parsingListSchedules(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    listSchedules(os, current_state);
  }
  void parsingListContexts(std::istream& is, std::ostream& os, goltsov::State& current_state)
  {
    if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    listContexts(os, current_state);
  }

  void add(std::ostream& os, goltsov::State& current_state, const std::string& id, const std::string& title, const std::string& description,
    const goltsov::DateTime& left_boundary_time, const goltsov::DateTime& right_boundary_time, const goltsov::TimeInterval& duration, const size_t& priority)
  {
    Task temp {id, title, description, left_boundary_time, right_boundary_time, left_boundary_time, left_boundary_time + duration, priority, false};
    bool is_planned = detail::pushTask(current_state, temp, duration);
    if (is_planned)
    {
      goltsov::DateTime k = current_state.current_schedule_->id_start_time_[id];
      os << "<TASK ADDED: Scheduled at " << current_state.current_schedule_->tasks_tree_.get(k)->second.start_time_ << ' ' << current_state.current_schedule_->tasks_tree_.get(k)->second.end_time_ << ">\n";
    }
    else
    {
      current_state.current_schedule_->unplanned_tasks_.insert({temp.id_, temp});
      os << "<TASK UNPLANNED>\n";
    }
    detail::pushUnplanned(current_state);
  }
  void addProtected(std::ostream& os, goltsov::State& current_state, const std::string& id, const std::string& title, const std::string& description,
    const goltsov::DateTime& left_boundary_time, const goltsov::DateTime& right_boundary_time)
  {
    Task task {id, title, description, left_boundary_time, right_boundary_time, left_boundary_time, right_boundary_time, 0, true};
    RBTIterator< goltsov::DateTime, goltsov::Task > res = detail::pushProtectedTask(current_state, task);
    if (res == current_state.current_schedule_->tasks_tree_.end())
    {
      os << "<TASK ADDED: Scheduled at " << left_boundary_time << ' ' << right_boundary_time << ">\n";
    }
    else
    {
      os << "<TASK DID NOT ADDED: Allready scheduled " << (* res).second.title_ << " at " << (* res).second.start_time_ << ' ' << (* res).second.end_time_ << ">\n";
    }
    detail::pushUnplanned(current_state);
  }
  void remove(std::ostream& os, goltsov::State& current_state, const std::string& id)
  {
    goltsov::DateTime k = current_state.current_schedule_->id_start_time_[id];
    current_state.current_schedule_->tasks_tree_.drop(k);
    os << "<TASK REMOVED: ID " << id << ">\n";
  }
  void list(std::ostream& os, goltsov::State& current_state)
  {
    for (RBTIterator< goltsov::DateTime, goltsov::Task > it = current_state.current_schedule_->tasks_tree_.begin(); it != current_state.current_schedule_->tasks_tree_.end(); ++it)
    {
      os << (* it).second << '\n';
    }
  }
  void merge(std::ostream& os, goltsov::State& current_state, const std::string& name_other_schedule)
  {
    goltsov::Schedule* other_schedule;
    try
    {
      other_schedule = & ((* current_state.current_context_->schedules_tree_.get(name_other_schedule)).second);
    }
    catch (...)
    {
      os << "<INVALID COMMAND>\n";
    }
    std::pair< size_t, size_t > res = detail::mergeInterval(current_state, other_schedule->tasks_tree_.begin(), other_schedule->tasks_tree_.end());
    os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">\n";
  }
  void showUnplanned(std::ostream& os, goltsov::State& current_state)
  {
    for (MapIterator< std::string, goltsov::Task > it = current_state.current_schedule_->unplanned_tasks_.begin(); it != current_state.current_schedule_->unplanned_tasks_.end(); ++it)
    {
      goltsov::Task& a = it->second;
      os << "<UNPLANNED: id=" << a.id_ << " Task=\"" << a.title_ << "\", left_boundary_time=" << a.left_boundary_time_
        << ", right_boundary_time=" << a.right_boundary_time_ << ", duration="
        << a.end_time_ - a.start_time_ << ", priority=" << a.priority_ << ", is_protected="
        << a.is_protected_ << ">\n";
    }
  }
  void unplannedRemove(std::ostream& os, goltsov::State& current_state, const std::string& id)
  {
    try
    {
      current_state.current_schedule_->unplanned_tasks_.erase(id);
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
      task = current_state.current_schedule_->unplanned_tasks_.at(id);
      current_state.current_schedule_->unplanned_tasks_.erase(id);
      size_t now_size_unplanned = current_state.current_schedule_->unplanned_tasks_.size();
      RBTIterator< goltsov::DateTime, goltsov::Task > res;
      if (task.is_protected_)
      {
        res = detail::pushProtectedTaskForce(current_state, task);
      }
      else
      {
        res = detail::pushProtectedTask(current_state, task);
      }
      if (res == current_state.current_schedule_->tasks_tree_.end())
      {
        os << "<FORCED ADD: " << task.title_ << ", NEW UNPLANNED: " << current_state.current_schedule_->unplanned_tasks_.size() - now_size_unplanned << ">\n";
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
    os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">\n";
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
      current_state.current_context_->schedules_tree_.get(schedule_name);
      os << "<SCHEDULE ALLREADY EXISTS>\n";
    }
    catch (...)
    {
      current_state.current_context_->schedules_tree_.push(schedule_name, schedule);
      os << "<ADD DONE. Added: " << schedule.tasks_tree_.size() << ", Conflicts: " << schedule.unplanned_tasks_.size() << ">\n";
    }
  }
  void addForceScheduleOtherContext(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name, const std::string& context_name)
  {
    bool is_current_schedule = false;
    try
    {
      current_state.current_context_->schedules_tree_.get(schedule_name);
      if (current_state.current_context_->name_context_ != context_name)
      {
        is_current_schedule = current_state.current_schedule_->name_schedule_ == schedule_name;
        current_state.current_context_->schedules_tree_.drop(schedule_name);
      }
    }
    catch (...)
    {}
    addScheduleOtherContext(os, current_state, schedule_name, context_name);
    if (is_current_schedule)
    {
      current_state.current_schedule_ = & current_state.current_context_->schedules_tree_.get(schedule_name)->second;
    }
  }
  void switchSchedule(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name)
  {
    try
    {
      current_state.current_schedule_ = & current_state.current_context_->schedules_tree_.get(schedule_name)->second;
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
      current_state.current_context_ = & current_state.contexts_tree_.get(context_name)->second;
      current_state.current_schedule_ = & current_state.current_context_->schedules_tree_.begin()->second;
      os << "<CONTEXT SWITCHED>\n";
    }
    catch (...)
    {
      os << "<NO CONTEXT>\n";
    }
  }
  void stats(std::ostream& os, goltsov::State& current_state, const goltsov::DateTime& start_time, const goltsov::DateTime& end_time)
  {
    goltsov::Task a;
    a.start_time_ = start_time;
    goltsov::RBTIterator< goltsov::DateTime, goltsov::Task > current = current_state.current_schedule_->tasks_tree_.rfind(detail::FindTask {a});
    goltsov::TimeInterval busy_time {0, 0, 0, 0, 0, 0};
    size_t count_tasks = 0;
    try
    {
      for (; current != current_state.current_schedule_->tasks_tree_.end() && current->second.start_time_ < end_time; ++current)
      {
        busy_time = busy_time + (std::max(end_time, current->second.end_time_) - current->second.start_time_);
        count_tasks += 1;
      }
    }
    catch (...)
    {
      os << "<INVALID COMMAND>\n";
      return;
    }
    os << "<Tasks: " << count_tasks << ", Load: ";
    os << std::fixed << std::setprecision(2) << (busy_time / (end_time - start_time)) * 100 << "%>\n";
  }
  void newSchedule(std::ostream& os, goltsov::State& current_state, const std::string& name_schedule)
  {
    current_state.current_context_->schedules_tree_.push(name_schedule, goltsov::Schedule {});
    current_state.current_context_->schedules_tree_.get(name_schedule)->second.name_schedule_ = name_schedule;
  }
  void newContext(std::ostream& os, goltsov::State& current_state, const std::string& name_context)
  {
    current_state.contexts_tree_.push(name_context, goltsov::Context {});
    current_state.contexts_tree_.get(name_context)->second.name_context_ = name_context;
    current_state.contexts_tree_.get(name_context)->second.schedules_tree_.push("Base_schedule", goltsov::Schedule {});
    current_state.contexts_tree_.get(name_context)->second.schedules_tree_.get("Base_schedule")->second.name_schedule_ = "Base_schedule";
  }
  void loadSchedule(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name, const std::string& filename)
  {
    try
    {
      current_state.current_context_->schedules_tree_.get(schedule_name);
      os << "<NAME IS OCCUPIED>\n";
    }
    catch (...)
    {
      goltsov::Schedule schedule;
      schedule.name_schedule_ = schedule_name;
      std::fstream inp_f (filename);
      inp_f >> schedule;
      current_state.current_context_->schedules_tree_.push(schedule_name, schedule);
      current_state.current_schedule_ = & current_state.current_context_->schedules_tree_.get(schedule_name)->second;
      os << "<SCHEDULE LOADED>\n";
    }
  }
  void saveSchedule(std::ostream& os, goltsov::State& current_state, const std::string& schedule_name, const std::string& filename)
  {
    try
    {
      goltsov::Schedule schedule = current_state.current_context_->schedules_tree_.get(schedule_name)->second;
      std::fstream out_f (filename, std::ios::out | std::ios::trunc);
      if (!out_f.is_open())
      {
        throw std::runtime_error("Cannot open file");
      }
      out_f << schedule << '\n';
      os << "<SCHEDULE SAVED>\n";
    }
    catch (...)
    {
      os << "<NO SUCH SCHEDULE>\n";
    }
  }
  void loadContext(std::ostream& os, goltsov::State& current_state, const std::string& context_name, const std::string& filename)
  {
    try
    {
      goltsov::Context context = current_state.contexts_tree_.get(context_name)->second;
      os << "<NAME IS OCCUPIED>\n";
    }
    catch (...)
    {
      goltsov::Context context;
      context.name_context_ = context_name;
      std::fstream inp_f (filename);
      inp_f >> context;
      current_state.contexts_tree_.push(context_name, context);
      current_state.contexts_tree_.get(context_name)->second.name_context_ = context_name;
      current_state.current_context_ = & current_state.contexts_tree_.get(context_name)->second;
      current_state.current_schedule_ = & current_state.current_context_->schedules_tree_.begin()->second;
      os << "<CONTEXT LOADED>\n";
    }
  }
  void saveContext(std::ostream& os, goltsov::State& current_state, const std::string& context_name, const std::string& filename)
  {
    try
    {
      goltsov::Context context = current_state.contexts_tree_.get(context_name)->second;
      std::fstream out_f (filename, std::ios::out | std::ios::trunc);
      if (!out_f.is_open())
      {
        throw std::runtime_error("Cannot open file");
      }
      out_f << context << '\n';
      os << "<CONTEXT SAVED>\n";
    }
    catch (...)
    {
      os << "<NO SUCH CONTEXT>\n";
    }
  }
  void findGap(std::ostream& os, goltsov::State& current_state, const goltsov::TimeInterval& interval)
  {
    try
    {
      topit::Vector< std::string > a;
      a.pushBack(current_state.current_schedule_->name_schedule_);
      std::pair< goltsov::DateTime, goltsov::DateTime > gap = detail::findCommonGapInVector(current_state, current_state.current_time, current_state.current_schedule_->tasks_tree_.rfind([](const std::pair<goltsov::DateTime, goltsov::Task>& element){return true;})->second.end_time_ + interval, interval, a);
      os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what();
    }
  }
  void findGapOnInterval(std::ostream& os, goltsov::State& current_state, const goltsov::DateTime& start_time, const goltsov::DateTime& end_time, const goltsov::TimeInterval& interval)
  {
    try
    {
      topit::Vector< std::string > a;
      a.pushBack(current_state.current_schedule_->name_schedule_);
      std::pair< goltsov::DateTime, goltsov::DateTime > gap = detail::findCommonGapInVector(current_state, start_time, end_time, interval, a);
      os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what();
    }
  }
  void findCommonGap(std::ostream& os, goltsov::State& current_state, const goltsov::TimeInterval& interval, const size_t& count_schedules, const topit::Vector< std::string >& names_schedules)
  {
    try
    {
      std::pair< goltsov::DateTime, goltsov::DateTime > gap = detail::findCommonGapInVector(current_state, current_state.current_time, current_state.current_schedule_->tasks_tree_.rfind([](const std::pair<goltsov::DateTime, goltsov::Task>& element){return true;})->second.end_time_ + interval, interval, names_schedules);
      os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what();
    }
  }
  void findCommonGapOnInterval(std::ostream& os, goltsov::State& current_state, const goltsov::DateTime& start_time, const goltsov::DateTime& end_time, const goltsov::TimeInterval& interval, const size_t& count_schedules, const topit::Vector< std::string >& names_schedules)
  {
    try
    {
      std::pair< goltsov::DateTime, goltsov::DateTime > gap = detail::findCommonGapInVector(current_state, start_time, end_time, interval, names_schedules);
      os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what();
    }
  }
  void listSchedules(std::ostream& os, goltsov::State& current_state)
  {
    for (RBTIterator< std::string, goltsov::Schedule > it = current_state.current_context_->schedules_tree_.begin(); it != current_state.current_context_->schedules_tree_.end(); ++it)
    {
      if (current_state.current_schedule_->name_schedule_ == it->second.name_schedule_)
      {
        os << "* " << it->second.name_schedule_ << '\n';
      }
      else
      {
        os << "  " << it->second.name_schedule_ << '\n';
      }
    }
  }
  void listContexts(std::ostream& os, goltsov::State& current_state)
  {
    for (RBTIterator< std::string, goltsov::Context > it = current_state.contexts_tree_.begin(); it != current_state.contexts_tree_.end(); ++it)
    {
      if (current_state.current_context_->name_context_ == it->second.name_context_)
      {
        os << "* " << it->second.name_context_ << '\n';
      }
      else
      {
        os << "  " << it->second.name_context_ << '\n';
      }
    }
  }
}
