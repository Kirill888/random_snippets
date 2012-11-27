#include <tuple>
#include <stdio.h>


template<typename Func, typename ...Args>
void apply_func(Func f, Args... args)
{
  f( args... );
}

namespace impl1
{
  template<int N>struct tuple_apply;

  template<>struct tuple_apply<0>
  {
    template<typename Func, typename Tuple>static void apply(Func f, Tuple t) {f();}
  };

  template<>struct tuple_apply<1>
  {
    template<typename Func, typename Tuple>static void apply(Func f, Tuple t)
    { f(std::get<0>(t));}
  };

  template<>struct tuple_apply<2>
  {
    template<typename Func, typename Tuple>static void apply(Func f, Tuple t)
    { f(std::get<0>(t), std::get<1>(t));}
  };

  template<>struct tuple_apply<3>
  {
    template<typename Func, typename Tuple>static void apply(Func f, Tuple t)
    { f(std::get<0>(t), std::get<1>(t), std::get<2>(t));}
  };
}//impl1::


namespace impl
{
  template<int N> struct tuple_apply;

  template<> struct tuple_apply<0>
  {
    template<typename Func, typename Tuple, typename ... Args>
    static void apply(Func f, Tuple t, Args ...args)
    {
      f(args...);
    }
  };

  template<int N>struct tuple_apply
  {
    template<typename Func, typename Tuple, typename ... Args>
    static void apply(Func f, Tuple t, Args ...args)
    {
      tuple_apply<N-1>::apply(f, t, std::get<N-1>(t), args... );
    }
  };
}

template<typename Func, typename Tuple>
void tuple_apply(Func f, Tuple args)
{
  const int N = std::tuple_size<Tuple>::value;

  impl::tuple_apply<N>::apply(f,args);
}


int main()
{
  auto f1 = [](int v, double a){ printf("f1: %d %g\n", v, a);};

  auto args = std::make_tuple(10, 3.4);

  tuple_apply(f1,args);

  return 0;
}

