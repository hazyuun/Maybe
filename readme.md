# Maybe
A generic Maybe monad library written in C.

I wrote this and a blog post to understand Monads, you can check it out here

https://medium.com/@hazyuun/fda7293a2a66

or in the `explanation.md` file

## Getting started
It's a header only library, so just include it !

## Usage example

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