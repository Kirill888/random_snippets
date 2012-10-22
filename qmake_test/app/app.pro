CONFIG+=console
TEMPLATE=app
TARGET=app

include(../libt/libt.pri)
include(../common.pri)

SOURCES += main.cc

defineReplace(foo) {
     message("foo called:"  $$1)
     deps = $$1
     message(" ..deps:"  $$deps)

     isEmpty(my_deps) { message(no my deps) }
     else { message("my_deps>>>" $$my_deps) }

     isEmpty(var3) { message(no var3 ) }
     else { message("var3>>>" $$var3) }

     for(d , deps) {
	out += $${d}.foo
	eval( $${d}.foo = $${d} )
	export( $${d}.foo )
     }
     return ($$out)
}


my_deps = blah1 blah2 blah3

bar = $$foo($$my_deps)

var3 = kk


message( "deps:" $$my_deps)
message( "Bar:" $$bar)
message( " exports:" $$blah1.foo )


isEmpty(blah2.foo) {
  message(" blah2.foo is empty: it shouldn't be")
}


defineTest(isFoo){
  a = $$1
  out=0
  contains(a, foo.*): out = 1
  return ($$out)
}


defineReplace(kk){
  return (kk.$${1}.kk)
}


message(" 1:" $$first(one two three) )
message(" 2:" $$second(one two three))
message(" 3:" $$third(one two three) )

message(" concat:" $$concat(foo,bar,baz) )
message(" concat:" $$concat(foo bar baz) )

message(" rest:" $$rest( one two three) )

message(" kk:" $$kk(foo) )
message(" mapcat:" $$mapcat(kk foo bar baz) )
message(" mapcat:" $$mapcat(kk, foo bar baz) )


mylist=kk1 kk2 vv
message(" join:" $$join(mylist, ' ') )
message("apply: " $$apply(concat , 1 2 3 4) )
message("apply: " $$apply(concat 1 2 3 4) )


isFoo(bar): message("bar is foo?")
else      : message("yes bar is not foo")

isFoo(foo): message("foo is foo")
else      : message("foo isn't foo??")


message(" filter:" $$filter(isFoo bar foo baz foo2 foo.k) )
message("!filter:" $$filterNot(isFoo bar foo baz foo2 foo.k) )

message("mapCatIF:" $$mapcat(kk, $$filter(isFoo bar foo foo1 baz foo.Foo j) ) )

my_dict = a:AAA  b:BBB c:CCCC
message( $$my_dict )

message(".... lookup-a:" $$lookup(a, $$my_dict) )
message(".... lookup-b:" $$lookup(b, $$my_dict) )
message(".... lookup-c:" $$lookup(c, $$my_dict) )
message(".... lookup-g:" $$lookup(d, $$my_dict) )
message(".... lookup-X:" $$lookup(DebugShared , DebugStatic:DebugStatic|DebugShared|ReleaseStatic|ReleaseShared|Static|Shared\
						DebugShared:DebugShared|DebugStatic|ReleaseShared|ReleaseStatic|Static\
						ReleaseStatic:ReleaseStatic|Static|ReleaseShared|Shared\
						ReleaseShared:ReleaseShared|Shared|ReleaseStatic|Static ))

s="key:val"
message("replace: " $$replace(s, ":", " ") )
message("split: " $$split(s, ":") )

message("pwd:" $$PWD)
message("pwd.in :" $$IN_PWD)
message("pwd.out:" $$OUT_PWD)

message("root:" $$proj.root)
message("pwd.rel:" $$replace(PWD, $$proj.root, ">>>"))

same($$s,"key:val"): message("same1" )
else         : message("HUH?")

same($$s,$$s): message("same2" )
else         : message("HUH?")

defineReplace(foo.bar){
  out = ____$$1
  return ($$out)
}

message("foo.bar:" $$foo.bar(kkkkk))
message("length:" $$length( 1 2 3 4) )
message("v.split:" $$v.split("boo.b.ees", ".") )

message("inc:" $$inc(1) )
message("dec:" $$dec(1) )


lib1.depends = ""
lib2.depends = lib1
lib3.depends = lib2 lib1
app1.depends = lib3 vendor_boost

defineReplace(expandDepend){
  v=$$1
  deps0 = $$valueOf($$v depends)

  for(d0, deps0 ) {
    deps1 = $$valueOf($$d0 depends)

    for(d1, deps1){
      deps2 = $$valueOf( $$d1 depends )

      out += $$deps2
    }
    out += $$deps1
  }
  out += $$deps0

  return ( $$out )
}

defineReplace(fake_recurse.2){
  count(ARGS,0){
    message("Reached bottom")
    out=.
  }else{
    out="bumming out"
  }

  return ($$out)
}

defineReplace(fake_recurse.1){
  count(ARGS,0){
    message("Reached bottom")
    out=.
  }else{
    message("Recurring")
    out = "boo:" $$fake_recurse.2( $$rest($$ARGS) )
  }

  return ($$out)
}

defineReplace(fake_recurse.0){
  count(ARGS,0){
    message("Reached bottom")
    out=.
  }else{
    message("Recurring")
    out = "boo:" $$fake_recurse.1( $$rest($$ARGS) )
  }

  return ($$out)
}

defineReplace(fake_recurse){
  count(ARGS,0){
    message("Reached bottom")
    out=.
  }else{
    message("Recurring")
    out = "boo:" $$fake_recurse.0( $$rest($$ARGS) )
  }

  return ($$out)
}


x=$$fake_recurse(a b c)
message("xxx:" $$x )


x=$$value.if('', True, False)
message("value.if.f:" $$x)
message("value.if.t:" $$value.if("True", True, False))

apply.test(count x 1): message("yay")
else                 : message("what??")


message(" test2val:" $$test2val("count(x,1)") )
message(" test2val:" $$test2val("count(x,0)") )

eval.test("count(x,1)") : message("yeah")
else                    : message("what?")

eval.test("count(x,0)") : message("what??")
else                    : message("yeah..")


out=''
for(x, __loop10 ){
  message("... " $$x)
  out += ':'
  count(out, 4): break()
}

k=Ki
y=Yi
message("$\$$$k $${k} ")

s="-lsomeLib-lo.12"

matches($$s,"^-l") : message("yay: match")
else               : message("nope")

matchesExactly("-lz-10.3A_g","^-l[-a-zA-Z0-9._]+") : message("yay: matches exactly")
else                 : message("nope")


matches("xxx@$%^&@xxx","foo"): message("Woot!")
else : message("marker.ok")

matches("xxx@$%^&@xxx","xx"): message("marker.ok2")
else : message("Woot!")
