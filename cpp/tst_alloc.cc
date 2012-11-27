#include <limits>
#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>

class MemChunk{
public:
  char * p_next;
  size_t sz_left;

  MemChunk():p_next(0),sz_left(0){}
  MemChunk(char * p, size_t sz): p_next(p), sz_left(sz){}
};

template<typename T>
class MyAlloc {
public : 
  //    typedefs
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template<typename U> struct rebind {typedef MyAlloc<U> other;};

  MemChunk & mem_;

public : 
  MyAlloc(MemChunk & m):mem_(m) {}

  ~MyAlloc() {}
  MyAlloc(MyAlloc const& other):mem_(other.mem_) {}
  template<typename U> MyAlloc(MyAlloc<U> const& other): mem_(other.mem_) {}

  //    address
  pointer address(reference r) { return &r; }
  const_pointer address(const_reference r) { return &r; }

  //    memory allocation
  pointer allocate(size_type cnt, 
		   typename std::allocator<void>::const_pointer = 0) { 
    size_t sz = cnt*sizeof(T);

    std::cerr << typeid(T).name() <<"\n";

    std::cerr << " alloc:" << cnt << " of " << sizeof(T) <<"\n";
    std::cerr << " sz:" << sz << " have:"<< mem_.sz_left <<"\n";

    if ( sz > mem_.sz_left) throw std::bad_alloc();

    pointer p = reinterpret_cast<pointer>(mem_.p_next);

    mem_.p_next  += sz;
    mem_.sz_left -= sz;

    return p; 
  }

  void deallocate(pointer p, size_type sz) { 
    std::cerr << "de-alloc: " << (void*)(p) << " sz:" << sz <<"\n";
  }

  size_type max_size() const { return mem_.sz_left / sizeof(T);}

  //    construction/destruction
  void construct(pointer p, const T& t) { new(p) T(t); }
  void destroy(pointer p) { p->~T(); }

  bool operator==(MyAlloc const&) { return true; }
  bool operator!=(MyAlloc const& a) { return !operator==(a); }
};    //    end of class MyAlloc

class NoopDeleter{
public:
  template<typename T> void operator()(T *){}
};

class Foo{};

int main()
{
  typedef boost::shared_ptr<Foo> Ptr;

  size_t const buf_sz = 1024;
  char buf[buf_sz];
  MemChunk mem(buf,buf_sz);
  MyAlloc<int> alloc(mem);

  std::cout << "sizeof(Alloc):"       << sizeof(alloc) <<"\n";
  std::cout << "sizeof(Foo):"         << sizeof(Foo)   <<"\n";
  std::cout << "sizeof(NoopDeleter):" << sizeof(NoopDeleter)   <<"\n";
  std::cout << "sizeof(Ptr):"         << sizeof(Ptr)   <<"\n";

  Foo v;
  Ptr ptr(&v, NoopDeleter(), alloc );

  Ptr ptr_other(&v, NoopDeleter(), alloc );


  Ptr p2 = ptr;
  p2.reset();
  {
    std::vector< Ptr , MyAlloc<int> > pool(alloc);
    pool.reserve(10);

    pool.push_back(ptr);
    pool.push_back(ptr);
    pool.push_back(ptr);
    pool.push_back(ptr);
    pool.push_back(ptr);
    pool.push_back(ptr);

    std::cerr << " " << pool.size() <<"\n";
    std::cerr << " usec:" << ptr.use_count() << "\n";
  }

  std::cerr << " usec:" << ptr.use_count() << "\n";
}
