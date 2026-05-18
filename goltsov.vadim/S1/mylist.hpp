#ifndef MYLIST_HPP
#define MYLIST_HPP
#include <stdexcept>
namespace detail
{
  template< class T >
  struct Node
  {
    T value;
    Node< T >* next;
  };
}

namespace goltsov
{
  template< class T >
  class List;

  template< class T >
  class LIter
  {
  public:
    LIter() noexcept;
    LIter(detail::Node< T >* p) noexcept;
    bool hasNext() const noexcept;
    LIter< T > next() const;
    LIter< T > operator++();
    T& operator*();
    T* operator->();
    bool operator==(const LIter< T >& other) const noexcept;
    bool operator!=(const LIter< T >& other) const noexcept;
  private:
    friend class List< T >;
    detail::Node< T >* ptr;
  };

  template< class T >
  class LCIter
  {
  public:
    LCIter() noexcept;
    LCIter(const detail::Node< T >* p) noexcept;
    bool hasNext() const noexcept;
    LCIter< T > next() const;
    LCIter< T > operator++();
    const T& operator*() const;
    const T* operator->() const;
    bool operator==(const LCIter< T >& other) const noexcept;
    bool operator!=(const LCIter< T >& other) const noexcept;
  private:
    friend class List< T >;
    const detail::Node< T >* ptr;
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
    void pop_start() noexcept;
    void pop_end() noexcept;
    LIter< T > insert(LIter< T > i, const T& a);
    LIter< T > insert(LIter< T > i, const T&& a);
    void clear() noexcept;
  private:
    detail::Node< T >* fake;
    detail::Node< T >* createFake();
    void rmFake() noexcept;
  };
}

namespace goltsov
{
  template< class T >
  LIter< T >::LIter() noexcept:
    ptr(nullptr)
  {}
  template< class T >
  LIter< T >::LIter(detail::Node< T >* p) noexcept:
    ptr(p)
  {}
  template< class T >
  bool LIter< T >::hasNext() const noexcept
  {
    return ptr != nullptr;
  }
  template< class T >
  LIter< T > LIter< T >::next() const
  {
    if (ptr)
    {
      return {ptr->next};
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  LIter< T > LIter< T >::operator++()
  {
    (* this) = next();
    return (* this);
  }
  template< class T >
  T& LIter< T >::operator*()
  {
    if (ptr)
    {
      return ptr->value;
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  T* LIter< T >::operator->()
  {
    if (ptr)
    {
      return ptr;
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  bool LIter< T >::operator==(const LIter< T >& other) const noexcept
  {
    return ptr == other.ptr;
  }
  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    ptr(nullptr)
  {}
  template< class T >
  LCIter< T >::LCIter(const detail::Node< T >* p) noexcept:
    ptr(p)
  {}
  template< class T >
  bool LCIter< T >::hasNext() const noexcept
  {
    return ptr != nullptr;
  }
  template< class T >
  LCIter< T > LCIter< T >::next() const
  {
    if (ptr)
    {
      return {ptr->next};
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  LCIter< T > LCIter< T >::operator++()
  {
    (* this) = next();
    return (* this);
  }
  template< class T >
  const T& LCIter< T >::operator*() const
  {
    if (ptr)
    {
      return ptr->value;
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  const T* LCIter< T >::operator->() const
  {
    if (ptr)
    {
      return ptr;
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& other) const noexcept
  {
    return ptr == other.ptr;
  }
  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class T >
  detail::Node< T >* List< T >::createFake()
  {
    detail::Node< T >* el = new detail::Node< T > {T(), nullptr};
    return el;
  }
  template< class T >
  void List< T >::rmFake() noexcept
  {
    delete fake;
  }
  template< class T >
  List< T >::List():
    fake(createFake())
  {
    fake->next = nullptr;
  }
  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    rmFake();
  }
  template< class T >
  List< T >::List(const List< T >& other):
    fake(createFake())
  {
    fake->next = nullptr;
    if (!other.fake->next)
    {
      return;
    }
    detail::Node< T >* now_old = other.fake->next;
    detail::Node< T >* now_new = new detail::Node< T >{now_old->value, nullptr};
    fake->next = now_new;
    now_old = now_old->next;
    while (now_old != nullptr)
    {
        now_new->next = new detail::Node< T >{now_old->value, nullptr};
        now_new = now_new->next;
        now_old = now_old->next;
    }
  }
  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    fake(other.fake)
  {
    other.fake = nullptr;
  }
  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != &other)
    {
      clear();
      fake->next = nullptr;
      if (other.fake->next)
      {
        detail::Node< T >* now_old = other.fake->next;
        detail::Node< T >* now_new = new detail::Node< T > {now_old->value, nullptr};
        fake->next = now_new;
        now_old = now_old->next;
        while (now_old != nullptr)
        {
          now_new->next = new detail::Node< T > {now_old->value, nullptr};
          now_new = now_new->next;
          now_old = now_old->next;
        }
      }
    }
    return *this;
  }
  template< class T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (this != & other)
    {
      detail::Node< T >* temp = fake;
      fake = other.fake;
      other.fake = temp;
    }
    return * this;
  }
  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(fake->next);
  }
  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(fake->next);
  }
  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T > (nullptr);
  }
  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T > (nullptr);
  }
  template< class T >
  LIter< T > List< T >::getLast() noexcept
  {
    LIter< T > now = begin();
    while(now.hasNext() && now.next() != nullptr)
    {
      now = now.next();
    }
    return now;
  }
  template< class T >
  LCIter< T > List< T >::getLast() const noexcept
  {
    LCIter< T > now = begin();
    while(now.hasNext() && now.next() != nullptr)
    {
      now = now.next();
    }
    return now;
  }
  template< class T >
  LIter< T > List< T >::push_start(const T& a)
  {
    detail::Node< T >* new_el = new detail::Node< T > {a, fake->next};
    fake->next = new_el;
    return LIter< T >(new_el);
  }
  template< class T >
  void List< T >::pop_start() noexcept
  {
    if (begin() != LIter< T > (nullptr))
    {
      detail::Node< T >* n = fake->next->next;
      delete fake->next;
      fake->next = n;
    }
    else
    {
      return;
    }
  }
  template< class T >
  void List< T >::pop_end() noexcept
  {
    detail::Node< T >* now = fake;
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
  LIter< T > List< T >::insert(LIter< T > i, const T& a)
  {
    if (i.ptr == nullptr)
    {
      i = push_start(a);
      return LIter< T >(i);
    }
    else
    {
      detail::Node< T >* new_el = new detail::Node< T > {a, i.next().ptr};
      i.ptr->next = new_el;
      i = i.ptr->next;
      return LIter< T >(i);
    }
  }
  template< class T >
  LIter< T > List< T >::insert(LIter< T > i, const T&& a)
  {
    if (i.ptr == nullptr)
    {
      i = push_start(a);
      return LIter< T >(i);
    }
    else
    {
      detail::Node< T >* new_el = new detail::Node< T > {a, i.next().ptr};
      i.ptr->next = new_el;
      i = i.ptr->next;
      return LIter< T >(i);
    }
  }
  template< class T >
  void List< T >::clear() noexcept
  {
    while (fake && fake->next != nullptr)
    {
      detail::Node< T >* temp = fake->next;
      fake->next = temp->next;
      delete temp;
    }
  }
}
#endif
