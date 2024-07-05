// ********** Templates **********

// Recall the 'List' class:
class List {
  struct Node {
    int data;
    Node* next;
  };
 
  Node* theList;
};

// If we want to store something other than an `int`, 
//    we can turn this `List` class into a template class.
//    A template class is parameterized by one or more types.
template <typename T> // make List a template class
class List {
  struct Node {
    T data;
    Node* next;
  };
 
public:
  class Iterator {
    Node* p;

  public:
    T& operator*() const;

  };
  void addToFront(const T&);
  T& ith(int);
};

// Possible client code:
List<int> ints; // creates a List object with T = int
List<string> strings; // creates a List object with T = string
List<List<int>> listOfListOfInts; // creates a List object with T = List<int> 
ints.addToFront(5);
ints.addToFront(6);
strings.addToFront("World");
strings.addToFront("Hello");
listOfListOfInts.addToFront(ints);
for (List<int>::Iterator it = ints.begin(); it != ints.end(); ++it) {
  cout << *it << endl;
} 
for (auto n : ints) {
  cout << n << endl;
}
// The compiler specializes the template into actual code as a source level 
//   transformation and then compiles the resulting code as usual


// ********** Standard Template Library (STL) **********
// STL is a large collection of useful templates.
// Example: vectors - dynamic length arrays
import <vector>; 
using namespace std;
vector<int> v1; // vector of ints, creates empty vector
vector<int> v2{4, 5}; // vector of [4, 5]
vector<int> v3(4, 5); // 4 elements of value 5 -> [5, 5, 5, 5]
vector v4{4, 5, 6, 7};  // compiler deduces we want a vector of ints
v1.emplace_back(1); // add the value 1 as an element to the back of v1
v1.push_back(2);    // add the value 2 as an element  to the back of v1

// Using iterators of vectors:
for ( int i = 0; i < v4.size(); ++i ) {
  cout << v[i] << endl;
}
for ( vector<int>::iterator it = v4.begin(); it != v4.end(); ++it ) {
  // iterator should be lower-case!
  cout << *it << endl;
}
for ( int n : v4 ) {
  cout << n << endl;
}
 
// Reverse iterator
for ( vector<int>::reverse_iterator it = v4.rbegin(); it != v4.rend(); ++it ) {
  cout << *it << endl;
}
// Vector based functions:
v4.front(); // returns first element v4[0]
v4.back(); // returns last element
v4.push_back(0); // adds 0 to end of vector
v4.pop_back(); // removes last element, void method
               // doesn't return any data that was popped off
// Vectors are guaranteed to be implemented as arrays.
// Use vectors whenever you need a dynamic length array.

// Removing elements from a vector:
for ( auto it = v.begin(); it != v.end(); ++it ) {
  if ( *it == 5 ) v.erase(it);  // wrong
}
// Consider the vector: [1, 5, 5, 2]
// [1, 5, 5, 2] (index = 0)
// [1, 5, 5, 2] (index = 1)
// [1, 5, 2] (remove at index 1)
// [1, 5, 2] (index = 2)
// 5 wasn't fully removed!!!

// Rule: After an insertion/erase, all iterators pointing after the point of 
//       insertion/erasure are considered invalid and and must be refreshed.
for ( auto it = v.begin(); it != v.end(); ) {
  if (*it == 5) {
    it = v.erase(it); // returns a new iterator to the point of erasure
  }
  else {
    ++it;
  }; 
}
// Note : We can't use a ranged-based for loop because 
// the iterator is "hidden" from us and we can't use the one returned by erase.


// ********** Design Patterns **********
// Iterator Pattern:
// AbstractIterator	                 ConcreteIterator
// + operator*()	                   + operator*()
// + operator++()	                   + operator++()
// + operator!=()	                   + operator!=()

// AbstractAggregate	               ConcreteAggregate
// + begin(): AbstractIterator	     + begin()
// + end(): AbstractIterator	       + end()

// Client uses AbstractIterator and AbstractAggregate

class AbstractIterator {
public:
  virtual int& operator*() const = 0;
  virtual AbstractIterator& operator++() = 0;
  virtual bool operator!=( const AbstractIterator& other ) const = 0;
  virtual ~AbstractIterator();
};
