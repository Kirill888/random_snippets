#include <cpuid.h>
#include <stdio.h>

inline 
void cpuid_query_topology(int levels[4])
{
  const int N = 4;
  int i;

  for(i = 0; i < N; ++i)
  {
    int a,b,c,d;
    __cpuid_count(0xb, i, a,b,c,d);
    if ( a == 0 && b == 0) break;

    levels[i] = b&0xFF;
  }
  while(i<N) levels[i++] = 0;
}


int main()
{
  int levels[4] = {1,1,1,1};
  cpuid_query_topology(levels);

  for(int i = 0; i < 4; ++i){
    printf("> %d  %d\n", i, levels[i]);
  }

  int nThreadsPerCpu = levels[0];
  int nLogicalCpus   = levels[1];

  printf("ThreadsPerCpu: %d, LogicalCpus: %d, PhysicalCpus: %d\n",
	 nThreadsPerCpu, nLogicalCpus, nLogicalCpus/nThreadsPerCpu);



  return 0;
}
