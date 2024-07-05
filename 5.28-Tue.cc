// ********** 1. Pass by Reference **********
// Pass by reference is a very powerful mechanism. 
// We can have a function operate directly on an object that we pass by reference.
void swap(double& d1, double& d2) {
  double temp = d1;
  d1 = d2;
  d2 = temp;
}
int main() {
  double x = 5;
  double y = 10;
  std::cout << x << ", " << y << std::endl; // 5, 10
  swap(x, y);
  std::cout << x << ", " << y << std::endl; // 10, 5
}
// Note: the standard library provides a 'swap' function 
//       so you don't have to write your own.


// ********** 2. Returning Values From Functions **********
// 2.1 Return by Value:
Node getMeANode() {
  Node n;
  /* ... */
  return n;
}
Node n1 = getMeANode(); 
// Node created in the function is copied into n1
// "expensive" as we're copying data
// we'll see shortly that this is not so expensive after all
// Can we avoid the overhead of copy by returning a pointer or a reference instead?

// 2.2 Pointer Version:
Node* getMeANode() {
  Node n;
  /* ... */
  return &n;
}
Node* n1 = getMeANode(); 
// BAD! Returns a pointer to stack-allocated data, which is dead on return

// 2.3 Reference Version:
Node& getMeANode() {
  Node n;
  /* ... */
  return n;
}
Node& n1 = getMeANode(); 
// BAD! Returns a reference to stack-allocated data, which is dead on return

// 2.4 Pointer Version - fixed:
Node* getMeANode() {
  Node* n = new Node; // Allocates the Node object on heap
  /* ... */
  return n;
}
Node *n1 = getMeANode(); // GOOD! Alive until you delete it!
// Note: have to remember to call delete 
//   the function transfers ownership of the allocated memory to 
//   the caller of the function. That caller is responsible for 
//   calling delete (or transferring ownership to some other function)
delete n1;

// Caution: never call delete on a stack-allocated(local) object
void f() {
  Node n;
  int a[10]; // a is actually an int *
  Node *np = &n;

  delete n; // BAD, won't compile because delete expects a pointer
  delete &n; // BAD, n lives on stack, compiler warning
  delete np; // same as before, not necessarily caught by compiler
  delete [] a; // BAD! a is on stack
}


// ********** 3. Operator Overloading **********
// Allows us to use build-in operators with user-defined types we create.
// operators: + - * / unary- [] () << >>
struct Vec {
  int x, y;
};
Vec operator+(const Vec& v1, const Vec& v2) { // Note passing by const ref
  Vec v{v1.x + v2.x, v1.y + v2.y}; // Declares and initializes a new Vec.
  return v;
}
Vec operator*(const Vec& v, const int k) {
  return {v.x * k, v.y * k}; // compiler knows we're returning a Vec, 
                             // so this creates and initializes the Vec.
}
Vec v1{1, 2};
Vec v2{4, 5};
Vec v3 = v1 + v2;
Vec v4 = v1 * 10;
Vec v5 = v1 + v2 + (v3 + v4) * 5;
Vec v6 = 10 * v1; // ERROR, so we implement:
Vec operator*(const int k, vec& v) {
  return v * k; // Defined in terms of previous function
  return {v.x * k, v.y * k};
}

// Note: 
// 1. cannot change operator precedence: eg. * always happens before +
// 2. operator+ must have at least one user-defined type parameter
//    包含library里的Ex. string
// 3. cannot override 1 + 2
// 4. common to overload other arithmetic operators too, 
//    like - / unary-(negate a vector) == != < >
// 5. There are shortcuts that let us implement a bunch of these with one function. 
//    We'll explore this later.

using std::istream;
using std::ostream;
ostream& operator<<(ostream& out, const Vec& v) { // does not change v
  out << "(" << v.x << ", " << v.y << ")";
  return out;
}
Vec v1{5, 10};
cout << v1; // (5, 10)
            // calling the operator << function we just defined 
istream& operator>>(istream& in, Vec& v) { // not const because v will be updated
  char p1, c, p2;
  in >> p1 >> v.x >> c >> v.y >> p2; // Note: >> not recursive
  return in;
}
// BTW, why is it okay for operator<< to return an ostream&?
// The out variable is the same reference that was passed in 
//   and refers to an object that exists elsewhere(i.e. not on the stack) 
//   and has a longer life.

string s1 = "Hello ";
string s2 = "World";
string s3 = s1 + s2; // build-in


// ********** 4. Separate Compilation **********
// Allows us to split programs into modules, where each module provides:
// 1. an interface
//    type definitions and function prototypes 
//    similar to header(.h) files
// 2. an implementation
//    full definitions for every provided function
//    similar to .cc files
// Recall: 
// 1. declaration: 
//   asserts the existence of a type, function, global variable; gives it a name
// 2. definition: 
//   full details of a type or function; 
//   allocates space for variables and function bodies

// declarations:
  struct Node;
  extern string parms;
  int f(int a, string b);
  int f(int a, string b); // Declarations can be repeated, 
                          // but must be the same every time
  int f(int n, string s); // Parameter names don't matter.

// definitions:
  struct Node {
    int data;
    Node *next;
  };
  string parms = "p, s, abc";
  int f(int a, string b) {
    // some code
  }

// Note: An entity can be declared many times but can be defined at most once

// interface files start with export module ...;
// implementation files start with module ...;

// compile: 
// g++20m -c vector.cc // creates vector.o, compiles only, does not link
// g++20m -c vector-impl.cc
// g++20m -c main.cc
// g++20m vector.o vector-impl.o main.o -o main // links the program, creates the executable

// Dependency Graph:
// vector-impl.cc和main.cc都指向 vector.cc
