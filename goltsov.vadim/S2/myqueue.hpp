#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP
#include "../Common/mylist.hpp"

namespace goltsov
{
  template< typename T >
  class Queue
  {
    List< T > dates;
    LIter< T > end;
    size_t size;
  public:
    Queue();
    ~Queue() default;
    Queue(const Queue< T >& other);
    Queue(Queue< T >&& other);
    Queue< T >& operator=(const Queue< T >& other);
    Queue< T >& operator=(Queue< T >&& other);

    void push(T rhs);
    T drop();
    T front() const;
    T back() const;
    bool empty() const;
    bool size() const;
  };
}
#endif
