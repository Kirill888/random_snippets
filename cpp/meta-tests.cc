#include <stdio.h>

typedef unsigned char uint8 ;
typedef unsigned  int uint32;

namespace bits
{
  class unpack
  {
  public:

    template<int B0>
    class impl
    {
      uint8 const * const d_;
    public:
      template<int NB>
      class trait
      {
      public:
	typedef impl<(B0 + NB)&7> next_type;
	static const int d_offset = (B0+NB)/8;
      };

      impl(uint8 const *d):d_(d){}

      template<int n1>
      typename trait<n1>::next_type bits()const
      {
	return typename trait<n1>::next_type(d_ + trait<n1>::d_offset);
      }

      template<int n1>
      typename trait<n1>::next_type bits(uint32 & v1)const
      {
	/// output for testing: FirstByteOfData|B0|00|Nbits
	v1 = (uint32(d_[0])<<24)|(B0<<16)|n1;
	return bits<n1>();
      }
    };

    ///run-time unpacking
    class impl_r
    {
      uint8 const * const d_;
      int const    b0_;
    public:
      impl_r(uint8 const * d, int b0): d_(d),b0_(b0){}

      impl_r operator()(int const nb)const
      {
	int d_off = (b0_ + nb)>>3;
        int b0    = (b0_ + nb)&7;
	return impl_r(d_+d_off,b0);
      }

      impl_r operator()(int const nb, uint32 & v)const
      {
	v = (uint32(d_[0])<<24)|(b0_<<16)|nb;
	return (*this)(nb);
      }

      impl_r const & get_pos(uint8 const * & d, int & b0)const
      {
	d = d_; b0 = b0_;
	return *this;
      }
    };

    unpack(uint8 const *d):d_(d){}

    /// skip function
    template<int n>
    typename impl<0>::trait<n>::next_type bits()const{return impl<0>(d_).bits<n>( );}

    template<int n>
    typename impl<0>::trait<n>::next_type bits(uint32 & v)const{return impl<0>(d_).bits<n>(v);}

    /// run-time version access
    impl_r operator()(int const nb            )const{return impl_r(d_,0)(nb  );}
    impl_r operator()(int const nb, uint32 & v)const{return impl_r(d_,0)(nb,v);}
    impl_r bits      (int const nb            )const{return impl_r(d_,0)(nb  );}
    impl_r bits      (int const nb, uint32 & v)const{return impl_r(d_,0)(nb,v);}
     
  private:
    uint8 const * const d_;
  };
}

uint32 test_meta(uint8 const * d)
{
  uint32 v1,v2,v3,v4;

  bits::unpack(d).bits<5>(v1)
    .             bits<7>(v2)
    .             bits<9>(v3)
    .             bits<11>(v4);

  return v1+v2+v3+v4;
}

uint32 test_const_prop(uint8 const * d)
{
  uint32 v1,v2,v3,v4,v5;

  int b0;

  bits::unpack(d).bits( 5, v1)
    ( 7, v2)
    ( 9, v3)
    (11, v4).get_pos(d,b0);

  bits::unpack(d).bits(b0)
    (4,v5);

  return v1+v2+v3+v4+v5;
}



#if 1

int main(int argc, char ** argv)
{
  unsigned char const d0[] = {0x00,0x11,0x22,0x33,0x44,0x55};
  unsigned int v1,v2,v3,v4;

  unsigned char const * d = d0;
  if( argc > 1) // stop const propagation for content of d
  {
    d = reinterpret_cast<unsigned char const*>(argv[0]);
  }

  bits::unpack(d).bits< 4>(v1)
    .             bits< 5>(v2)
    .             bits< 8>(v3)
    .             bits<10>(  )
    .             bits< 7>(v4);

  printf("%08X %08X %08X %08X\n",v1,v2,v3,v4);
  return 0;
}
#endif
