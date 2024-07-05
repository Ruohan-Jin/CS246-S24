// 1. 3 lenses for course:
  // 1. effectively programmer i.e. features of languages that help write "better" code
  // 2. software design: how to organize classes effectively
  // 3. compiler: what does the compiler need to support a feature/technique


// 2. Mechanism: C++20
  // => 100% backwards compatible with C


// 3. C++ I/O Intro
  // eg. Hello world
  // 3.1 C: hello.c
  #include <stdio.h>
  int main() {
    printf("Hello world\n");
    return 0;
  }
  // compile: 
  // gcc hello.c 
  // ./a.out


  // 3.2 C++(no modules): hello.cc
  #include <iostream>
  int main() {
    std::cout << "Hello world" << std::endl;
    // std::cout = standard output
    // std::endl = newline + request to flush buffer
    return 0; // implicit
  }
  // compile:
  // g++ hello.cc

  // g++20i hello.cc   
  // ./a.out

  // g++20i hello.cc -o hello   
  // ./hello


  // 3.3 C++20(with modules): hello.cc
  import <iostream>; // 提高编译效率
  // We have access to 3 stream variables/objects
  // standard input:         std::cin          std::istream is the type
  // standard output:        std::cout         std::ostream is the type
  // standard error:         std::cerr         std::ostream is the type
  // ./pgm < in.txt 1>out.txt 2>err.txt
  using namespace std; // allows omission of "std::" everywhere
                       // tell C++ which namespace we are using
  /**
   * std: standard
   * C++ has a standard library that contains common functionality for building applications
   * object/variable/function cannot be accessed directly
   * 
   * cin, cout, cerr, string is all defined under namespace std {...}
   * :: : scope resolution operator
  */
  int main() {
    cout << "Hello World" << endl;
    // return 0; is implicit!
  }
  // compile:
  // g++20h iostream
  // g++20m hello.cc
  // ./a.out
  
  // g++20h : creates gcm.cache, 
  //          compile system headers first, provide these headers as cmd-line argument
  // g++20m : compile your own code


// 4. namespace
using namespace std; // bad style
// just use whatever you need, don't use the entire namespace
using std::cin;
using std::cout;

namespace A {
  int x = 10;
}
int main() {
  int x = 5;
  std::cout << x << std::endl; // 5
}

namespace A {
  int x = 10;
}
int main() {
  int x = 5;
  std::cout << A::x << std::endl; // 10
}

namespace A {
  int x = 10;
}
using namespace A;
int main() {
  int x = 5; // local scope
  std::cout << x << std::endl; // 5
} 

// ERROR
namespace A {
  int x = 10;
}
using namespace A; // 把A里面的变成global scope
int x = 5; // global  scope
int main() {
  std::cout << x << std::endl; 
} 

// ERROR
namespace A {
  int x = 10;
}
namespace B {
  int x = 5;
}
using namespace A;
using namespace B;
int main() {
  std::cout << x << std::endl; 
} 

 
// 5. alias 
// i stands for include
alias g++20i='g++-11 -std=c++20 -Wall -g' 
// h stands for headers
alias g++20h='g++-11 -std=c++20 -fmodules-ts -c -x c++-system-header'
// m stands for modules
alias g++20m='g++-11 -std=c++20 -fmodules-ts -Wall -g'

g++-11                  <-    compiler version
-std=c++20              <-    which standard
-Wall                   <-    warnings as errors
-g                      <-    symbol table info for gdb/valgrind
-fmodules-ts            <-    turn on module support
-c                      <-    create .o from .cc
-x c++-system-header    <-    compiling a system header


// 6. << >>
// operator << and >> return the modified I/O stream on the left; 
//   parameter is the right

// ctrl-d - simulates EOF

//default cin >> ignores whitsepace such as space, tab, \n char to find next char
// what decides the end of the input is based upon the type

// READ EXAMPLE 1: read 2 ints into variables and print the sum of both
int main () {
	int x,y;
	std::cin >> x >> y;
	std::cout << x + y << std::endl;
}
// input:  a b c    output: 0          -> because they are strings not int

// Q: What happens if we try to read an integer and it fails?
// A: type of failure matters and will require different approaches
