#include <stdexcept>
#include <iomanip>
#include <fstream>
#include "commands.hpp"
#include "structs.hpp"

std::istream& goltsov::detail::skipSpaces(std::istream& is)
{
  while (is.peek() == ' ')
  {
    is.get();
  }
  return is;
}
bool goltsov::detail::isEndOfLine(std::istream& is)
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
bool goltsov::detail::pushTask(State& current_state, Task& task, const TimeInterval& duration)
{
  if (current_state.current_schedule_->tasks_.size() == 0)
  {
    task.start_time_ = task.left_boundary_time_;
    task.end_time_ = task.start_time_ + duration;
    current_state.current_schedule_->tasks_.insert({task.start_time_, task});
    current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
    return true;
  }
  goltsov::MapIterator< DateTime, Task > current =
    rfindByPredicate(*current_state.current_schedule_, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule_->tasks_.end())
  {
    current = current_state.current_schedule_->tasks_.begin();
  }
  else
  {
    current = current.next();
  }
  DateTime start = task.left_boundary_time_;
  while (current != current_state.current_schedule_->tasks_.end()
    && start + duration > current->second.start_time_ && start + duration <= task.right_boundary_time_)
  {
    if (task.priority_ > current->second.priority_ && !current->second.is_protected_)
    {
      current_state.current_schedule_->unplanned_tasks_.insert({current->second.id_, current->second});
      goltsov::MapIterator< DateTime, Task > temp = current;
      ++current;
      current_state.current_schedule_->id_start_time_.erase(temp->second.id_);
      current_state.current_schedule_->tasks_.erase(temp->first);
    }
    else
    {
      start = current->second.end_time_;
      ++current;
    }
  }
  if (((current == current_state.current_schedule_->tasks_.end()) &&
    (start + duration <= task.right_boundary_time_))
      || ((current != current_state.current_schedule_->tasks_.end())
      && (start + duration <= current->second.start_time_ && start + duration <= task.right_boundary_time_)))
  {
    task.start_time_ = start;
    task.end_time_ = start + duration;
    current_state.current_schedule_->tasks_.insert({task.start_time_, task});
    current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
    return true;
  }
  else
  {
    return false;
  }
}
bool goltsov::detail::pushSoftTask(State& current_state, Task& task, const TimeInterval& duration)
{;
  if (current_state.current_schedule_->tasks_.empty())
  {
    task.start_time_ = task.left_boundary_time_;
    task.end_time_ = task.start_time_ + duration;
    current_state.current_schedule_->tasks_.insert({task.start_time_, task});
    current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
    return true;
  }
  MapIterator<DateTime, Task> current = rfindByPredicate(*current_state.current_schedule_, FindTaskHasETLessLB{task});
  current = (current == current_state.current_schedule_->tasks_.end()) ?
    current_state.current_schedule_->tasks_.begin() : current.next();
  DateTime potential_start = task.left_boundary_time_;
  while (current != current_state.current_schedule_->tasks_.end()
    && potential_start + duration <= task.right_boundary_time_)
  {
    if (potential_start + duration <= current->second.start_time_)
    {
      break;
    }
    potential_start = current->second.end_time_;
    ++current;
  }
  if (potential_start + duration <= task.right_boundary_time_)
  {
    task.start_time_ = potential_start;
    task.end_time_ = potential_start + duration;
    current_state.current_schedule_->tasks_.insert({task.start_time_, task});
    current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
    return true;
  }
  return pushTask(current_state, task, duration);
}
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::pushProtectedTask(
  State& current_state, Task& task)
{
  goltsov::MapIterator< DateTime, Task > current =
    rfindByPredicate(*current_state.current_schedule_, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule_->tasks_.end())
  {
    current = current_state.current_schedule_->tasks_.begin();
  }
  else
  {
    current = current.next();
  }
  while (current != current_state.current_schedule_->tasks_.end()
    && task.right_boundary_time_ > current->second.start_time_)
  {
    if (current->second.is_protected_)
    {
      current_state.current_schedule_->unplanned_tasks_.insert({task.id_, task});
      return current;
    }
    else
    {
      current_state.current_schedule_->unplanned_tasks_.insert({current->second.id_, current->second});
      goltsov::MapIterator< DateTime, Task > temp = current;
      ++current;
      current_state.current_schedule_->id_start_time_.erase(temp->second.id_);
      current_state.current_schedule_->tasks_.erase(temp->first);
    }
  }
  task.start_time_ = task.left_boundary_time_;
  task.end_time_ = task.right_boundary_time_;
  current_state.current_schedule_->tasks_.insert({task.start_time_, task});
  current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
  return current_state.current_schedule_->tasks_.end();
}
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::pushProtectedTaskForce(
  State& current_state, Task& task)
{
  goltsov::MapIterator< DateTime, Task > current =
    rfindByPredicate(*current_state.current_schedule_, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule_->tasks_.end())
  {
    current = current_state.current_schedule_->tasks_.begin();
  }
  else
  {
    current = current.next();
  }
  while (current != current_state.current_schedule_->tasks_.end()
    && task.right_boundary_time_ > current->second.start_time_)
  {
    current_state.current_schedule_->unplanned_tasks_.insert({current->second.id_, current->second});
    goltsov::MapIterator< DateTime, Task > temp = current;
    ++current;
    current_state.current_schedule_->id_start_time_.erase(temp->second.id_);
    current_state.current_schedule_->tasks_.erase(temp->first);
  }
  task.start_time_ = task.left_boundary_time_;
  task.end_time_ = task.right_boundary_time_;
  current_state.current_schedule_->tasks_.insert({task.start_time_, task});
  current_state.current_schedule_->id_start_time_[task.id_] = task.start_time_;
  return current_state.current_schedule_->tasks_.end();
}
void goltsov::detail::pushUnplanned(State& current_state)
{
  size_t count_pushed = 1;
  while (count_pushed)
  {
    count_pushed = 0;
    for (goltsov::MapIterator< std::string, Task > it =
      current_state.current_schedule_->unplanned_tasks_.begin();
      it != current_state.current_schedule_->unplanned_tasks_.end(); ++it)
    {
      if (pushSoftTask(current_state, it->second, it->second.end_time_ - it->second.start_time_))
      {
        current_state.current_schedule_->unplanned_tasks_.erase(it->second.id_);
        ++count_pushed;
      }
    }
  }
}
std::pair< size_t, size_t > goltsov::detail::mergeInterval(State& current_state,
  goltsov::MapIterator< DateTime, Task > s,
  goltsov::MapIterator< DateTime, Task > e)
{
  size_t count_planned0 = current_state.current_schedule_->tasks_.size();
  for (; s != e; ++s)
  {
    if (!s->second.is_protected_)
    {
      pushSoftTask(current_state, s->second, s->second.end_time_ - s->second.start_time_);
    }
    else
    {
      pushProtectedTask(current_state, s->second) == current_state.current_schedule_->tasks_.end();
    }
  }
  detail::pushUnplanned(current_state);
  size_t count_unplanned = current_state.current_schedule_->tasks_.size();
  size_t count_planned1 = current_state.current_schedule_->tasks_.size();
  return {count_planned1 - count_planned0, count_unplanned};
}
std::pair< goltsov::DateTime, goltsov::DateTime > goltsov::detail::findCommonGapInVector(
    State& current_state,
    const DateTime& start_time,
    const DateTime& end_time,
    const TimeInterval& duration,
    const topit::Vector< std::string >& names_schedules)
  {
    topit::Vector< Task > all_tasks;
    for (size_t i = 0; i < names_schedules.getSize(); ++i)
    {
      goltsov::Schedule& current_schedule =
        current_state.current_context_->schedules_.at(names_schedules[i]);
      for (MapIterator< DateTime, Task > it = current_schedule.tasks_.begin(); it != current_schedule.tasks_.end(); ++it)
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
    DateTime current = start_time;
    for (size_t i = 0; i < all_tasks.getSize(); ++i)
    {
      Task& task = all_tasks[i];
      DateTime task_start = (task.start_time_ < start_time) ? start_time : task.start_time_;
      DateTime task_end = (task.end_time_ > end_time) ? end_time : task.end_time_;
      if (task_start > current)
      {
        if (task_start - current >= duration)
        {
          return {current, current + duration};
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
        return {current, current + duration};
      }
    }
    throw std::runtime_error("<GAP DID NOT FOUND>");
  }
template< class Predicate >
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::findByPredicate(Schedule& schedule, Predicate pred)
{
  for (MapIterator< DateTime, Task > it = schedule.tasks_.begin(); it != schedule.tasks_.end(); ++it)
  {
    if (pred(*it))
    {
      return it;
    }
  }
  return schedule.tasks_.end();
}
template< class Predicate >
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::rfindByPredicate(Schedule& schedule, Predicate pred)
{
  for (MapIterator< DateTime, Task > it = schedule.tasks_.getLast(); it != schedule.tasks_.begin(); --it)
  {
    if (pred(*it))
    {
      return it;
    }
  }
  if (pred(*schedule.tasks_.begin()))
  {
    return schedule.tasks_.begin();
  }
  return schedule.tasks_.end();
}

void goltsov::parsingAdd(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  std::string title, description;
  DateTime left_boundary_time, right_boundary_time;
  TimeInterval duration;
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
void goltsov::parsingAddProtected(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  std::string title, description;
  DateTime start_time, end_time;
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
void goltsov::parsingRemove(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  is >> id;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  remove(os, current_state, id);
}
void goltsov::parsingList(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  list(os, current_state);
}
void goltsov::parsingMerge(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name;
  is >> schedule_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  merge(os, current_state, schedule_name);
}
void goltsov::parsingShowUnplanned(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  showUnplanned(os, current_state);
}
void goltsov::parsingUnplannedRemove(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  is >> id;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  unplannedRemove(os, current_state, id);
}
void goltsov::parsingUnplannedForce(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  is >> id;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  unplannedForce(os, current_state, id);
}
void goltsov::parsingMergeScheduleOtherContext(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingAddScheduleOtherContext(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingAddForceScheduleOtherContext(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingSwitchSchedule(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name;
  is >> schedule_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  switchSchedule(os, current_state, schedule_name);
}
void goltsov::parsingSwitchContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string context_name;
  is >> context_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  switchContext(os, current_state, context_name);
}
void goltsov::parsingStats(std::istream& is, std::ostream& os, State& current_state)
{
  DateTime start_time, end_time;
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
void goltsov::parsingNewSchedule(std::istream& is, std::ostream& os, State& current_state)
{
  std::string name_schedule;
  is >> name_schedule;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  newSchedule(os, current_state, name_schedule);
}
void goltsov::parsingNewContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string name_context;
  is >> name_context;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  newContext(os, current_state, name_context);
}
void goltsov::parsingLoadSchedule(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingSaveSchedule(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingLoadContext(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingSaveContext(std::istream& is, std::ostream& os, State& current_state)
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
void goltsov::parsingFindGap(std::istream& is, std::ostream& os, State& current_state)
{
  TimeInterval gap;
  is >> gap;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  findGap(os, current_state, gap);
}
void goltsov::parsingFindGapOnInterval(std::istream& is, std::ostream& os, State& current_state)
{
  DateTime start_time, end_time;
  TimeInterval gap;
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
void goltsov::parsingFindCommonGap(std::istream& is, std::ostream& os, State& current_state)
{
  TimeInterval gap;
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
void goltsov::parsingFindCommonGapOnInterval(std::istream& is, std::ostream& os, State& current_state)
{
  DateTime start_time, end_time;
  TimeInterval gap;
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
void goltsov::parsingExit(std::istream& is, std::ostream&, State&)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is.setstate(std::ios_base::eofbit);
}
void goltsov::parsingListSchedules(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  listSchedules(os, current_state);
}
void goltsov::parsingListContexts(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  listContexts(os, current_state);
}
void goltsov::add(std::ostream& os, State& current_state, const std::string& id, const std::string& title,
  const std::string& description, const DateTime& left_boundary_time,
  const DateTime& right_boundary_time, const TimeInterval& duration, const size_t& priority)
{
  Task temp{id, title, description, left_boundary_time, right_boundary_time, left_boundary_time,
    left_boundary_time + duration, priority, false};
  bool is_planned = detail::pushSoftTask(current_state, temp, duration);
  if (is_planned)
  {
    DateTime k = current_state.current_schedule_->id_start_time_[id];
    os << "<TASK ADDED: Scheduled at " << current_state.current_schedule_->tasks_.at(k).start_time_
      << ' ' << current_state.current_schedule_->tasks_.at(k).end_time_ << ">\n";
  }
  else
  {
    current_state.current_schedule_->unplanned_tasks_.insert({temp.id_, temp});
    os << "<TASK UNPLANNED>\n";
  }
  detail::pushUnplanned(current_state);
}
void goltsov::addProtected(std::ostream& os, State& current_state, const std::string& id, const std::string& title,
  const std::string& description, const DateTime& left_boundary_time,
  const DateTime& right_boundary_time)
{
  Task task{id, title, description, left_boundary_time, right_boundary_time, left_boundary_time,
    right_boundary_time, 0, true};
  MapIterator< DateTime, Task > res = detail::pushProtectedTask(current_state, task);
  if (res == current_state.current_schedule_->tasks_.end())
  {
    os << "<TASK ADDED: Scheduled at " << left_boundary_time << ' ' << right_boundary_time << ">\n";
  }
  else
  {
    os << "<TASK DID NOT ADDED: Allready scheduled " << (* res).second.title_ << " at " << (* res).second.start_time_
      << ' ' << (* res).second.end_time_ << ">\n";
  }
  detail::pushUnplanned(current_state);
}
void goltsov::remove(std::ostream& os, State& current_state, const std::string& id)
{
  DateTime k = current_state.current_schedule_->id_start_time_[id];
  current_state.current_schedule_->tasks_.erase(k);
  os << "<TASK REMOVED: ID " << id << ">\n";
}
void goltsov::list(std::ostream& os, State& current_state)
{
  for (MapIterator< DateTime, Task > it = current_state.current_schedule_->tasks_.begin();
    it != current_state.current_schedule_->tasks_.end(); ++it)
  {
    os << (* it).second << '\n';
  }
}
void goltsov::merge(std::ostream& os, State& current_state, const std::string& name_other_schedule)
{
  goltsov::Schedule* other_schedule;
  try
  {
    other_schedule = &(current_state.current_context_->schedules_.at(name_other_schedule));
  }
  catch (...)
  {
    os << "<INVALID COMMAND>\n";
  }
  std::pair< size_t, size_t > res = detail::mergeInterval(current_state, other_schedule->tasks_.begin(),
    other_schedule->tasks_.end());
  os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">\n";
}
void goltsov::showUnplanned(std::ostream& os, State& current_state)
{
  for (MapIterator< std::string, Task > it = current_state.current_schedule_->unplanned_tasks_.begin();
    it != current_state.current_schedule_->unplanned_tasks_.end(); ++it)
  {
    Task& a = it->second;
    os << "<UNPLANNED: id=" << a.id_ << " Task=\"" << a.title_ << "\", left_boundary_time=" << a.left_boundary_time_
      << ", right_boundary_time=" << a.right_boundary_time_ << ", duration="
      << a.end_time_ - a.start_time_ << ", priority=" << a.priority_ << ", is_protected="
      << a.is_protected_ << ">\n";
  }
}
void goltsov::unplannedRemove(std::ostream& os, State& current_state, const std::string& id)
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
void goltsov::unplannedForce(std::ostream& os, State& current_state, const std::string& id)
{
  Task task;
  try
  {
    task = current_state.current_schedule_->unplanned_tasks_.at(id);
    current_state.current_schedule_->unplanned_tasks_.erase(id);
    size_t now_size_unplanned = current_state.current_schedule_->unplanned_tasks_.size();
    MapIterator< DateTime, Task > res;
    if (task.is_protected_)
    {
      res = detail::pushProtectedTaskForce(current_state, task);
    }
    else
    {
      res = detail::pushProtectedTask(current_state, task);
    }
    if (res == current_state.current_schedule_->tasks_.end())
    {
      os << "<FORCED ADD: " << task.title_ << ", NEW UNPLANNED: "
        << current_state.current_schedule_->unplanned_tasks_.size() - now_size_unplanned << ">\n";
    }
    else
    {
      os << "<FORCED ADD FAILED: " << res->second.title_ << " allready scheduled at "
        << res->second.start_time_ << ' ' << res->second.end_time_ << ">\n";
    }
  }
  catch (...)
  {
    os << "<NO TASK>\n";
  }
}
void goltsov::mergeScheduleOtherContext(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& context_name)
{
  goltsov::Context context;
  goltsov::Schedule schedule;
  try
  {
    context = current_state.contexts_.at(context_name);
  }
  catch (...)
  {
    os << "<NO CONTEXT>\n";
    return;
  }
  try
  {
    schedule = context.schedules_.at(schedule_name);
  }
  catch (...)
  {
    os << "<NO SCHEDULE>\n";
    return;
  }
  std::pair< size_t, size_t > res = detail::mergeInterval(
    current_state, schedule.tasks_.begin(), schedule.tasks_.end());
  os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">\n";
}
void goltsov::addScheduleOtherContext(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& context_name)
{
  goltsov::Context context;
  goltsov::Schedule schedule;
  try
  {
    context = current_state.contexts_.at(context_name);
  }
  catch (...)
  {
    os << "<NO CONTEXT>\n";
    return;
  }
  try
  {
    schedule = context.schedules_.at(schedule_name);
  }
  catch (...)
  {
    os << "<NO SCHEDULE>\n";
    return;
  }
  try
  {
    current_state.current_context_->schedules_.at(schedule_name);
    os << "<SCHEDULE ALLREADY EXISTS>\n";
  }
  catch (...)
  {
    current_state.current_context_->schedules_.insert({schedule_name, schedule});
    os << "<ADD DONE. Added: " << schedule.tasks_.size() << ", Conflicts: "
      << schedule.unplanned_tasks_.size() << ">\n";
  }
}
void goltsov::addForceScheduleOtherContext(std::ostream& os, State& current_state,
  const std::string& schedule_name, const std::string& context_name)
{
  bool is_current_schedule = false;
  try
  {
    current_state.current_context_->schedules_.at(schedule_name);
    if (current_state.current_context_->name_context_ != context_name)
    {
      is_current_schedule = current_state.current_schedule_->name_schedule_ == schedule_name;
      current_state.current_context_->schedules_.erase(schedule_name);
    }
  }
  catch (...)
  {}
  addScheduleOtherContext(os, current_state, schedule_name, context_name);
  if (is_current_schedule)
  {
    current_state.current_schedule_ = &current_state.current_context_->schedules_.at(schedule_name);
  }
}
void goltsov::switchSchedule(std::ostream& os, State& current_state, const std::string& schedule_name)
{
  try
  {
    current_state.current_schedule_ = &current_state.current_context_->schedules_.at(schedule_name);
    os << "<SCHEDULE SWITCHED>\n";
  }
  catch (...)
  {
    os << "<NO SCHEDULE>\n";
  }
}
void goltsov::switchContext(std::ostream& os, State& current_state, const std::string& context_name)
{
  try
  {
    current_state.current_context_ = & current_state.contexts_.at(context_name);
    current_state.current_schedule_ = & current_state.current_context_->schedules_.begin()->second;
    os << "<CONTEXT SWITCHED>\n";
  }
  catch (...)
  {
    os << "<NO CONTEXT>\n";
  }
}
void goltsov::stats(std::ostream& os, State& current_state, const DateTime& start_time,
  const DateTime& end_time)
{
  Task a;
  a.start_time_ = start_time;
  goltsov::MapIterator< DateTime, Task > current =
    detail::rfindByPredicate(*current_state.current_schedule_, detail::FindTaskHasETLessLB{a});
  TimeInterval busy_time {0, 0, 0, 0, 0, 0};
  size_t count_tasks = 0;
  try
  {
    for (; current != current_state.current_schedule_->tasks_.end()
      && current->second.start_time_ < end_time; ++current)
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
void goltsov::newSchedule(std::ostream&, State& current_state, const std::string& name_schedule)
{
  current_state.current_context_->schedules_.insert({name_schedule, goltsov::Schedule {}});
  current_state.current_context_->schedules_.at(name_schedule).name_schedule_ = name_schedule;
}
void goltsov::newContext(std::ostream&, State& current_state, const std::string& name_context)
{
  current_state.contexts_.insert({name_context, goltsov::Context {}});
  current_state.contexts_.at(name_context).name_context_ = name_context;
  current_state.contexts_.at(name_context).schedules_.insert({"Base_schedule", goltsov::Schedule {}});
  current_state.contexts_.at(name_context).schedules_.at("Base_schedule").name_schedule_ = "Base_schedule";
}
void goltsov::loadSchedule(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& filename)
{
  try
  {
    current_state.current_context_->schedules_.at(schedule_name);
    os << "<NAME IS OCCUPIED>\n";
  }
  catch (...)
  {
    goltsov::Schedule schedule;
    schedule.name_schedule_ = schedule_name;
    std::fstream inp_f (filename);
    inp_f >> schedule;
    current_state.current_context_->schedules_.insert({schedule_name, schedule});
    current_state.current_schedule_ = &current_state.current_context_->schedules_.at(schedule_name);
    os << "<SCHEDULE LOADED>\n";
  }
}
void goltsov::saveSchedule(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& filename)
{
  try
  {
    goltsov::Schedule schedule = current_state.current_context_->schedules_.at(schedule_name);
    std::fstream out_f(filename, std::ios::out | std::ios::trunc);
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
void goltsov::loadContext(std::ostream& os, State& current_state, const std::string& context_name,
  const std::string& filename)
{
  try
  {
    goltsov::Context context = current_state.contexts_.at(context_name);
    os << "<NAME IS OCCUPIED>\n";
  }
  catch (...)
  {
    goltsov::Context context;
    context.name_context_ = context_name;
    std::fstream inp_f (filename);
    inp_f >> context;
    current_state.contexts_.insert({context_name, context});
    current_state.contexts_.at(context_name).name_context_ = context_name;
    current_state.current_context_ = &current_state.contexts_.at(context_name);
    current_state.current_schedule_ = &current_state.current_context_->schedules_.begin()->second;
    os << "<CONTEXT LOADED>\n";
  }
}
void goltsov::saveContext(std::ostream& os, State& current_state, const std::string& context_name,
  const std::string& filename)
{
  try
  {
    goltsov::Context context = current_state.contexts_.at(context_name);
    std::fstream out_f(filename, std::ios::out | std::ios::trunc);
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
void goltsov::findGap(std::ostream& os, State& current_state, const TimeInterval& interval)
{
  try
  {
    topit::Vector< std::string > a;
    a.pushBack(current_state.current_schedule_->name_schedule_);
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state,
      current_state.current_time, current_state.current_schedule_->tasks_.getLast()->second.end_time_ + interval,
      interval, a);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::findGapOnInterval(std::ostream& os, State& current_state, const DateTime& start_time,
  const DateTime& end_time, const TimeInterval& interval)
{
  try
  {
    topit::Vector< std::string > a;
    a.pushBack(current_state.current_schedule_->name_schedule_);
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state, start_time,
      end_time, interval, a);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::findCommonGap(std::ostream& os, State& current_state, const TimeInterval& interval,
  const size_t&, const topit::Vector< std::string >& names_schedules)
{
  try
  {
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state,
      current_state.current_time, current_state.current_schedule_->tasks_.getLast()->second.end_time_ + interval,
        interval, names_schedules);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::findCommonGapOnInterval(std::ostream& os, State& current_state, const DateTime& start_time,
  const DateTime& end_time, const TimeInterval& interval, const size_t&,
  const topit::Vector< std::string >& names_schedules)
{
  try
  {
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state, start_time,
      end_time, interval, names_schedules);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::listSchedules(std::ostream& os, State& current_state)
{
  for (MapIterator< std::string, goltsov::Schedule > it = current_state.current_context_->schedules_.begin();
    it != current_state.current_context_->schedules_.end(); ++it)
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
void goltsov::listContexts(std::ostream& os, State& current_state)
{
  for (MapIterator< std::string, goltsov::Context > it = current_state.contexts_.begin();
    it != current_state.contexts_.end(); ++it)
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
