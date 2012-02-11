#!/usr/bin/env python

'''
  Solution to a puzzle I got from Derek (aparently from google interview) 
  I no longer remember exactly the problem definition
  ...should have written it down...
  
  It is something along these lines:
   Given a positive integer X find all non-increasing sequences of
   positive integers that would add up to X.
   And you have to print them out in a particular order also
   (shortest to longest, but also {4 1} before {3 2}, not sure
    how exactly this was worded in the original)
  Example: 3 -> {3} {2 1} {1 1 1}
'''

def solve_lvl(N,L, cap):
  if L < 1 or N < L or L*cap < N:
    return []
  elif L == 1:
    return [[N]]
  
  out = []
  start = min(cap, N-L+1)

  for c in xrange(start,0,-1) :
    res_ = solve_lvl( N - c , L - 1, c )

    if len(res_) == 0:
      break

    out.extend([ [c]+l for l in res_ ])

  return out

def solve(N):
  out = []

  for l in xrange(1,N+1) :
    out.extend( solve_lvl(N,l,N) )

  return out
  
if __name__ == "__main__":
  import sys
  X  = int(sys.argv[1]) if len(sys.argv) > 1 else 5

  ll = solve(X)

  for l in ll:
    print " ".join([str(i) for i in l])
  
