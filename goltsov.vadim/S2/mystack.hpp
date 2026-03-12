#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "../Common/mylist.hpp"

namespace goltsov
{
  template< typename T >
  class Stack
  {
    List< T > dates;
    size_t size;
  public:
    Stack();
    ~Stack();
    Stack< T >& Stack(const Stack< T >& other);
    Stack< T >& Stack(Stack< T >&& other);
    Stack< T >& operator=(const Stack< T >& other);
    Stack< T >& operator=(Stack< T >&& other);

    void push(T rhs);
    T drop();
    T front() const;
    bool empty() const;
    bool size() const;
  };
}
#endif
