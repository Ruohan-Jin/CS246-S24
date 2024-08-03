// ********** 1. What Should Go Into a Module **********

// When should classes be grouped together in a module, 
//   and when should they be in separate modules?
// To answer this, we can explore two 'measures of design quality': 
//   coupling and cohesion.


// 1.1 Coupling
// Coupling refers to how much distinct program elements
//   (classes, functions, modules) depend on one another.
// - 'High coupling (tightly coupled)' means that the elements are closely 
//     connected and changes in one many have a rippled effect on others. 
//     This makes it harder to reuse individual elements.
// - 'Low coupling (loosely coupled)' means the elements are independent, 
//     and changes in one element have little effect on others. 
//     Ex. Observer Pattern

// You do need some degree of coupling. Loosely coupled elements can interact 
//   but they generally have little knowledge of each other. Design with loose 
//   coupling allow us to build flexible OO systems that can handle 
//   change because they minimize the interdependencies between object.


// 1.2 Cohesion
// Cohesion refers to the degree to which elements of a module work together 
//   to fulfill a single, well-defined purpose.
// - 'High cohesion' means that 
//     the elements can closely related and focus on a single purpose.
// - 'Low cohesion' means that 
//     the elements are loosely related and serve multiple purposes.
// Note. The elements could be methods of a class or classes in a module, etc.

// Low cohesion implies poorly organized code. 
//   It's harder to reuse something without getting other stuff bundled with it.
// High cohesion is related to a design principle:
//   Single Responsibility Principle 
//   -- "a class should have only one reason to change"
//   It means that you should strive to design your classes so they only have 
//   one responsibility, one reason to change. If a class does multiple things, 
//   there are more reasons to go back to the class and make changes, 
//   and changes means problems can creep in. When your classes 
//   does multiple things, it might affect multiple aspects of your design.

// GOAL : Strive for low coupling and high cohesion


// 1.3 What should go in a module?
// Won't compile:
class A {
  int x;
  B y;
};
class B { 
  char x;
  A y; // Don't know how big A or B are
};

// Won't compile:
class B;
class A {
  int x;
  B y;
};
class B {
  char x;
  A y; 
};

// Solution:
class B; // Forward reference
class A {
  int x;
  B* y; // and change this to a pointer.
};

// How should A and B be placed into modules?
// Modules must be compiled in dependency order. You can't forward declare
//   another module or any items within another module. 
//   Therefore, A and B must reside in the same module.
//   This makes sense since A and B are tightly coupled.

// Sometimes one class must come before others
class C { 

};
class D : public C { 

}; // we need to know the size of C to construct D.
// 这种情况下不能forward declare C, 再在D的definition之后define C



// ********** 2. Decoupling the User Interface (MVC) **********

// What would be wrong with this?
class chessBoard { // has the core logic to maintain state of the chess board
  ...
  std::cout << "Your move" << std::endl; 
}; // This makes reuse harder because it interacts with the user directly

class chessBoard { 
  istream& in;
  ostream& out;
  ...
  out << "Your move" << std::endl; 
}; // could possibly put the user interaction in 'main', but hard to reuse too

// need to separate the user interaction from the game state 
// single responsibility principle is a guide
// Model-View-Controller(MVC) Pattern separates distinct responsibilities of
// 1. Model:
//   1.1 state(or data) and the application logic or rules that change the state
//   1.2 can have multiple views (e.g. text and graphics views)
//   1.3 doesn't need to know about their details
// 2. View:
//   2.1 they presents the state in the user interface
//   2.2 gets the state from the model
//   2.3 model and view typically use the Observer pattern
// 3. Controller:
//   3.1 mediates the flow of control between the model and view
//   3.2 accepts input (from the view or other sources like keyword) 
//       and converts it to actions on the module or view
// By decoupling application state, presentation, and control, MVC promotes reuse



// ********** 3. Exceptions **********
// Recall accessing the i-th element of a vector v:
// v[i]    : No bounds checking on it, unchecked
// v.at(i) : Checks that i is within bounds, a bounds-checked version of v[i]

// What happens if you go out of bounds?
// C : functions might use special return values or the global `errno` variable.
//     Leads to awkward programming. Encourages programmers to ignore error checks.
// C++ : when an error condition is detected, instead of returning normally,
//       the function 'throws an exception' indicating what went wrong.

// The fundamental idea behind exceptions is to separate 
//   detection of an error (which should be done in the called function) from 
//   the 'handling' of an error (which should be done in the caller function),
//   while ensuring the detected error 'cannot be ignored'.

// When an exception is thrown, by default program execution stops.
//   But we can write 'handlers' to catch exceptions and deal with them.

// `at()` throws an exception of type std::out_of_range when i is out of bounds
// handle it as follows:
#include <stdexcept> // standard exception types
try { // statements that might throw an exception go in a try block
  std::cout << v.at(i) << std::endl; // may throw an exception
  std::cout << "This does not get printed if exception is thrown" << std::endl;
} catch (std::out_of_range e) {
  std::cout << "Range error: " << e.what() << std::endl; 
  // e.what() gives details about the error
}
// the exception causes execution to transfer to the catch block

// Consider
void f() {
  throw std::out_of_range{"function f"}; 
  // "function f" is the value returned by e.what
}
void g() {
  f();
}
void h() {
  g();
}
int main() {
  try {
    h();
  } catch (std::out_of_range e) {
    std::cout << e.what() << std::endl; 
  }
}
// main->h->g->f->throw
// control goes back up the call chain (called stack unwinding) 
//   until a handler is found, all the way back to main()
//   If there is no matching handler in the entire call chain,
//   the program terminates.

// You can have more than one handler to catch different types of exceptions
try {
  // some code
} catch (std::out_of_range e) { 
  // some code
} catch (std::bad_alloc e) { // exception that `new` throws when fail
  // some code
} catch (...) { // the catch-all handler, catches any exception type
  // some code
}

// as part of stack unwinding, any stack-allocated objects 
//   are destroyed automatically(i.e. their dtors run)
