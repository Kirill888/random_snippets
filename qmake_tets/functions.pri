#This file contains a bunch of helper functions for writing qmake scripts more comfortable.
#
#

# true if both arguments are the same
#
#  unlike built in isEqual that requires first argument to be a name of
#  a variable and a second to be a value this takes values for both
#  arguments
defineTest(same){
  a = $$1
  b = $$2

  isEqual(a,$$b): out=1
  else          : out=0
  return ($$out)
}

#return length of the list as string: i.e. legth( 1 2 3) => "3"
#
# Current implmentation limit is 10, lists longer than that will return +Inf
#
defineReplace(length){
  a=$$1
  count      (a, 0): out =  0
  else: count(a, 1): out =  1
  else: count(a, 2): out =  2
  else: count(a, 3): out =  3
  else: count(a, 4): out =  4
  else: count(a, 5): out =  5
  else: count(a, 6): out =  6
  else: count(a, 7): out =  7
  else: count(a, 8): out =  8
  else: count(a, 9): out =  9
  else: count(a,10): out = 10
  else:              out = "+Inf"
  return ($$out)
}

#concat(a1,a2,...an) -> a1a2a3...an
# go through all input arguments and merge them all together into one string
defineReplace(concat){
  return ($$join(ARGS,'') )
}

# rest( list ) -> list[1:]
defineReplace(rest){
  first=true
  for( v, ARGS ){
    isEqual(first, true): first=false
    else: out += $$v
  }
  return ( $$out )
}

#first (list ) -> list[0]
defineReplace(first){
 return ( $$member(ARGS,0) )
}

#second (list ) -> list[1]
defineReplace(second){
  return ( $$member(ARGS ,1) )
}

#third ( list ) -> list[2]
defineReplace(third){
  return ( $$member(ARGS,2) )
}

#expand(string, separator) -> list
# Like built in split but input is value not variable
defineReplace(expand){
  s=$$1
  sep=$$2
  out= $$split(s,$$sep)
  return ( $$out )
}

# apply(func,a1,a2..aN) -> func(a1,a2,a3...aN)
defineReplace(apply){
  func=$$first($$ARGS)
  args=$$rest($$ARGS)
  eval("out=\$\$"$$func'('$$join(args,',')')' )
  return ( $$out )
}

# apply.test(test_func, a1,a2..aN) -> if( pred(a1,a2,a3...aN) ) True else ''
#
# Same as apply but for test functions.
defineTest(apply.test){
  func=$$first($$ARGS)
  args=$$rest($$ARGS)
  out=0
  eval($$func'('$$join(args,',')'):out=1' )
  return ( $$out )
}

# Takes one argument: code to be evaluated as a test function invocation
# generates a boolean result for condition statement
defineTest(eval.test){
  out=0
  eval("$${1}: out=1")
  return ( $$out )
}

# Takes a test expression and converts into value: '' when false True when true
defineReplace(test2val){
  eval("$${1}: out=True")
  return ( $$out )
}

# mapcat(func, list) -- call function on every element of the list in order
# collecting all results into output list
defineReplace(mapcat){
  func=$$first($$ARGS)
  args=$$rest($$ARGS)
  for( v , args): eval("out += \$\$$${func}( $$v )")
  return ($$out)
}

# filter(predicate, a1,a2,..aN) -> list
# return elements of list for which predicate(a) is true
defineReplace(filter){
  pred=$$first($$ARGS)
  args=$$rest($$ARGS)

  for( v, args ): eval("$${pred}(\$\$v): out += \$\$v")

  return ($$out)
}

# filterNot(predicate, a1,a2,..aN) -> list
# return elements of list for which predicate(a) is false
defineReplace(filterNot){
  pred=$$first($$ARGS)
  args=$$rest($$ARGS)

  for( v, args ): eval("!$${pred}(\$\$v): out += \$\$v")

  return ($$out)
}


# lookup(key, map) -> map[key]
#   where map is encoded as a list of key value pairs using : as separator [k1:v2 k2:v2 .. kN:vN]
defineReplace(lookup) {
  key = $$first($$ARGS)
  map = $$rest($$ARGS)
  sep = ":"

  for(entry, map ){
    kv = $$split(entry, $$sep)
    k  = $$first($$kv)
    v  = $$second($$kv)

    same($$key, $$k): out = $$v
  }

  return ($$out)
}

# inc(v) -> v+1 valid input range 0->10
defineReplace(inc){
  v=$$1
  isEqual      (v, 0): out =  1
  else: isEqual(v, 1): out =  2
  else: isEqual(v, 2): out =  3
  else: isEqual(v, 3): out =  4
  else: isEqual(v, 4): out =  5
  else: isEqual(v, 5): out =  6
  else: isEqual(v, 6): out =  7
  else: isEqual(v, 7): out =  8
  else: isEqual(v, 8): out =  9
  else: isEqual(v, 9): out = 10
  else: isEqual(v,10): out = 11
  else: out="NaN"

  return ( $$out )
}

# dec(v) -> v-1 valid input range 1->11
defineReplace(dec){
  v=$$1
  isEqual      (v, 1): out =  0
  else: isEqual(v, 2): out =  1
  else: isEqual(v, 3): out =  2
  else: isEqual(v, 4): out =  3
  else: isEqual(v, 5): out =  4
  else: isEqual(v, 6): out =  5
  else: isEqual(v, 7): out =  6
  else: isEqual(v, 8): out =  7
  else: isEqual(v, 9): out =  8
  else: isEqual(v,10): out =  9
  else: isEqual(v,11): out = 10
  else: out="NaN"

  return ( $$out )
}

defineReplace(valueOf){
  count(ARGS,1): out=$$eval($$1)
  else         : out=$$eval($$join(ARGS,"."))
  return ( $$out )
}

__loop10 = x . . . . x . . . .

# repeat input 10 times
defineReplace(times10){
  for( v, __loop10 ): out += $$1
  return ( $$out )
}

__loop100  = $$times10(__loop10 )
__loop1000 = $$times10(__loop100)

# value.if(cond, if_true, if_false)
#  if(cond) return if_true
#  else     return if_false
defineReplace(value.if){
  cond=$$1
  !isEmpty(cond): out=$$2
  else          : out=$$3
  return ( $$out )
}

#  func.repeat(cond, func, seed [, MAXLOOP=1000] ) -> func(func(func(...func(s0)...)))
#
#  Repeatedly call function with output of previous call, stop when condition returns false
#
#  s = seed; while( cond(s) ){s=func(s);}; return s;
#
#  Limitation: MAXLOOP can be 10,100,1000
defineReplace(func.repeat){
  cond=$$1
  func=$$2
  s   =$$3
  MAXLOOP=$$4

  count(MAXLOOP,0): loop=$$__loop1000
  loop=$$eval("\$\$__loop$${MAXLOOP}")

  # body = cond($$s): s=$$func($$s)
  for( v, loop){
    apply.test($$cond $$s): s=$$eval("\$\$$${func}(\$\$s)")
    else : break()
  }
  return ( $$s )
}