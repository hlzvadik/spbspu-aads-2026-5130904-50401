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
    size_t size_;
  };
}

namespace goltsov
{
  template< class T >
  Queue< T >::Queue():
    dates_(List< T > {}),
    tail_(LIter< T > {}),
    size_(0)
  {}
  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    tail_ = dates_.insert(tail_, rhs);
    size_ += 1;
  }
  template< class T >
  void Queue< T >::push(T&& rhs)
  {
    tail_ = dates_.insert(tail_, std::move(rhs));
    size_ += 1;
  }
  template< class T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    dates_.pop_start();
    size_--;
    if (empty())
    {
      tail_ = LIter< T >();
    }
  }
  template< class T >
  T& Queue< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (*dates_.begin());
  }
  template< class T >
  const T& Queue< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (*dates_.begin());
  }
  template< class T >
  T& Queue< T >::back()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (*tail_);
  }
  template< class T >
  const T& Queue< T >::back() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (*tail_);
  }
  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }
  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }
  template< class T >
  void Queue< T >::clear()
  {
    dates_.clear();
    size_ = 0;
    tail_ = LIter< T >();
  }
}
#endif
