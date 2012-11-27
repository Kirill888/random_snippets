// g++-mp-4.6 -std=c++0x -o meta meta-select.cc
#include <iostream>

struct foo{
  template<int N>
  void operator()()
  {
    std::cout << "foo.void.void:" << N << "\n";
  }

  template<int N>
  int operator()(int x)
  {
    std::cout << "foo<int>("<<x
    <<"):" << N << "\n";
    return N+x;
  }

  template<int N>
  void operator()(const char* s)
  {
    std::cout << "s:" << s << "\n"
	      << N
	      << "\n";
  }
};

template<>
void foo::operator()<0>()
{
    std::cout << "foo: I am special\n";
}

class Bloop
{
public:
  template<int N>static
  int get(int * a){
    std::cout << "get:" << N << "\n";
    return a[N];
  }

  template<int N>static
  int* set(int * a, int v){
    std::cout << "set:" << N << "\n";
    a[N] = v;
    return a;
  }
};

struct bloop_wrap
{
  template<int N>int  operator()(int* a){return Bloop::get<N>(a);}
  template<int N>int* operator()(int* a, int v){return Bloop::set<N>(a,v);}
};


namespace meta
{
  class select
  {
  public:
    int i_;
    select(int i): i_(i){};

    template<int N, typename R>
    class impl
    {
    public:
      int i_;
      impl(int i): i_(i){}

      template<typename Action, typename... Args>
      R operator()(Action act, Args... args)
      {
	if (i_ == N) return act.template operator()<N>(args...);
	return impl<N-1,R>(i_).template operator()<Action,Args...>(act, args...);
      }
    };

    template<int N, typename T>
    impl<N,T> of(){return impl<N,T>(i_);}

    template<int N>
    impl<N,void> of(){return impl<N,void>(i_);}

  };

  template<typename R>
  class select::impl<0,R>
  {
  public:
    static const int N = 0;
    int i_;
    impl(int i): i_(i){}

    template<typename Action, typename... Args>
    R operator()(Action act, Args... args)
    {
      if (i_ == N) return act.template operator()<N>(args...);
      return R();
    }

  };
}

int main(int argc, char ** argv)
{
  int n = argc -1;

  {
    int x = meta::select(n).of<15, int>()( foo(), 1000 );
    std::cout << "got: " << x << "\n";
  }

  {
    meta::select(n&7).of<7>()( foo() );
  }

  {
    char const *t = "TEXT";
    meta::select(n).of<7>()( foo(), "TEXT");
  }

  {
    int data[15] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};

    int x    = meta::select(n).of<3,int >()( bloop_wrap(), data );
    int * xp = meta::select(n).of<3,int*>()( bloop_wrap(), data, 177);

  }
  return 0;
}
