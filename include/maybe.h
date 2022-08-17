/*
  MIT License

  Copyright (c) 2022 Yuun https://github.com/hazyuun

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef T
#error "Maybe requires a generic type T to be defined as a macro before including maybe.h"
#else

/*
  Generic data types at home
*/
#define GENERIC(name, type) name##_##type

#define Just(TYPE)      GENERIC(Just, TYPE)
#define Nothing(TYPE)   GENERIC(Nothing, TYPE)
#define Maybe(TYPE)     GENERIC(Maybe, TYPE)
#define Bind(TYPE)      GENERIC(Bind, TYPE)


#include <stdbool.h>

typedef struct {
  T       data;
  bool    exists;
} Maybe(T);

Maybe(T) Nothing(T) = {.exists = false};

/* Return */
Maybe(T) Just(T)(T data){
  return (Maybe(T)){.data = data, .exists = true};
}

/* Bind */
Maybe(T) Bind(T)(Maybe(T) m, Maybe(T) (*f)(T)){
  if(!m.exists)
    return Nothing(T);
  return f(m.data);
}

#endif
