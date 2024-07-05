// ********** Accessors and Mutators **********

// Provide access to private fields: accessors and mutators.
struct Vec {
  private:
    int x, y; 

  public:
    int getX() const { return x; } // accessor methods
    int getY() const { return y; } 
    void setX(const int newX) { x = newX; } // mutator methods
    void setY(const int newY) { y = newY; }
};
// Accessors and mutators like this are simple, but they can do a lot more 
// (validate new values, return derived values, etc.)

// Q: What about operator<<? 
//    Needs access to x and y but it can't be a member function.
// 1. use the accessor methods to get x and y
// 2. make operator<< a friend of Vec, it would have access to x and y directly
struct Vec {
  friend ostream& operator<<(ostream& out, const Vec& v);
};
ostream& operator<<(ostream& out, const Vec& v) {
  return out << v.x << ", " << v.y; // Note access to private x, y
}


// ********** Equality Revisited **********
// If we want to check if two lists are equal, 
// doing a length check is an easy first check.
// Suppose we want to add 'length' to List.
// Option1 : loop through the nodes and count them, O(n)
// Option2 : store the length as a field of list and keep it up to date, O(1)
// Option2 is preferred because it's faster 
//   and adds only a small amount of overhead.
// Now consider <=> in the special case of equality checking.
(l1 <=> l2) == 0 // testing if l1 == l2

class List {
  Node* theList;

public:
  auto operator<=>(const List& rhs) const {
    if (!theList && !rhs.theList) {
      return std::strong_ordering::equal;
    } else if (!theList) {
      return std::strong_ordering::less;
    } else if (!rhs.theList) {
      return std::strong_ordering::greater;
    }
    return (*theList <=> *rhs.theList); // calling the Node <=> operator
  }

  bool operator==(const List& rhs) const {
    if (length != rhs.length) { 
      return false; 
    }
    return (*this <=> rhs) == 0; // implemented in terms of <=> above
  }
};
List m1, m2;
if (m1 <=> m2 < 0) ... // invokes it
List* p = new List;
if (*p <=> m2 < 0) ... // needs to dereference if its a pointer

// Recall that `operator<=>` provides automatic implementation 
//   of the 6 relational operators, but if you write `operator==` separately, 
//   the compiler will use that for both `==` and `!=`. 
//   This lets you optimize equality checks, if possible.
// You can implement both `==` and `!=` if you want a custom `!=` operator,
//   but obviously you don't have to.
//   If you write `operator==`, the compiler will generator `operator!=` 
//   but the reverse is not true.


// ********** System Modeling **********
// Visualizing 'the structure of classes' and 'relationships between classes' 
// using 'diagrams' to aid design and implementation.
// This 'diagram' is called a class diagram
// -- Popular standard we will use is UML(unified modeling language)
// -- We will only cover UML class diagrams
// Modeling a class:
// --------------------
// Vec                     - class name
// --------------------
// -x: integer             - fields (optional)
// -y: integer             - '+' means public, '-' means private
// --------------------
// +getX(): integer        - methods (optional)
// +setX(newX: integer)
// --------------------

// Relationship: Composition
// Recall Basis:
class Basis {
  Vec v1, v2;
};

// UML:
// -----          v1,v2        ---
// Basis      *---------->     Vec 
// -----              2        ---

// *是实心菱形
// v1, v2: fields names
// 2: multiplicity of 2 means exactly 2 Vec objects for every Basis

// Embedding one object(Vec) in another(Basis). 
// A `Basis` is composed of 2 `Vec`s. 
// They are a part of the Basis, and that is their only purpose.
// Relationship: "owns a" relationship.
// a `Basis` owns a `Vec`, actually it owns two of them
// If A owns a B, then typically:
//   -- B has no identity outside A (no independent existence)
//   -- If A is destroyed, then B is destroyed
//   -- If A is copied, then B is copied

// Relationship: Aggregation
// compare the situation of car parts in a car(engine and wheels) 
//   -- the car owns the parts
// compare to a catalog of parts
//   -- but the parts have a separate/independent existence from the catalog.

// Typical implementation:
class Pond {
  Duck* ducks[maxDucks];
};

// UML:
 ------             ------
| Pond | *-------->| Duck |
 ------          *  ------
// multiplicity "*" means 0 or more
// 第一个*应该是空心菱形

// Relationship: "has a" relationship.
// If A has a B, then typically:
//   -- B exists apart from its association with A
//   -- if A is destroyed, then B lives on
//   -- if A is copied, B is not.(shallow copy) Copies of A share the same B.

// Does using a pointer always imply non-ownership(i.e. aggregation)?
// No. Recall List and Node:
//  ----                      --------------    next
// |List|       theList      |Node          | <-----|
//  ----    *---------->      --------------    0..1|
// |    |                    |-data: Integer| *-----|
//  ----                      --------------
// *都是实心菱形

// A Node owns the Nodes that follow it. A List owns the first Node. 
// Note that implementation of the Big 5 is a good sign of ownership.
// But these ownerships are implemented with pointers.

// Another way to view List and Node:
 ------             ------
| List | *-------->| Node |
 ------          *  ------
// *是实心菱形
// This diagram implies that `List` owns all of the `Node`s 
//   and is responsible for deleting all of them.
// Possible iterative(loop-based) management of pointers 
//   rather than recursive routines when `Node`s manage other `Node`s.


// ********** Inheritance / Specialization **********
// Suppose you want to track your collection of books
class Book {
  string title, author;
  int length; // number of pages
};
class Text {
  string title, author;
  int length;
  string topic; // new field
};
class Comic {
  string title, author;
  int length;
  string hero;
};
// This is ok, but it doesn't capture the relationship between Book, Text, and Comic.
// Observe that `Text` and `Comic` are types of `Book`s. 
// `Book`s with extra features. We can model this in c++ using inheritance.
class Book { // base class (or super class)
  string title, author;
  int length;
};
class Text : public Book { // derived class (or subclass)
  string topic;
};
class Comic : public Book { // derived class (or subclass)
  string hero;
};
// inheritance -- : public Book
// Derived class inherit the fields and methods from the base class.
// So text and comic get title, author, and length fields automatically.

// Memory layout for these objects:
   Book             Text             Comic
 --------         --------          --------
| title  |       | title  |        | title  |
 --------         --------          --------
| author |       | author |        | author |
 --------         --------          --------
| length |       | length |        | length |
 --------         --------          --------
                 | topic  |        |  hero  | 
                  --------          --------
// -- subclass fields are added after base class fields
// -- Derived classes inherit the members (fields and methods) from the base class.
// -- Any methods that can be called on `Book` can be called on `Text` and `Comic`.

// Who can see the members?
// -- Anything private in `Book` can't be seen outside `Book`
//      except if `Book` makes a class or a function a friend
// -- Subclasses cannot access private data
// How do we construct / initialize a `Text` object 
//   if it cannot access the private fields in `Book`?
class Text : public Book {
  Text(string title, string author, int length, string topic) : 
    title{title}, author{author}, length{length}, topic{topic} { }
};
// This will not work for 2 reasons:
// -- 1. title, author, and length are not accessible by `Text`. 
//       The MIL only lets you mention your own fields.
// -- 2. Object creation steps:
//       Step1 : space is allocated
//       Step2 : the superclass part is constructed
//       Step3 : fields are constructed
//       Step4 : the constructor body runs

// instead we should do:
class Text : public Book {
  Text(string title, string author, int length, string topic) : 
    Book{title, author, length}, topic{topic} { }
};
// step2 : Book{title, author, length}
// step3 : topic{topic}
// step4 : { } 
