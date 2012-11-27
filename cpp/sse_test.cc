#include <stdio.h>

#include <emmintrin.h>
#include <tmmintrin.h>
#include <iostream>
#include <iomanip>


namespace detail
{
  template<typename T>inline T print_cast(T v){return v;}

  inline unsigned int print_cast(unsigned char v){return (unsigned int)v;}
  inline int print_cast(char v){return (int)v;}

  template<int N, typename T>
  std::ostream & print_impl(std::ostream & out, __m128i a, int w)
  {
    union{
      __m128i p;
      T x[N];
    } u;

    u.p = a;

    for(int i = 0; i < N; ++i)
    {
      out << " " << std::setw(w) << print_cast(u.x[i]);
    }

    return out;
  }

  template<int n, typename T>
  class print_base{
  public:
    static const int N = n;
    typedef T value_type;

    __m128i const & value;
    int width;

    print_base(__m128i const & v): value(v),width(0){}
    print_base(__m128i const & v, int w): value(v),width(w){}
  };
}

#define DEFINE_AS_FUNC(name, type, N)					\
detail::print_base<N, type> as_##name(__m128i const & v, int w){return detail::print_base<N,type>(v,w);}


DEFINE_AS_FUNC(u8,unsigned char, 16)
DEFINE_AS_FUNC(i8,char         , 16)

DEFINE_AS_FUNC(u16,unsigned short, 8)
DEFINE_AS_FUNC(i16,short         , 8)

DEFINE_AS_FUNC(u32,unsigned int, 4)
DEFINE_AS_FUNC(i32,int         , 4)


template<int n, typename T>
std::ostream & operator<<(std::ostream & out, detail::print_base<n,T> v)
{
  return detail::print_impl<n, T>(out, v.value, v.width );
}

template<int N, typename T>
void print_impl(const char* fmt, __m128i a)
{
  union{
    __m128i p;
    T x[N];
  } u;

  u.p = a;

  for(int i = 0; i < N; ++i) printf(fmt, u.x[i] );
}
template<typename T> void print(__m128i a);

template<> void print<unsigned char>(__m128i a){print_impl<16, unsigned char>(" %u", a);}
template<> void print<char         >(__m128i a){print_impl<16,          char>(" %d", a);}

template<> void print<short         >(__m128i a){print_impl<8,          short>(" %d", a);}
template<> void print<unsigned short>(__m128i a){print_impl<8, unsigned short>(" %u", a);}

template<> void print<int         >(__m128i a){print_impl<4, int>(" %d", a);}
template<> void print<unsigned int>(__m128i a){print_impl<4, unsigned int>(" %u", a);}


template<typename T>
void print(char const* prefix, __m128i a, char const * postfix="\n")
{
  printf("%s", prefix); print<T>(a); printf("%s", postfix);
}

inline __m128i fromArray(unsigned char d[16]){ return _mm_loadu_si128((__m128i*)d);}
inline __m128i fromArray(         char d[16]){ return _mm_loadu_si128((__m128i*)d);}

inline __m128i fromArray(unsigned short d[8]){ return _mm_loadu_si128((__m128i*)d);}
inline __m128i fromArray(	  short d[8]){ return _mm_loadu_si128((__m128i*)d);}

inline __m128i fromArray(unsigned int d[4]){ return _mm_loadu_si128((__m128i*)d);}
inline __m128i fromArray(	  int d[4]){ return _mm_loadu_si128((__m128i*)d);}

template<int v0, int v1, int v2, int v3>
inline __m128i make(){ return _mm_set_epi32(v3,v2,v1,v0); }

template<int v0, int v1, int v2, int v3
        ,int v4, int v5, int v6, int v7 >
inline __m128i make(){ return _mm_set_epi16(v7,v6,v5,v4,v3,v2,v1,v0); }

template
  < int v0, int v1, int v2, int v3
  , int v4 ,int v5, int v6, int v7
  , int v8 ,int v9, int vA, int vB
  , int vC ,int vD, int vE, int vF
  >
inline __m128i make(){
  return _mm_set_epi8
    ( vF,vE,vD,vC
     ,vB,vA,v9,v8
     ,v7,v6,v5,v4
     ,v3,v2,v1,v0 ); }

template
  < int v0, int v1, int v2, int v3
  , int v4 ,int v5, int v6, int v7
  , int v8 ,int v9, int vA, int vB
  , int vC ,int vD, int vE, int vF
  >
inline __m128i shuffle(__m128i a) //< SSSE3
{
  return _mm_shuffle_epi8
    (a, make<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,vA,vB,vC,vD,vE,vF>() );
}

template<typename T> void upscale(__m128i src, __m128i &lo, __m128i &hi);

template<>inline
void upscale<unsigned char>(__m128i src, __m128i &lo, __m128i &hi)
{
  lo = _mm_unpacklo_epi8(src, _mm_setzero_si128() );
  hi = _mm_unpackhi_epi8(src, _mm_setzero_si128() );
}

template<>inline
void upscale<char>(__m128i src, __m128i &lo, __m128i &hi)
{
  lo = _mm_unpacklo_epi8(src, _mm_setzero_si128() );
  hi = _mm_unpackhi_epi8(src, _mm_setzero_si128() );
}

template<>inline
void upscale<short>(__m128i src, __m128i &lo, __m128i &hi)
{
  lo = _mm_unpacklo_epi16(src, _mm_setzero_si128() );
  hi = _mm_unpackhi_epi16(src, _mm_setzero_si128() );
}

template<>inline
void upscale<unsigned short>(__m128i src, __m128i &lo, __m128i &hi)
{
  lo = _mm_unpacklo_epi16(src, _mm_setzero_si128() );
  hi = _mm_unpackhi_epi16(src, _mm_setzero_si128() );
}


int main()
{
#define off if(0)

  typedef unsigned char u8;
  typedef unsigned short u16;

  const __m128i zero = _mm_setzero_si128();

  off{//Upscale to 16bit
    __m128i a = make<0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15>();
    print<u8>("a:", a);

    __m128i pl = _mm_unpacklo_epi8(a, zero );
    __m128i ph = _mm_unpackhi_epi8(a, zero );

    print<short>("pl:", pl);
    print<short>("ph:", ph);

    // __m128i y = _mm_madd_epi16(pl,ph);
    //print<int>("madd:", y);
  }


  off{//Shuffle test (sse3): split into: <odd> | <even>
    __m128i a = make<0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15>();
    print<u8>("a:", a);
    a = shuffle<0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15>(a);
    print<u8>("a':", a);
  }

  {
     __m128i data = make<0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15>();
     __m128i a,b;

     std::cout << "a.stream:" << std::setbase(16) << as_u8(data,2) << "\n";

     upscale<u8>(data, a, b); //u8 -> u16,u16

     __m128i x = _mm_hadd_epi16(a,b);

     std::cout << std::setbase(10)
	       << "ab:" <<as_i16(a,3) << "|"
               <<as_i16(b,3) << "\n"
	       << "r :" <<as_i16(x,3) << "\n-\n";

     upscale<u16>(x, a,b);
     x = _mm_hadd_epi32(a,b);
     std::cout << std::setbase(10)
	       << "a:" <<as_i32(a,3) << "|"
	       << as_i32(b,3) << "\n"
	       << "r:" <<as_i32(x,3) << "\n";
       ;

  }


  return 0;
}
