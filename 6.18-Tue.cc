// ********** Logical V.S. Physical Constness **********
struct Student {
  mutable int numMethodCalls = 0;

  float grade() const {
    ++numMethodCalls;
    return /*...*/;
  }
};

// The mutable numMethodCalls field affects only 
//   the physical constness of the object not its logical constness.
// physical: whether the actual bits/bytes that make up the object have changed.
// logical: whether the updated object should logically 
//          be regarded as different after the update.
// mutable fields can be changed even if the object is const
// Use mutable to indicate that a field does not contribute to 
//   the logical constness of an object.


// ********** Comparing Objects **********

// in C
strcmp(s1, s2); // char* s1, char* s2;
// returns < 0 if s1 < s2
//         = 0 if s1 == s2
//         > 0 if s1 > s2

// in C++
// string s1, s2;
if (s1 < s2) { // first comparison
  // some code
} else if (s1 == s2) { // second comparison, less efficient
  // some code
} else { // s1 > s2
  // some code
}

// We can use the three-way comparison operator <=> (a.k.a. the spaceship operator):
import <utility>;
string s1, s2;
std::strong_ordering result = s1 <=> s2; // one comparison
// shortcut: 
auto result = s1 <=> s2; 
if (result < 0) { // same as result == strong_ordering::less
  cout << "Less";
} else if (result == 0) { // same as result == strong_ordering::equal
  cout << "Equal";
} else { // same as result == strong_ordering::greater
  cout << "Greater";
}
// static fields/constants in the strong_ordering class
strong_ordering::less
strong_ordering::equal/equivalent
strong_ordering::greater

// using auto keyword, compiler automatically deduces the type
auto i = 5; // automatic type deduction, int i = 5;
auto f() { // compiler automatically deduces the return type
  return Student{60, 70, 80}; // from the expression in the return statement
}

// Defining <=> for our own classes
struct Vec { 
  int x, y;
  auto operator<=>(const Vec& other) const {
    auto n = x <=> other.x;
    if (n != 0) {
      return n;
    }
    return y<=>other.y;
  }
};
Vec v1{1, 0};
Vec v2{1, 3};
v1 <=> v2; // strong_ordering::less
// you get all the comparison operator for free by implementing operator <=>
v1 <= v2; // (v1 <=> v2) <= 0;
v1 == v2; // (v1 <=> v2) == 0;
v1 > v2; // (v1 <=> v2) > 0;
// You can sometimes even get the <=> operator for free:
struct Vec {
  int x, y;
  auto operator<=>(const Vec& other) const = default;
};
// This just does lexicographic comparison on the fields of Vec.
// Equivalent to what we wrote before.

// What about Node?
Struct Node {
  int data;
  Node* next;
};
// What does it mean for 2 Node objects to be equal, less, greater?
// Step1. compare data
// If not equal, the one with the lesser value should be less.
// Step2. compare next fields, 4 cases:
//   2.1 both next == nullptr  --> return "equal"
//   2.2 lhs == nullptr and rhs != nullptr --> return "less than"
//   2.3 lhs != nullptr and rhs == nullptr --> return "greater than" 
//   2.4 both next != nullptr --> advance to the next nodes and repeat
struct Node {
  int data;
  Node* next;

  auto operator<=>(const Node& other) const {
    // Step1.
    auto n = data <=> other.data;
    if (n != 0) {
      return n;
    }
    // Step2.
    // Case2.1
    if (!next && !other.next) {
      return n; // std::strong_ordering::equal
    }
    // Case2.2
    if (!next) {
      return std::strong_ordering::less;
    }
    // Case2.3
    if (!other.next) {
      return std::strong_ordering::greater;
    }
    // Case2.4
    return *next <=> *other.next; // recursive call to next nodes in both lists
  }
}


// ********** Invariants and Encapsulation **********

struct Node { 
  int data;
  Node* next;

  ~Node() {
    delete next;
  }
};
Node n1{1, new Node{11, nullptr}};
Node n2{2, nullptr};
Node n3{3, &n2}; // n3's dtor will try to delete n2(on stack), Undefined Behaviour!
// The Node class relies on the assumption that 
//   next is either nullptr or was allocated by new(on the heap).
// This assumption is an example of an 'invariant' 
//   - a statement that must hold true - upon which Node relies
// We can't guarantee this invariant - 
//   we can't trust the client to use Node properly, 
//   because we are exposing the next field, 
//   allowing client to manipulate it directly
// It's hard to reason about programs if you can't rely on invariants.
// To enforce invariants, we introduce 'encapsulation' - 
//   we want clients to treat our objects as black boxes - capsules - 
//   that creates abstractions in which implementation details 
//   are hidden or sealed away such that clients can only 
//   manipulate them via provided methods.

// For example:
struct Vec {
  private: // whatever follows is private, cannot be accessed outside Vec
    int x, y;
  public: 
    Vec(int x, int y) : x{x}, y{y} { }
};

class Vec {
  private:
    int x, y;
  public: 
    Vec(int x, int y) : x{x}, y{y} { }
};
// the only difference between 'struct' and 'class' is the default visibility
// -- struct is public
// -- class is private 

// Let's fix our linked list example by introducing encapsulation. 
// The key is to create a wrapper class List that 
//   has exclusive access to the underlying Node objects.

// list.cc
export class List {
  struct Node; // forward declaration
               // private nested class
               // only accessible inside List
  Node* theList = nullptr;

  public:
    void addToFront(int data);
    int ith(int i);
    ~List();
};

// list-impl.cc
struct List::Node { // Nested Class
  int data;
  Node* next;

  Node(int data, Node* next) : data{data}, next{next} { }
  ~Node() {
    delete next;
  }
};
List::~List() {
  delete theList;
}
List::addToFront(int data) {
  theList = new Node{data, theList};
}
int List::ith(int i) const {
  Node* cur = the List;
  for (int j = 0; j < i; ++j, cur = cur->next);
  return cur->data;
}
// only List can manipulate Node objects 
// so we can guarantee the invariant that next is either nullptr or allocated by new
