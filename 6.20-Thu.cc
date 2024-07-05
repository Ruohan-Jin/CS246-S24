// Using our new List class:
List lst;
lst.addToFront(1);
lst.addToFront(2);
lst.addToFront(3);
for (int i = 0; i < 3; ++i) {
  cout << lst.ith(i) << endl;
}
// Output: 
// 3
// 2 
// 1
// Time Complexity for ith() is O(n^2)


// ********** Iterators **********
// Iterators allow us to easily and efficiently iterate over the items in the list.
// Iteration is a common pattern in programming. 
// C++ has a standard way that iteration works.
// The general idea is to create a new 'Iterator' class 
//   that gives us access to the nodes:
//   -- it's an abstraction of a pointer
//   -- lets us walk the list without exposing the actual pointers
// It works like this:
//   -- The List class has a method called 'begin()' that 
//      creates and returns an Iterator object that points to the first node.
//   -- Using this iterator object you can 'access the data' in the node.
//   -- You can 'advance' the iterator so that it points to the next node.
//   -- The List class also has an end() method that creates and returns 
//      an iterator object pointing to 'one position past' the last node.
//   -- When your iterator 'equals' the iterator returned by end(),
//      you've reached the end of the list.
// We access that data with the 'it' iterator.(*it) 
// The end() iterator object is only used for comparison. 
// Using it to access data or advancing it would fail in most cases. 
int main() {
  List lst;
  lst.addToFront(1);
  lst.addToFront(2);
  lst.addToFront(3);

  List::Iterator it = lst.begin(); // create iterator object
  while (it != lst.end()) { // check if we've reached the end of the list
                            // (compares two iterator objects)
    cout << *it << endl; // access data using the iterator
    ++it; // advance the iterator
  }
}

// For-loop version: (you can use auto for while-loop as well)
for (auto it = lst.begin(); it != lst.end(); ++it) {
  cout << *it << endl;
}

// Range-based for-loop:
for (int n : lst) { // shortcut syntax, reads as "for each int n in lst"
  cout << n << endl; 
}

// Mutate list items or avoid copying each item:
for (int& n : lst) { // using a reference avoids copying
  n *= 2;            // and lets us mutate the data
}                    // our list now contains 6 4 2

// The range-based for loop is available for any class with:
// - methods `begin()` and `end()` that produce iterators
// - The iterator class must support the `*`, `++`, and `!=` operator.
// The compiler "re-writes" the for loop as a traditional for loop, 
//   using the `begin()` and `end()` methods,
//   and the `*`, `++`, and `!=` operators.

// The code to make this all work:
class List {
  struct Node;
  Node* theList = nullptr;

  public:
    class Iterator { // Nested class
      Node* p; // Points to current node

      public:
        explicit Iterator(Node* p) : p{p} {} // ctor
        int& operator*() const { // access the data
          return p->data;
        }
        Iterator& operator++() { // advances the iterator
          p = p->next;
          return *this;
        }
        bool operator==(Iterator& other) const { // by implementing ==, 
                                                 // we get != for free
          return p == other.p;
        }
    }; // class Iterator

    Iterator begin() const {
      return Iterator{theList};
    }
    Iterator end() const {
      return Iterator{nullptr};
    }
    // other List methods like big 5.
}; // class List

// List client code can still create iterator objects directly:
List::Iterator it = List::Iterator{nullptr};
// This violates encapsulation. 
//   Only begin() and end() should be allowed to create iterators.
// If we make the Iterator ctor private, 
//   then the client code can't do this,
//   but neither can List.

// Solution: give the List class privileged access 
//           to the Iterator class by making List a 'friend'.
class List {
  ...
  class Iterator {
    private: // Technically optional because these members are private by default
      Node* p;
      explicit Iterator(Node* p) : p{p}; // ctor is private

      friend class List; // gives List access to the private members of Iterator.
      // Note: it doesn't matter where in the class Iterator you put this.
  };
};
// Now List can create Iterator objects 
//   but the client code can only create Iterators 
//   by calling begin() and end()
// The friend keyword:
//   The Iterator class is saying that List is a 'friend' 
//   and can be trusted to access the private members correctly.


// ********** Nested Classes **********
class List {
  class Node {

  };
  class Iterator {

  };
};
// These classes are "nested" (defined) inside the List class. 
// Their names are List::Node and List::Iterator.
// At runtime, when you create instances of these classes (objects), 
// the objects can be related in many different ways 
// depending on how you use the classes. 

// Don't be confused by nested classes and think that 
// creating a List object automatically creates a Node object and an Iterator object.

// Nested classes allow you to reuse class names (Iterator) 
// so you don't have to create unique names for each class.

// They allow you to create "local" classes that 
// are private and used internally as an implementation detail (Node).

// An instance of a nested class has access to 
// the private members (fields and methods) of its "wrapping" outer class, 
// as long as it has a way to get to an outer object 
// (through a pointer/reference/object).

// if Iterator has access to a List object, 
// it would be able to access the private members of List.
