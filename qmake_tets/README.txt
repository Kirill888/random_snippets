qmake has rather strange and poorly documented scripting language. It
is not a proper language and it has rather flaky syntax (some examples
below). This project is an experiment to understand its limitations
and possibilities.

Types:
-----------------------

The only types are:
* Strings
* Lists of Strings
* Transform Functions: List -> List
* Conditional Functions: List -> Bool

Bool type can only be returned from what qt calls "test function", it
can not be assigned to a variable. There are very few built in
functions, but user can define their own.

Variables are all lists of strings there is no distinction between a
string and a list of one string. There are no numerical type, but
there is a numerical constant that can only be used in the context
test function 'count' and function 'member'.

 x=one two three
 count(x,3): message("Yes variable x is a three a element list")

 message("first element of x is: " $$member(x,0) )

To assign to a variable:
 x = word

To append 
 x += another
 x += several at the same time also

To get values out of variables use $$var or $${var}
 y = $$x $$x

Just like in shell $$var can be expanded in-place inside string literals.
  y = "one of $${x}."


NOTE: most built-in functions take variable name, not its value. This
is actually quite annoying, that means you can't pass results of
functions to those functions directly, you have to assign them to some
variable first then pass name of the variable.

Example:
  x=$$member( $$split("some|stuff","|"), 0)  # This is not going to work as epxected

  # Has to have two extra named vars, to achieve that
  s="some|stuff"
  ss=$$split(s,"|")
  x = $$member(ss,0)



Conditionals:
-----------------------

testStatemnt : single_line_action_if_True
else         : single_line_action_if_False


testStatemnt {
  multi
  line
  action
  when true
}else {
  multi
  line
  action when false
}


When using second form opening curly brace '{' must be on the same line as testStatement,
similarly for else, it has to be followed either by : or by '{'. 
 cond { x=y
 }
  
 cond {
 x=y}

Both allowed, but not 
  cond {x=y}

For that use:
  cond : x = y


Iterations:
--------------------------------

The only iteration construct is what is essentially foreach

collection=one two three
for( it , collection ){
  message("element:" $$it)
}

NOTEs: 

* There should be no space between 'for' and '(' it has to be 'for(' spaces elsewhere don't seem to matter.
* Just like conditionals '{' has to be on the same line
* Just like conditionals you can use ':' for single statement loops.


User Declared Functions
---------------------------------
There are two types:

* Data mapping functions List -> List
* Data test functions List -> Bool

TODO

Recursion
---------------------------------

Not disallowed but seems to seg-fault interpreter on first return from
the call chain or soon after. I suspect there is some state that is
per-function rather than per function call and it gets cleared on
return, but is still in use by calling function.


