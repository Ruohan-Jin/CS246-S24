// ********** Copy Constructors **********

// Every class comes with a default copy constructor
// the copy constructor constructs one object as a copy of another

// writing our own copy constructor for Student: 
struct Student{
	int assns, mt, final;
	student(const Student& other): // general pattern for a cctor
    assns{other.assns}, mt{other.mt}, final{other.final} {}
		// this copy is equivalent to the built-in one.
}; 
Student s1{60, 70, 80};
Student s2 = s1; // invokes copy constructor implicitly

// Q: Why would I need to write a copy constructor 
//      if the compiler always provides a default one? 
//    When is the built-in copy constructor not correct? 
// A:  Built-in copy constructor does a shallow copy. 
//     If you want a deep copy, you must implement your own copy constructor. 
//     The copy constructor we write then replaces the built-in one.
//     You want a copy to be independent and not share any data with the original

struct Node {
  int data;
  Node *next;
};
Node* n = new Node{1, new Node{2, new Node{3, nullptr}}};
Node m = *n; // cctor implicitly
Node* p = new Node{*n}; // cctor explicitly

Node(const Node& other) : 
  data{other.data}, 
  next{other.next == nullptr ? nullptr : new Node{*other.next}} {
  // some code
}
// new Node{*other.next} : recursively copies the rest of the list

// Aside:
  // ternary operator: ?:
  // example:
  count == 1 ? "coin" : "coins"
  if count==1 then "coin" else "coins"

// The cctor is called implicitly:
//   1. An object is initialized with another object of same type
//   2. An object is passed by value to a function.
//   3. An object is returned by value from a function. The truth is somewhat nuanced.

// Q: Why is this version of cctor wrong?
Node(const Node other) : ... { } // passing 'other' by value, 
                                 // which ends up calling the cctor recursively
// when calling cctor, always have const & for the parameter


// Constructors that take just one parameter:
struct Node {
  ...
  Node(int data) : data{data}, next{nullptr} {}
  Node(int data, Node *next = nullptr) : data{data}, next{next} {}
}
Node n = {1}; // implicit conversion from int to Node
Node m = {1, nullptr};
Node p = {1, new Node{2}};
Node n2 = {4, nullptr}; // this would not invoke the single-arg ctor 
                        // because there are 2 parameters
// Problem with single-arg ctors is they create implicit conversions. 
//   Why is this a problem?
int f(Node n);
f(4); // works! 4 implicitly converted to Node using the 1-arg Node ctor.

// Danger: accidentally passing an int to a function that expects a Node.
//   -- silent conversion
//   -- compiler does not signal an error
//   -- potential errors not caught
// Don't do things that limit your compiler's ability to help you.
// Solution: Disable the implicit function. 
//           make the ctor explicit by using the explicit keyword.
struct Node {
  explicit Node(int data, Node *next=nullptr) : data{data}, next{next} {}
};
// After adding explicit:
Node n{4}; // ok!
Node n = 4; // Error!
f(4); // Error!
f(Node{4}); // ok!
// Good practice: when you have a single-arg ctor, 
//                label it with explicit to avoid implicit conversions.


// ********** Destructors **********
// When an object is destroyed 
//   (stack-allocated: go out of scope; heap-allocated: is deleted), 
//   a method called the destructor(dtor) runs.
// Classes always come with a default destructor, 
//   which invokes the dtors of all fields that are objects.

// When an object is destroyed, 3 steps:
//   1. dtor body runs
//   2. field's destructors run in reverse declaration order 
//      (for fields that are objects)
//   3. space deallocated

// When do we need to write our own destructor?
Node *np = new Node{1, new Node{2, new Node{3, nullptr}}};
// If np goes out of scope, the pointer is reclaimed, list is leaked.
// If we say delete np, the first Node is reclaimed 
//   (*np's destructor is called), which doesn't do anything
// Solution: write our own destructor
struct Node {
  ...
  ~Node() {
    delete next; 
  }
};
// The Node class is responsible for freeing memory. We use a dtor to do that.
// Note: A class has only one dtor method. Cannot write multiple dtors in one class.
// The job of the destructor is to clean up resources(memory, files, etc.)

// C function exit(1);
// -- exit() terminates your program. None of the destructors run when you use exit();
// -- not deallocating all memory allocated 
//    is considered an incorrect program in this course
