#ifndef MYLIST_HPP
#define MYLIST_HPP
#include <stdexcept>

namespace goltsov
{
  template< class T >
  class List;
  template< class T >
  class LIter;
  template< class T >
  class LCIter;
}

namespace goltsov
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T value;
      Node< T >* next;
    };
    template< class T >
    LIter< T > makeLIterByPtr(Node< T >* p);
    template< class T >
    LCIter< T > makeLCIterByPtr(const Node< T >* p);
  }
}

namespace goltsov
{
  template< class T >
  class LIter
  {
  public:
    LIter() noexcept;
    LIter< T >& operator++();
    LIter< T > operator++(int);
    T& operator*();
    T* operator->();
    bool operator==(const LIter< T >& other) const noexcept;
    bool operator!=(const LIter< T >& other) const noexcept;
  private:
    friend class List< T >;
    friend LIter< T > detail::makeLIterByPtr< T >(Node< T >* p);
    detail::Node< T >* ptr_;
    LIter(detail::Node< T >* p) noexcept;
    LIter< T > next() const;
  };

  template< class T >
  class LCIter
  {
  public:
    LCIter() noexcept;
    LCIter< T >& operator++();
    LCIter< T > operator++(int);
    const T& operator*() const;
    const T* operator->() const;
    bool operator==(const LCIter< T >& other) const noexcept;
    bool operator!=(const LCIter< T >& other) const noexcept;
  private:
    friend class List< T >;
    friend LCIter< T > detail::makeLCIterByPtr< T >(const Node< T >* p);
    const detail::Node< T >* ptr_;
    LCIter(const detail::Node< T >* p) noexcept;
    LCIter< T > next() const;
  };

  template< class T >
  class List
  {
  public:
    List();
    ~List() noexcept;
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LIter< T > getLast() noexcept;
    LCIter< T > getLast() const noexcept;
    LIter< T > push_start(const T& a);
    LIter< T > push_start(T&& a);
    void pop_start() noexcept;
    void pop_end() noexcept;
    LIter< T > insert(LIter< T > i, const T& a);
    LIter< T > insert(LIter< T > i, T&& a);
    void clear() noexcept;
  private:
    detail::Node< T >* fake_;
    detail::Node< T >* createFake();
    void rmFake() noexcept;
  };
}


template< class T >
goltsov::LIter< T > goltsov::detail::makeLIterByPtr(Node< T >* p)
{
  return LIter< T > (p);
}
template< class T >
goltsov::LCIter< T > goltsov::detail::makeLCIterByPtr(const Node< T >* p)
{
  return LCIter< T > (p);
}

template< class T >
goltsov::LIter< T >::LIter() noexcept:
  ptr_(nullptr)
{}
template< class T >
goltsov::LIter< T >::LIter(detail::Node< T >* p) noexcept:
  ptr_(p)
{}
template< class T >
goltsov::LIter< T > goltsov::LIter< T >::next() const
{
  if (ptr_)
  {
    return {ptr_->next};
  }
  else
  {
    throw std::runtime_error("Null pointer dereference");
  }
}
template< class T >
goltsov::LIter< T >& goltsov::LIter< T >::operator++()
{
  (*this) = next();
  return (*this);
}
template< class T >
goltsov::LIter< T > goltsov::LIter< T >::operator++(int)
{
  LIter< T > temp = (*this);
  (*this) = next();
  return temp;
}
template< class T >
T& goltsov::LIter< T >::operator*()
{
  if (ptr_)
  {
    return ptr_->value;
  }
  else
  {
    throw std::runtime_error("Null pointer dereference");
  }
}
template< class T >
T* goltsov::LIter< T >::operator->()
{
  if (ptr_)
  {
    return ptr_;
  }
  else
  {
    throw std::runtime_error("Null pointer dereference");
  }
}
template< class T >
bool goltsov::LIter< T >::operator==(const LIter< T >& other) const noexcept
{
  return ptr_ == other.ptr_;
}
template< class T >
bool goltsov::LIter< T >::operator!=(const LIter< T >& other) const noexcept
{
  return !((*this) == other);
}
template< class T >
goltsov::LCIter< T >::LCIter() noexcept:
  ptr_(nullptr)
{}
template< class T >
goltsov::LCIter< T >::LCIter(const detail::Node< T >* p) noexcept:
  ptr_(p)
{}
template< class T >
goltsov::LCIter< T > goltsov::LCIter< T >::next() const
{
  if (ptr_)
  {
    return {ptr_->next};
  }
  else
  {
    throw std::runtime_error("Null pointer dereference");
  }
}
template< class T >
goltsov::LCIter< T >& goltsov::LCIter< T >::operator++()
{
  (*this) = next();
  return (*this);
}
template< class T >
goltsov::LCIter< T > goltsov::LCIter< T >::operator++(int)
{
  LCIter< T > temp = (*this);
  (*this) = next();
  return temp;
}
template< class T >
const T& goltsov::LCIter< T >::operator*() const
{
  if (ptr_)
  {
    return ptr_->value;
  }
  else
  {
    throw std::runtime_error("Null pointer dereference");
  }
}
template< class T >
const T* goltsov::LCIter< T >::operator->() const
{
  if (ptr_)
  {
    return ptr_;
  }
  else
  {
    throw std::runtime_error("Null pointer dereference");
  }
}
template< class T >
bool goltsov::LCIter< T >::operator==(const LCIter< T >& other) const noexcept
{
  return ptr_ == other.ptr_;
}
template< class T >
bool goltsov::LCIter< T >::operator!=(const LCIter< T >& other) const noexcept
{
  return !((*this) == other);
}
template< class T >
goltsov::detail::Node< T >* goltsov::List< T >::createFake()
{
  detail::Node< T >* el = new detail::Node< T > {T(), nullptr};
  return el;
}
template< class T >
void goltsov::List< T >::rmFake() noexcept
{
  delete fake_;
}
template< class T >
goltsov::List< T >::List():
  fake_(createFake())
{
  fake_->next = nullptr;
}
template< class T >
goltsov::List< T >::~List() noexcept
{
  clear();
  rmFake();
}
template< class T >
goltsov::List< T >::List(const List< T >& other):
  fake_(createFake())
{
  fake_->next = nullptr;
  if (!other.fake_->next)
  {
    return;
  }
  LIter< T > i = begin();
  for (LCIter< T > it = other.begin(); it != other.end(); ++it)
  {
    i = insert(i, (*it));
  }
}
template< class T >
goltsov::List< T >::List(List< T >&& other) noexcept:
  fake_(other.fake_)
{
  other.fake_ = nullptr;
}
template< class T >
goltsov::List< T >& goltsov::List< T >::operator=(const List< T >& other)
{
  if (this != &other)
  {
    clear();
    fake_->next = nullptr;
    if (other.fake_->next)
    {
      detail::Node< T >* now_old = other.fake_->next;
      detail::Node< T >* now_new = new detail::Node< T > {now_old->value, nullptr};
      fake_->next = now_new;
      now_old = now_old->next;
      while (now_old != nullptr)
      {
        now_new->next = new detail::Node< T > {now_old->value, nullptr};
        now_new = now_new->next;
        now_old = now_old->next;
      }
    }
  }
  return (*this);
}
template< class T >
goltsov::List< T >& goltsov::List< T >::operator=(List< T >&& other) noexcept
{
  if (this != & other)
  {
    detail::Node< T >* temp = fake_;
    fake_ = other.fake_;
    other.fake_ = temp;
  }
  return (*this);
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::begin() noexcept
{
  return LIter< T >(fake_->next);
}
template< class T >
goltsov::LCIter< T > goltsov::List< T >::begin() const noexcept
{
  return LCIter< T >(fake_->next);
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::end() noexcept
{
  return LIter< T > (nullptr);
}
template< class T >
goltsov::LCIter< T > goltsov::List< T >::end() const noexcept
{
  return LCIter< T > (nullptr);
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::getLast() noexcept
{
  LIter< T > now = begin();
  while (now.ptr_ != nullptr && now.next() != nullptr)
  {
    now = now.next();
  }
  return now;
}
template< class T >
goltsov::LCIter< T > goltsov::List< T >::getLast() const noexcept
{
  LCIter< T > now = begin();
  while (now.ptr_ != nullptr && now.next() != nullptr)
  {
    now = now.next();
  }
  return now;
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::push_start(const T& a)
{
  detail::Node< T >* new_el = new detail::Node< T > {a, fake_->next};
  fake_->next = new_el;
  return LIter< T >(new_el);
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::push_start(T&& a)
{
  detail::Node< T >* new_el = new detail::Node< T > {std::move(a), fake_->next};
  fake_->next = new_el;
  return LIter< T >(new_el);
}
template< class T >
void goltsov::List< T >::pop_start() noexcept
{
  if (begin() != end())
  {
    detail::Node< T >* n = fake_->next->next;
    delete fake_->next;
    fake_->next = n;
  }
  else
  {
    return;
  }
}
template< class T >
void goltsov::List< T >::pop_end() noexcept
{
  detail::Node< T >* now = fake_;
  if (now->next == nullptr)
  {
    return;
  }
  while (now->next->next != nullptr)
  {
    now = now->next;
  }
  delete now->next;
  now->next = nullptr;
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::insert(LIter< T > i, const T& a)
{
  if (i.ptr_ == nullptr)
  {
    i = push_start(a);
    return LIter< T >(i);
  }
  else
  {
    detail::Node< T >* new_el = new detail::Node< T > {a, i.next().ptr_};
    i.ptr_->next = new_el;
    i = i.ptr_->next;
    return LIter< T >(i);
  }
}
template< class T >
goltsov::LIter< T > goltsov::List< T >::insert(LIter< T > i, T&& a)
{
  if (i.ptr_ == nullptr)
  {
    i = push_start(std::move(a));
    return LIter< T >(i);
  }
  else
  {
    detail::Node< T >* new_el = new detail::Node< T > {std::move(a), i.next().ptr_};
    i.ptr_->next = new_el;
    i = i.ptr_->next;
    return LIter< T >(i);
  }
}
template< class T >
void goltsov::List< T >::clear() noexcept
{
  while (fake_ && fake_->next != nullptr)
  {
    detail::Node< T >* temp = fake_->next;
    fake_->next = temp->next;
    delete temp;
  }
}

#endif
