#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP
<<<<<<< HEAD
=======
#include <utility>
>>>>>>> goltsov.vadim/S4
#include <mylist.hpp>

namespace goltsov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >& other) = default;
    Queue(Queue< T >&& other) = default;
    ~Queue() = default;
    Queue< T >& operator=(const Queue< T >& other) = default;
    Queue< T >& operator=(Queue< T >&& other) = default;

    template< class ValRef >
    void push(ValRef&&);

    void pop();
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
  private:
    List< T > dates_;
    LIter< T > tail_;
  };
}

template< class T >
template< class ValRef >
void goltsov::Queue< T >::push(ValRef&& rhs)
{
  tail_ = dates_.insert(tail_, std::forward< ValRef >(rhs));
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
void goltsov::Queue< T >::clear() noexcept
{
  dates_.clear();
  tail_ = LIter< T >();
}

#endif
