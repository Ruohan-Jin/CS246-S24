// ********** Pointers as Parameters **********
// moving a unique_ptr into a function:
void f(unique_ptr<C> p); 
// f will take over ownership of the object pointed to by p,
// caller loses custody of object
// here the parameter p is pass by value

unique_ptr<C> cp{new C}; // cp manages the C object
f(std::move(cp)); // transfers ownership of the managed object to f 
                  // cp would now be empty

void g(C* p); // g will not take ownership of the object pointed to by p
              // caller's ownership of the object does not change 



// ********** Pointers as Return Values **********
// moving unique_ptr out of a func
unique_ptr<C> f(); // return by value is always a 'move'
                   // so f handles over ownership of the C object to the caller 

C* g(); // the raw pointer is understood not to be deleted by the caller
// it might be a pointer to non-heap or heap data that someone else already owns



// ********** Shared Ownership (shared_ptr) **********
// when you do need true shared ownership, i.e., any of several pointers
// might need to free the resource, then we use a share pointer.
{
  auto p1 = std::make_shared<C>(); // allocates a C object on the heap
  if () {
    auto p2 = p1; // two pointers pointing to the same object, copy is allowed
  } // p2 goes out of scope, but the object is not deleted yet
} // p1 goes out of scope, the object is deleted

// shared_ptr maintains a reference count - a count of all shared pointers 
// pointing at the same object. Memory is freed when the number of shared_ptrs
// pointing to it reaches zero.

// Summary: Use the type of pointer (raw, unique, shared) that accurately
// reflects the pointer's ownership role, leading to dramatically 
// fewer opportunity of memory leaks



// ********** STL Maps **********
// Maps are also known as associate arrays or dictionaries. 
// Very useful container types. Store (key, value) pairs. 
// Key is unique, values can be set and updated.
// Example:
import <map>;
std::map<std::string, int> m; // constructs an empty map
m["a"] = 2;
m["b"] = 3;
std::cout << m["a"] << std::endl; // 2
std::cout << m["b"] << std::endl; // 3
std::cout << m["c"] << std::endl; // if key is not found, 
                                  // it is inserted and 
                                  // the value is default constructed, 0 for int
m.erase("a"); // removes the (key, value) pair from array
m.count("b"); // 0 for not found, 1 for found
// map stores their keys in sorted order
for (auto& p : m) { // iterate over a map in sorted key orders
  // p's type here is <std::string, int>& 
  std::cout << p.first << "=" << p.second << std::endl;
}

// pair is defined in <utility>
// Pair is basically just:
struct Pair {
  T first;
  U second;
};

// Alternatively:
for (auto& [key, value] : m) { // called a structure binding,
  // decomposes the pair objects into two local variable called key and value.
  // Key and value are references. We can actually change the value 
  // using this variable, but the key cannot be changed.
  std::cout << key << "=" << value << std::endl; 
}

// These structured bindings can be used on any structure (class) type 
//   with all fields public:
Vec v{1, 2}; // assuming public fields
auto[x, y] = v; // x = 1, y = 2
// use on stack arrays where size is known:
int a[] = {10, 20, 30};
auto[x, y, z] = a; // x = 10, y = 20, z = 30



// ********** Inheritancy and Copy/Move **********
// When you have an inheritance hierarchy, you need to do a few special things
// with your copy/move operations.
class Book {
  std::string title, author;
  int length;

public:
  // has copy(move) ctor and copy(move) assignment operator implemented
};

class Text : public Book {
  string topic;

public:
  // does not define copy / move operations
};

Text t1{"Algorithms", "CLRS", 500, "CS"};
Text t2 = t1;
// this copy initialization calls Book's cctor and goes field-by-field
// i.e. default behaviour for the Text part. The same is true for other 
// compiler-provided operations. 

// If we were to write our own operations, they would look like this.
// Note: these are equivalent to the compiler provided ones.
// copy ctor
Text::Text(const Text& other) : Book{other}, topic{other.topic} { }
// copy assignment
Text& Text::operator=(const Text& other) {
  Book::operator=(other); // invoking superclass's copy assignment
  topic = other.topic; // copying text-specific fields
  return *this;
}
// move ctor
Text::Text(Text&& other) : 
  Book{std::move(other)}, topic{std::move(other.topic)} { 
    // the two std::move parts are stealing the data from other
}
// move assignment
Text& Text::operator=(Text&& other) {
  Book::operator=(std::move(other)); // invoking superclass's move assignment
  topic = std::move(other.topic); // move text-specific fields
  return *this;
} 
// Note: even though `other` points at an rvalue, `other` itself is an lvalue.
// So is other.topic. Therefore, we use std::move(x), which forces an lvalue x
// to be treated as an rvalue, so that the move operations run 
// instead of the copy operations. This is the general pattern for 
// sub-classes copy/move operations. Specialize as needed for other classes.

// An lvalue(thinking of it as a "locator value") is anything that:
// -- can appear on the left hand side of an assignment operation
// -- denotes a storage location
// -- has a name
// -- allows you to take its address using the address of operator &
// An rvalue is a temporary object that has none of these properties.



// ********** Preventing partial and mixed assignment **********
// Now consider:
Text t1{"Algorithms", "CLRS", 500, "CS"};
Text t2{"Algebra", "ABC", 200, "MATH"};
Book* pb1 = &t1;
Book* pb2 = &t2;
// What if we do:
*pb1 = *pb2;
// We expect this to be equivalent to t1 = t2;
// But it's not. In fact, `Book::operator=` runs. 
// The result is partial assignment : only the `Book` part is copied, 
// so t1 is only partially updated.

// Could we make `operator=` virtual?
class Book {
public:
  virtual Book& operator=(const Book& other); // added virtual
};
class Text : public Book {
  Text& operator=(const Text& other) override; 
  // won't compile because parameter type doesn't match

  Text& operator=(const Book& other) override; 
  // corrected type, make the parameter match
  // this will compile, but now this version would accept any kind of Book
  // on the RHS of an assignment, a Text object, but also a Book / Comic
};
Text t{"Algorithms", "CLRS", 500, "CS"};
// copying from a Book object -- BAD, get partial assignment
t = Book{"Programming for Beginners", "Niklaus Wirth", 200};
// copying from a Comic object -- REALLY BAD, what does it mean?
t = Comic{"Marvel", "DC", 30, "Superman"};
// This is called mixed assignment, which generally should not be allowed
// between objects that have different "shapes" or data fields.

// In summary:
// -- If `operator=` is non-virtual, we get partial assignment 
//    when assigning through base class pointers.
// -- If `operator=` is virtual, the compiler will allow mixed assignment.
// Both of them are BAD!!!
// Solution: Make all super-classes (non-leaf classes) abstract

// Rewriting our Book hierarchy
// Base Class      : AbstractBook
// Derived Classes : Book, Text, Comic
// We introduce a new superclass that has the common features of Book, Text, 
// and Comic, and we make it abstract. This eliminates the need to 
// allow assignment between Book objects (i.e. when Book was the base class, 
// it was also a concrete class.)

class AbstractBook {
  std::string title, author;
  int length;
protected:
  AbstractBook& operator=(const AbstractBook& other); // not virtual
public:
  AbstractBook( std::string, std::string, int );
  virtual ~AbstractBook() = 0; 
  // need at least one pure virtual method to make it abstract.
  // If you don't have one, use the dtor.
};
class Book : public AbstractBook {
public:
  Book& operator=(const Book& other) { // book-to-book assign, same as before
    AbstractBook::operator=(other);
    return *this;
  }
};
class Text : public AbstractBook {
public:
  Text& operator=(const Text& other) {
    AbstractBook::operator=(other);
    topic = other.topic;
    return *this;
  }
};
// comic class is similar

// The solution gives you everything you need:
// -- You can assign 'like' objects: b1 = b2; t1 = t2; c1 = c2;
// -- Partial and mixed assignments are prohibited: t1 = b1; b1 = t1; t1 = c1;
// -- Assignment through base class pointers is prohibited : *pb1 = *pb2,
//    avoiding unintentional partial and mixed assignment 
// -- Derived class assignment operators may call all assignment operators in
//    the base class. Base class assignment operators can only be called by
//    its sub-classes, but not the code outside.

// Important: When you have a pure virtual dtor, it must be implemented,
// even though it's pure virtual, or your program will not link. 
// Most pure virtual functions are not implemented, 
// but pure virtual dtors are a special case.
// The sub-class dtor will call the base class dtor, 
// so it must exist in the program.
