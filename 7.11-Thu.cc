// ********** 1. The Exception Object **********
// out_of_range is a class in the standard library
throw std::out_of_range{"f"}; // constructs the object with "f" as ctor args
// This object is called the 'exception object'.
// Catch handlers can access the exception object 
//   through the parameter in the catch declaration.

// What is the difference between:
catch (std::out_of_range e) { 
  // catch by value
} 
catch (std::out_of_range& e) { 
  // catch by reference
} 
// In C++, we have a maxim "throw by value and catch by reference".
// Catching by reference is usually the right thing to do.

// Catching by value causes the exception object to be 'copied' 
//   into the variable e, which could cause 'slicing' (refer back to 6.27-Thu)
//   if you throw derived class like "Text" but catch a base class like "Book".
//   Catching exceptions by reference doesn't cause slicing.

// Exception types are often part of a class hierarchy of exception classes.
// The out_of_range class is part of an exception hierarchy in standard library.
// base class    : exception
// derived class : logic_error, runtime_error
// out_of_range又是logic_error的derived class
// Subclasses can add additional fields or behaviour,
//   such as capturing details about the specific error that occurred.



// ********** 2. Rethrowing an Exception **********
// A catch handler can do part of the recovery job, 
//   i.e. executing some corrective code,
//   and then rethrow the original exception object.
try {
  // some code
} catch (SomeExceptionType& e) {
  // recovery / clean-up processing
  throw; // no explicit object being thrown
}

// Or throw a 'new' exception:
try {
  // some code
} catch (SomeExceptionType& e) {
  throw e;
  // or
  throw SomeOtherExceptionType{/*some code*/};
}

// suppose exception e is actually a type that is 
//   a subclass of SomeExceptionType, rather than SomeExceptionType itself
// Compare:
throw; // throws the same exception, retaining its actual type
       // without doing any copying   
throw e; // not necessarily the same, discards the original exception object and 
         // creates and throws a new exception of type SomeExceptionType
         // slicing e into SomeExceptionType

// slicing can occur when you are throwing too
// Ex. the object being thrown might be a subclass of SomeExceptionType 
//     rather than SomeExceptionType itself. Throw always throws an object
//     with the same type as the static type of expression
throw Text {
  // some code
} catch (Book& e) { // e is actually a Text object
  throw e; // object being thrown is sliced into a Book object cuz e is a Book&
}
// final may ask this

// Q: What can be thrown?
// A: You can throw anything that you want, you don't have to throw objects

// 23-exceptions/exfact.cc
import <iostream>;
using namespace std;
void fact(int n) {
  if (n == 0) throw 1;
  try {
    fact(n-1);
  }
  catch (int m) {
    throw (n * m);
  }
}
int main() {
  int n;
  while (cin >> n) {
    try {
      fact(n);
    }
    catch (int m) {
      cout << m << endl;
    }
  }
}

// 23-exceptions/exfib.cc
import <iostream>;
using namespace std;
void fib(int n) {
  if (n == 0) throw 0;
  if (n == 1) throw 1;
  try {
    fib(n-1);
  }
  catch (int a) {
    try {
      fib(n-2);
    }
    catch (int b) {
      throw (a + b);
    }
  }
}
int main() {
  int n;
  while (cin >> n) {
    try {
      fib(n);
    }
    catch (int m) {
      cout << m << endl;
    }
  }
}

// 23-exceptions/readInts.cc
#include <iostream>
int main() {
  std::cin.exceptions(std::ios::failbit | std::ios::eofbit);
  int n;
  try {
    while (true) {
      std::cin >> n;
      std::cout << n << std::endl;
    }
  } catch (std::ios::failure&) {

  }
}

// 23-exceptions/readInts5.cc
import <iostream>;
using namespace std;
int main () {
  cin.exceptions(ios::eofbit|ios::failbit);
  int i;
  while (true) {
    try {
      cin >> i;
      cout << i << endl;
    }
    catch (ios::failure&) {
      if (cin.eof()) break;
      cin.clear();
      cin.ignore();
    }
  }
}

// But you can also define your own exception classes, or reuse existing ones
class BadInput {
  // some code
};
int main() {
  try {
    if (int n; !(cin >> n)) throw BadInput{};
  } catch(BadInput&) {
    cerr << "Input not well-formed\n";
  }
}



// ********** 3. Exceptions in Dtors **********
// ADVICE : NEVER let a dtor throw an exception! By default, the program will
// terminate immediately (std::terminate will be called). If a dtor is executed
// during stack unwinding while dealing with an exception and the dtor throws,
// you would now have two active unhandled exceptions, and the program will
// terminate immediately.



// ********** 4. Nested Exception Handlers **********
// It is valid to nest a 'try ... catch'
// inside another try block or a catch handler.
try {
  try { // FROM HERE

  } catch ( ) {

  } // TO HERE is the nested try ... catch
} catch ( ) {
  try { // FROM HERE
    // clean-up code
  } catch ( ) {

  } // TO HERE is the nested try ... catch
}

// Q: Why exceptions? 
// A: Exceptions make your code simpler, cleaner, and less likely to miss errors
// See https://isocpp.org/wiki/faq/exceptions
// really good resource about all details of exceptions



// ********** 5. Exception Safety **********
// Consider:
void f() {
  C c;            // stack-allocated
  C* cp = new C;  // heap-allocated
  g();
  delete cp;
}
// Looks correct, no memory leaks. What if `g()` throws?
// What is guaranteed? During stack unwinding, all stack-allocated data 
// is cleaned up: dtors run, memory is reclaimed. But allocated heap memory
// is not reclaimed. If `g()` throws, c is destroyed, but *cp is not. 
// Memory Leak! Could we do:
void f() {
  C c;
  C* cp = new C;
  try {
    g();
  } catch () {
    delete cp;
    throw;
  }
  delete cp;
}
// This works but it is ugly and error-prone, and we have code duplication. 
// It gets complicated as we add more logic.
// How can we guarantee that something(here, `delete cp;`) will happen 
// no matter how we exit `f()` (normally or by exception)?
// You can count on dtors for stack-allocated data to run. 
// Therefore, use stack-allocated objects with destructors as much as possible 
// - use that guarantee to your advantages



// ********** 6. C++ idiom: RAII **********
// RAII stands for Resource Allocation Is Initialization
// which says that every 'resource' should wrapped in a stack-allocated object, 
// where job is to release the resource.
// Example:
{
  ifstream f{"file.txt"};
}
// Acquiring the source(here, the file) happens by initializing the object f.
// The file is guaranteed to be released(here, closed) when f goes out of scope.

// This can also be done with dynamic memory:
import <memory>;
void f() { // function f rewritten to use RAII
  C c;
  std::unique_ptr<C> cp{new C};
  g();
}
// new C    : constructs C on heap, returns a ptr to it
// {new C}  : unique_ptr ctor params
// The unique_ptr's dtor will delete the pointer. We can dereference the cp 
// object just like a regular pointer. We call this a 'smart pointer' because
// it automatically frees memory (dumb pointers don't do anything) 
// when it goes out of scope. Smart pointers should always be stack-allocated.

// Another way to create a smart pointer object:
void f() {
  std::unique_ptr<C> cp = std::make_unique<C>();
}
// std::make_unique  : function
// <C>               : type of pointer being wrapper
// ()                : ctor args for C's ctor
// `make_unique` internally calls `new C{}`, forwaring the arguments in the 
// parentheses to C's ctor

// 26-unique_ptr

// What if we try this:
std::unique_ptr<C> cp{new C};
std::unique_ptr<C> cp2 = cp; // calling cctor to copy cp
// `cp` and `cp2` now point to the same heap memory. We're trying to 
// copy a unique_ptr, but we don't want to delete the same ptr twice. 
// Therefore copying is disabled for unique_ptr. They can only be 'moved'.

// If you need to copy pointers, you first need to 
// answer the question of 'ownership' :
// 1. Who will 'own' the resource 
//    (i.e. who will have the responsibility for freeing it)
// 2. That ptr should be a unique_ptr object, 
//    all other ptrs should be raw pointers
// 3. can access the underlined raw pointer with `cp.get()`

// This leads to a new understanding of pointers:
// 1. unique_ptr  : indicates ownership
// 2. raw pointer : indicates non-ownership since the raw pointer is consider
// not to own the resources it points at, you should not delete it 
// 3. moving a unique_ptr (into or out of a function) means transfer ownership
