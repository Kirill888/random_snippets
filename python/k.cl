__kernel void box13(__global const int *a,
		    __global int *c)
{
  size_t gid = get_global_id(0);

  int s = 0;

  for(size_t i = gid; i < (gid+13); ++i)
  {
    s += a[i];
  }

  c[gid] = s;
}

__kernel void box13x13(__global const uchar *a,
		       int a_step,
		       __global int * out,
		       int o_step)
{
  size_t c = get_global_id(0);
  size_t r = get_global_id(1);

  int s = 0;

  a += r*a_step + c;
  o += r*o_step + c;

  for(size_t i = 0; i < 13; ++i)
  {
    uchar const * a_end = a + 13;
    for( ;a != a_end; ++a)
    {
      s += *a;
    }

    a += a_step - 13;
  }

  *o = s;
}


