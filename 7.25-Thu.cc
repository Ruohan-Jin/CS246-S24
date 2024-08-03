// ********** Template Method Design Pattern **********
// The Template Method Pattern can be used when we want subclasses to override
// some aspects of superclass behaviour, but other aspects must stay the same.

// For example, we have red and green turtles:
class Turtle {
public:
  void draw() {
    drawHead();
    drawShell();
    drawFeet();
  }

private:
  void drawHead();
  virtual void drawShell() = 0; // Yes, virtual methods can be private
  void drawFeet();
};

class RedTurtle : public Turtle {
  void drawShell() override; // draw red shell
}

class GreenTurtle : public Turtle {
  void drawShell() override; // draw green shell
}

// Here the part that must stay the same is the way a turtle is drawn
// (head, then shell, then feet). Subclasses cannot change this. 
// But they can change the way the shell is drawn.

// The pattern is often used to allow subclasses to customize or provide an
// implementation for 'steps of an algorithm'. The overall algorithm is 
// implemented in the base class as a 'template method' -- a method that 
// provides a template (or the steps) of an algorithm. The base class allows
// the subclasses to provide an implementation for one or more steps.

// Above Example:
// -- draw() is the template method
// -- drawHead(), drawShell(), drawFeet() are the steps
// -- Turtle controls the overall algorithm but it allows the subclass to 
//    customize various steps, like drawShell()

// Definition: defines the skeleton of an algorithm in a method, deferring some 
// steps to subclasses. Subclasses can define certain steps without changing 
// the algorithm's structure.

// Template Method leads into another C++ idiom:



// ********** Non-Virtual Interface (NVI) Idiom **********
// A public virtual method is really two things:
// -- public : provides an inferface to the client
//    promises certain behaviour to the client:
//      pre/post conditions, class invariants, etc.
// -- virtual : provides an interface for subclasses
//    behaviour can be replaced with anything the subclass wants
// These two are at odds with each other : 
//   making promises that subclasses can break

// NVI to the rescue! It says:
// -- All public methods should be non-virtual
// -- All virtual methods should be private, or at least protected
// Expose non-virtual methods to the client. Provide virtual methods to the 
// subclasses to allow them to replace / override certain behaviour.

// Example: Digital Media

// -- Before:
class DigitalMedia {
  // ABC that defines a common interface for playing digital media.
  // ABC stands for Abstrace Base Class
public:
  virtual void play() = 0;
};

// -- Translated to NVI:
class DigitalMedia {
public:
  void play() {
    doPlay();
  }

private:
  virtual void doPlay() = 0;
};

// Now, if we need to exert extra control over play():
// -- We can later decide to add before/after code around doPlay() that 
// can’t be changed (e.g. copyright check before, or update play count after).
// -- We can add more hooks by calling additional virtual methods from play() 
// e.g. showCoverArt().
// - All of this without changing the public interface.

// It’s much easier to take this kind of control over our virtual methods 
// from the beginning than to take back control over them later.

// The NVI idiom extends the Template method pattern by putting 
// every virtual method inside a non-virtual wrapper.
// (That’s how our Turtle example was structured)

// There is essentially no disadvantage in terms of performance, 
// because a good compiler can optimize away that extra function call.



// ********** Template Functions **********

// Similar to template classes, we can create template functions that are
// parameterized by one or more steps:

template <typename T>
T min(T x, T y) {
  return x < y ? x : y;
}

int i1 = 2, i2 = 5;
int i = min(i1, i2); // T = int

double d1 = 3.14, d2 = 13.14;
double d = min(d1, d2); // T = double

// C++ can infer T from the types of the arguments. 
// But if it cannot determine T (e.g. if the function has no arguments)
z = min<int>(x, y);

// Q: For what types T can min(x, y) be used? 
//    For what types T does the body of min compile? 
// A: Any type for which operator< is defined.

// We can generalize a function by 
// making the types of its parameters template arguments.
template <typename Iter, typename Fn>
// "class" could be used instead of "typename", means the same thing
void for_each(Iter start, Iter finish, Fn f) {
	while (start != finish) {
		f(*start);
		++start;
	}
}

// Iter can be any type that supports !=, *, ++, including raw pointers. 
// And Fn can be any function that accepts the type 
// returned by the iterator’s operator*:
void printOut(int n) { 
  cout << n << endl; 
}
int a[] = {1, 2, 3, 4, 5};
for_each(a, a + 5, printOut); // Prints the array.
// Note: a + 5 is just simple pointer arithmetic.



// ********** The Algorithm Library (STL) **********
// The STL algorithm library is a suite of template functions, 
// many of which work with iterators.
import <algorithm>;

// 1. for_each - as described above

// 2. find - searches for a value
template <typename Iter, typename T>
Iter find(Iter first, Iter last, const T& val) {
	// Returns an iterator to the first element in [first, last) matching val,
	// or last if val is not found.
}

// 3. count - like find but returns the number of occurrences of val

// 4. copy - copies one container range to another
template <typename InIter, typename OutIter>
OutIter copy(InIter first, InIter last, OutIter result) {
	// Copies from [first, last) to [result, ...)
	// Note: does not allocate new memory,
	// so output container must have space available.
}

vector v{1, 2, 3, 4, 5, 6, 7};
vector<int> w(4); // Creating a vector of 4 elements
copy(v.begin() + 1, v.begin() + 5, w.begin()); // w = {2, 3, 4, 5}
