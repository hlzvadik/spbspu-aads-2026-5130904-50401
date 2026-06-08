#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP
#include <mylist.hpp>

namespace goltsov
{
  template< class T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue< T >& other) = default;
    Queue(Queue< T >&& other) = default;
    ~Queue() = default;
    Queue< T >& operator=(const Queue< T >& other) = default;
    Queue< T >& operator=(Queue< T >&& other) = default;

    void push(const T& rhs);
    void push(T&& rhs);
    void pop();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  private:
    List< T > dates_;
    LIter< T > tail_;
  };
}

template< class T >
goltsov::Queue< T >::Queue():
  dates_(List< T > {}),
  tail_(LIter< T > {})
{}
template< class T >
void goltsov::Queue< T >::push(const T& rhs)
{
  tail_ = dates_.insert(tail_, rhs);
}
template< class T >
void goltsov::Queue< T >::push(T&& rhs)
{
  tail_ = dates_.insert(tail_, std::move(rhs));
}
template< class T >
void goltsov::Queue< T >::pop()
{
  if (empty())
  {
    throw std::runtime_error("Queue is empty");
  }
  dates_.pop_start();
  if (empty())
  {
    tail_ = LIter< T >();
  }
}
template< class T >
T& goltsov::Queue< T >::front()
{
  if (empty())
  {
    throw std::runtime_error("Queue is empty");
  }
  return (*dates_.begin());
}
template< class T >
const T& goltsov::Queue< T >::front() const
{
  if (empty())
  {
    throw std::runtime_error("Queue is empty");
  }
  return (*dates_.begin());
}
template< class T >
T& goltsov::Queue< T >::back()
{
  if (empty())
  {
    throw std::runtime_error("Queue is empty");
  }
  return (*tail_);
}
template< class T >
const T& goltsov::Queue< T >::back() const
{
  if (empty())
  {
    throw std::runtime_error("Queue is empty");
  }
  return (*tail_);
}
template< class T >
bool goltsov::Queue< T >::empty() const noexcept
{
  return dates_.empty();
}
template< class T >
size_t goltsov::Queue< T >::size() const noexcept
{
  return dates_.size();
}
template< class T >
void goltsov::Queue< T >::clear()
{
  dates_.clear();
  tail_ = LIter< T >();
}

#endif
