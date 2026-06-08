#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include <mylist.hpp>

namespace goltsov
{
  template< class T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack< T >& other) = default;
    Stack(Stack< T >&& other) = default;
    ~Stack() = default;
    Stack< T >& operator=(const Stack< T >& other) = default;
    Stack< T >& operator=(Stack< T >&& other) = default;

    void push(const T&);
    void push(T&&);
    void pop();
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  private:
    List< T > dates_;
  };
}

template< class T >
goltsov::Stack< T >::Stack():
  dates_(goltsov::List< T > {})
{}
template< class T >
void goltsov::Stack< T >::push(const T& rhs)
{
  dates_.push_start(rhs);
}
template< class T >
void goltsov::Stack< T >::push(T&& rhs)
{
  dates_.push_start(std::move(rhs));
}
template< class T >
void goltsov::Stack< T >::pop()
{
  if (empty())
  {
    throw std::runtime_error("Stack is empty");
  }
  dates_.pop_start();
}
template< class T >
T& goltsov::Stack< T >::top()
{
  if (empty())
  {
    throw std::runtime_error("Stack is empty");
  }
  return (*dates_.begin());
}
template< class T >
const T& goltsov::Stack< T >::top() const
{
  if (empty())
  {
    throw std::runtime_error("Stack is empty");
  }
  return (*dates_.begin());
}
template< class T >
bool goltsov::Stack< T >::empty() const noexcept
{
  return dates_.empty();
}
template< class T >
size_t goltsov::Stack< T >::size() const noexcept
{
  return dates_.size();
}
template< class T >
void goltsov::Stack< T >::clear()
{
  dates_.clear();
}

#endif
