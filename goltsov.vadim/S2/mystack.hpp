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
    size_t size_;
  };
}

namespace goltsov
{
  template< class T >
  Stack< T >::Stack():
    dates_(goltsov::List< T > {}),
    size_(0)
  {}
  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    dates_.push_start(rhs);
    size_ += 1;
  }
  template< class T >
  void Stack< T >::push(T&& rhs)
  {
    dates_.push_start(std::move(rhs));
    size_ += 1;
  }
  template< class T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    dates_.pop_start();
    size_--;
  }
  template< class T >
  T& Stack< T >::top()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return (*dates_.begin());
  }
  template< class T >
  const T& Stack< T >::top() const
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return (*dates_.begin());
  }
  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return size_ == 0;
  }
  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }
  template< class T >
  void Stack< T >::clear()
  {
    dates_.clear();
    size_ = 0;
  }
}
#endif
