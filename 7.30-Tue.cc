// ********** The Algorithm Library (STL) Continued **********

// 5. transform - transforms values using a user-defined function
template <typename InIter, typename OutIter, typename Fn>
OutIter transform(InIter first, InIter last, OutIter result, Fn f) {
  while (first != last) {
    *result = f(*first);
    ++first;
    ++result;
  }
  return  result;
}

// Example
int add1(int n) { return n + 1; }
vector v{2, 3, 5, 7, 11};
vector<int> w(v.size()); // create a vector of 0s of size matching v
transform(v.begin(), v.end(), w.begin(), add1); // w = {3, 4, 6, 8, 12}



// ********** Function Objects **********
// What can we use for f in transform? We have to see how f is used in transform
*result = f(*first);
// f can be anything that can be called as a function with the appropriate 
// parameter and return types. 

// We could write an `operator()` method in a class.
// `operator()` is the function call operator
class Plus1 {
public:
  int operator()(int n) { return n + 1; }
};
// Example:
Plus1 p; // construct a Plus1 object
p(4); // uses the object like a function, produces 5
transform(v.begin(), v.end(), w.begin(), p); // using object like a function

// Plus1 can be easily generalized:
class Plus {
  int m;

public:
  Plus(int m) : m{m} { }

  int operator()(int n) { return n + m; }
};
// Example:
transform(v.begin(), v.end(), w.begin(), Plus{1}); // last arg is a ctor call

// Instances of Plus1 and Plus classes are called 'function objects' 
// -- objects that can behave like a function.

// Advantages of function objects: 
// they can be configured with ctors and can maintain state
class IncreasingPlus {
  int m = 0;

public:
  int operator()(int n) { return n + (m++); } 
  // increases m every time it's called

  void reset() { m = 0; }
};
// Example:
vector<int> v(5, 0); // creates a vector of 5 int values of 0
vector<int> w(v.size()); // destination vector
transform(v.begin(), v.end(), w.begin(), IncreasingPlus{}); // w = {0, 1, 2, 3, 4}

// function objects are used extensively in the STL. We use them to specify 
// what we are searching for in search functions such as `find_if()`, 
// for defining sort criteria for function `sort()`, etc.
struct Person {
  std::string name, address;
};

struct CompareByName {
  bool operator()(const Person& a, const Person& b) const {
    return a.name < b.name;
  }
};

struct CompareByAddress {
  bool operator()(const Person& a, const Person& b) const {
    return a.address < b.address;
  }
};

std::vector<Person> v;
sort(v.begin(), v.end(), CompareByName{});
sort(v.begin(), v.end(), CompareByAddress{});



// ********** Lambdas **********
// Lambda expressions can be a convenient shorthand 
// for function objects or functions.

// Consider the question: how many `int`s in a vector are even?
std::vector<int> v;
// using a function:
bool even(int n) { return n % 2 == 0; }
int num_evens = count_if(v.begin(), v.end(), even); 
// using a lambda expression instead:
int num_evens = count_if(v.begin(), v.end(), [](int n){ return n % 2 == 0; })
// lambda expression    [](int n){ return n % 2 == 0; }
// lambda introducer    []
// parameters           (int n)
// function body        { return n % 2 == 0; }

// A lambda expression is an unnamed function, specified right where it's needed

// Lambda introducers:
// []  -- ordinary function that can access its own parameters, 
//        its own local variables, anything in the global scope
// [&] -- can use names from its enclosing scope, by reference
// [=] -- ... by value(accessing copies of variable from the scope)



// ********** More Uses of Iterators **********
// An iterator is anything that supports the operations *, ++, !=. 
// We can apply the nation of an iterator to other data sources 
// or destinations such as streams.

import <iostream>;
import <iterator>;

std::ostream_iterator<int> osi{std::cout, " "}; // writes ints to cout; 
// ctor takes an output stream, 
// and an optional string to be printed out after each value

*osi = 13; // writes "13 " to cout
*osi = 42; // writes "42 " to cout 
// don't need to advance the iterator, ++osi does nothing

std::vector<int> v{1, 2, 3, 4, 5};
copy(v.begin(), v.end(), osi); // writes "1 2 3 4 5 " to cout

std::istream_iterator<int> isi{std::cin}; // reads from std::cin
int i1 = *isi;
++isi;
int i2 = *isi;

// Now consider:
std::vector<int> w;
copy(v.begin(), v.end(), back_inserter(w)); // constructs an iterator that 
// calls push_back() on w every time operator* is assigned
// Remember: copy() does not allocate space in w. It can't because it doesn't
//           even know what kind of container w iterates over. 
// The back inserter inserts a new item in w each time. Now v is copied to the 
// end of w by adding new items. Back inserters are available for any container 
// with a push_back() method.



// ********** Is Dynamic Casting a Good Style? **********
// Recall dynamic_cast. You can use it to make decisions based on an object's
// run time type information:
void whatIsIt(Book* b) {
  using std::cout;
  if (dynamic_cast<Text*>(b)) { // returns nullptr if unsuccessful
    cout << "Text";
  } else if (dynamic_cast<Comic*>(b)) {
    cout << "Comic";
  } else if (b) {
    cout << "Normal Book";
  } else {
    cout << "Nothing";
  }
}

// But code like this is tightly coupled to the Book class hierarchy, and it 
// may indicate bad design, why? Because what happens if you create another 
// subclass of Book?
// -- whatIsIt() doesn't work anymore until you add a case for the new subclass
// -- must find and fix all similar logic throughout your program 
//    before it will work properly again
// -- easy to miss some

// Therefore, it's better to use virtual methods. Fixing the whatIsIt example:
class Book {
  virtual std::string identify() { return "Normal Book"; }
  // override identify() in Text and Comic

  void whatIsIt(Book* b) {
    using std::cout;
    if (b) {
      cout << b->identify();
    } else {
      cout << "Nothing";
    }
  }
};
// This works by creating an interface function that is uniform across all 
// Book types. Each subclass implements the appropriate logic for the virtual 
// method so you don't need to write code like above that uses dynamic_cast.

// Not all cases of dynamic casting are bad design. 'Dynamic reference' casting 
// offers a posssible solution to the polymorphic assignment problem
// (e.g. if we want to allow assignment through base class pointers).

// Recall :
Book* pb1, pb2;
*pb1 = *pb2;
// We would expect this to work for "like" objects (b1 = b2, t1 = t2, c1 = c2)
// but not "unlike" objects (b1 = t2). To make this work:
class Book {
public:
  virtual Book& operator=(const Book& other); // virtual
};

class Text{
public:
  Text& operator=(const Book& other) override { // override
    const Text& textother = dynamic_cast<const Text&>(other);
    // run time check if the object being copied from is actually a Text.
    // Throws if it is not. Prevens unlike assignment.
    Book::operator=(other);
    topic = textother.topic;
    return *this;
  }
};
