# Maybe you will understand monads here

Monads are just monoids in the category of endofunctors what's the problem ?

*insert rickroll gif here*

Seriously, wtf is a monad ?? Well hopefully it's gonna become clearer after reading this article.

Ladies and gentlemen, welcome to attempting to understand wtf are monads

## Attempt 0
Monads are just monoids in the category of endofunctors

## Attempt 1
Alright, imagine we want to wrap a type like `int` or `float` in a struct containing "metadata" fields.


As an example, lot of times when programming, you need to do error checking to check the validity of the data (checking for null, undefined, NaN, etc.), so a way to solve this problen is to make a wrapper like said above, where the "metadata" is whether the data exists or not

```C
struct {
  int   data;    /* The actual data     */
  bool  exists;  /* Is the data valid ? */
}
```

If the `exists` field is true then we can use the `data` field, otherwise, `data` is likely to contain a garbage value, so basically an instance of this struct, is just an int that maybe exists maybe not, lets call it `Maybe`

```C
struct {
  int   data;    /* The actual data     */
  bool  exists;  /* Is the data valid ? */
} Maybe;
```

You may be thinking, well why not check the data directly ? well that's perfectlty valid, but the code becomes cluttered with those `if(value == null || value == undefined)`

Alright, now we need a way to transform our usual `int` variables into a Maybe version, that's easy, we can write a function to do that

```C
Maybe Just(int n){
  return (Maybe) {.data = n, .exists = true};
}
```

it's "Just" an int, and we know that it exists because it's there.

But what if it is not ? it's Nothing, and we can define it as :

```C
Maybe Nothing = {.exists = false}; // the data field can contain anything

```

Alright, we now can use those two like this 
```C
Maybe a = Just(50);
Maybe b = Nothing;
```

But, how do we avoid the repetitive error checking with this ? I mean, we still have to check for the `exists` field before using the actual `data` right ? 

Well till now, yes, but here's the thing :

Imagine we want to call a function on that int, that squares it for example.
The trick is not to call that function directly, instead, we define a function that calls it after doing the validity check, like this :

```C
Maybe Bind(Maybe m, Maybe (*f)(int)){
  if(!m.exists)
    return Nothing;

  return f(m.data);
}
```

so basically, Bind takes the Maybe, and the function we wanna call, then it checks if the Maybe value exists, if it does then the function is called, otherwise, it just returns Nothing.

Note that the function that we wanna call, takes an int (not a Maybe), and returns a Maybe.

Now that we have all this, we can avoid error handling very easily, like this :
```C
Maybe square_positives(int x){
  if(x < 0) return Nothing
  return x*x
}

Maybe a = Just(50);
Maybe b = Nothing;
Maybe c = Just(-1);

Maybe x = Bind(a, square_positives); // x.data contains 50*50 I'm too lazy to calculate it 
Maybe y = Bind(b, square_positives); // y.data contains Nothing
Maybe z = Bind(c, square_positives); // z.data contains Nothing

```

Note that we didn't bother to check for the existance of the data, we just let Bind do it.

Also, because Bind actually returns a Maybe, we can actually chain Bind calls ! that's where we can really feel that it's useful, because we won't have to check for the existance at each step

```C
Maybe square_positives(int x){
  if(x < 0) return Nothing
  return x*x
}

Maybe opposite(int x){
  return -x;
}

Maybe a = Just(50);

Maybe x;
x = Bind(a, square_positives);  // 2500 (suddely I got less lazy and calculated it)
x = Bind(x, opposite);          // -2500
x = Bind(x, square_positives);  // Nothing (because it's negative)
x = Bind(x, opposite);          // Nothing (because we passed Nothing)
```

That's great ! but wait a second, wtf is a monad ??? 
Well, now that we have seen all of this, we can answer that quesion very easily ! A monad is just a monoid in the category of endofunctors ! 

Haha seriously, a monad is just a generalization of what we saw here, I mean

- A wrapper around some data
- A Just function that takes data and wraps it
- A Bind function that calls a function over the data after doing something

In this example, we had

- The wrapper is the Maybe struct
- The Just function is the Just function
- The Bind function is the Bind function

Hopefully you got a basic idea, now we will make a generic Maybe library in C, that can be used with any data type 

## Bonus : Writing a generic Maybe library in C
But C doesn't have generic data types does it ? Well, it doesn't, but we can use generics anyway

*Insert gigachad GIF here*

We can make generics possible, by expecting a T macro to be defined before including the header file, for example
```C
/* We want int Maybe*/
#define T int
#include <maybe.h>
#undef T

/* And float ones */
#define T float
#include <maybe.h>
#undef T
```

As you probably noticed, we actually included maybe.h twice, so we need to be careful
- No include guards or pragma once in maybe.h
- No stuff that will be defined twice (and therefore causes errors) if we include maybe.h twice

Alright, so now that we have a T type defined, we can use it inside maybe.h

```C
typedef struct {
  T       data;
  bool    exists;
} Maybe;
```

This will work fine, but will break as soon as we include maybe.h the second time, because the Maybe struct will be defined 2 times, that's why we want to append a suffix (or prefix) to the name of the struct depending on the T type, that can be done using macros 

We define a Maybe(T) macro, that concatenates Maybe to the type

```C
#define Maybe(TYPE)     Maybe_##TYPE
```
This looks good, but it's not gonna work if the type itself is in a macro (the T in our case), `Maybe(T)` is gonna expand to Maybe_T in all scenarios, so we need to actually expand the T first, so we add an extra layer of indirection, like this 
```C
#define GENERIC(name, type) name##_##type

#define Maybe(TYPE)     GENERIC(Maybe, TYPE)
```

Cool ! now we can have multiple Maybe versions depending on the T
```C
Maybe(int)
Maybe(float)
Maybe(char)
...
```

Looks familiar ? it resembles the `<T>` thing in languages that support generics

Alright, now we can define the maybe struct like this :
```C
typedef struct {
  T       data;
  bool    exists;
} Maybe(T);
```
And it's gonna work just fine even if we include maybe.h more than once, as long as T is different each time.

Now we can define the Nothing using that struct, first we need to define it as a generic thing

```C
#define Nothing(TYPE)     GENERIC(Nothing, TYPE)
```
Then we define it

```C
Maybe(T) Nothing(T) = {.exists = false};
```
In case you are still confused, imagine T is int, the above will expand to :
```C
Maybe_int Nothing_int = {.exists = false};
```
Where `Maybe_int` is the struct, and Nothing_int is the name of the nothing variable.

Alright, now we need to define the `Just` and `Bind`, First we define them as generic things
```C
#define Just(TYPE)      GENERIC(Just, TYPE)
#define Bind(TYPE)      GENERIC(Bind, TYPE)
```
And we define them as we've seen above

```C
Maybe(T) Just(T)(T data){
  return (Maybe(T)){.data = data, .exists = true};
}
```
Just just wraps the data into a Maybe struct

```C
Maybe(T) Bind(T)(Maybe(T) m, Maybe(T) (*f)(T)){
  if(!m.exists)
    return Nothing(T);
  return f(m.data);
}
```
And Bind unwraps it, checks for existance, then calls a function on the data, returning another Maybe-wrapped value so we can chain Bind calls.

Cool ! Now we have everything ready to use ! Here is the full file to see the whole picture

```C
#ifndef T
#error "Maybe requires a generic type T to be defined as a macro before including maybe.h"
#else

/*
  Generic data types at home
*/
#define CONCAT(x, y) x##_##y
#define GENERIC(name, type)  CONCAT(name, type)

#define Just(TYPE)      GENERIC(Just, TYPE)
#define Nothing(TYPE)   GENERIC(Nothing, TYPE)
#define Maybe(TYPE)     GENERIC(Maybe, TYPE)
#define MaybeBind(TYPE) GENERIC(MaybeBind, TYPE)

#include <stdbool.h>

typedef struct {
  T       data;
  bool    exists;
} Maybe(T);

Maybe(T) Nothing(T) = {.exists = false};

Maybe(T) Just(T)(T data){
  return (Maybe(T)){.data = data, .exists = true};
}

Maybe(T) MaybeBind(T)(Maybe(T) m, Maybe(T) (*f)(T)){
  if(!m.exists)
    return Nothing(T);
  return f(m.data);
}

#endif
```
Nice ! Now we can use that !

```C
#include <stdio.h>
#include <math.h>

#define T int
#include <maybe.h>
#undef T

#define T float
#include <maybe.h>
#undef T

/* Some usual functions */
Maybe(int)    opposite_i(int a);
Maybe(float)  opposite_f(float a);

Maybe(int)    square_i(int a);
Maybe(float)  square_f(float a);

Maybe(int)    sqrt_i(int a);
Maybe(float)  sqrt_f(float a);


int main(void){

  // Example using int
  Maybe(int) a = Just(int) (50);

  Maybe(int) x;
  x = Bind(int)(a, square_i);     // 2500
  x = Bind(int)(x, opposite_i);   // -2500
  x = Bind(int)(x, sqrt_i);       // Nothing (because it's negative)
  x = Bind(int)(x, opposite_i);   // Nothing (because we passed Nothing)


  // Example using float
  Maybe(float) pi = Just(float) (3.1415);

  Maybe(float) y;
  y = Bind(float)(pi, square_f);     // 9.869022
  y = Bind(float)(y, opposite_f);   // -9.869022
  y = Bind(float)(y, sqrt_f);       // Nothing (because it's negative)
  y = Bind(float)(y, opposite_f);   // Nothing (because we passed Nothing)

  return 0;
}



Maybe(int) opposite_i(int a){
  return Just(int) (-a);
}
Maybe(float) opposite_f(float a){
  return Just(float) (-a);
}

Maybe(int) square_i(int a){
  return Just(int) (a * a);
}
Maybe(float) square_f(float a){
  return Just(float) (a * a);
}

Maybe(int) sqrt_i(int a){
  if(a < 0) return Nothing(int);
  
  int s = (int) sqrt(a);
  
  return Just(int) (s);
}
Maybe(float) sqrt_f(float a){
  if(a < 0) return Nothing(float);
  
  float s = (float) sqrt(a);
  
  return Just(float) (s);
}


```
