// ********** Copy Assignment Operator **********

// Not a constructor, overrides the = assignment operator
Student s1{60, 70, 80}; // ctor for Student
Student s2 = s1; // cctor for Student
Student s3; // Default ctor
s3 = s1; // copy, not constructing the object, it's actually doing a copy assignment
// copy construction starts with an uninitialized object
// copy assignment starts with a fully constructed object from earlier

// The goal of copy assignment is to replace the data in the object 
//   with a copy of the data from the other object, without introducing memory leaks.

// The compiler provides a default copy assignment operator that we can replace.

struct Node {
	int data;
	Node* next;
	
	Node& operator=(const Node& other) {
		data = other.data;
		next = other.next? new Node(*other.next): nullptr;  // memory leak!
		return this;
	}
}; 
// new Node(*other.next) calls cctor

struct Node {
	int data;
	Node *next;
	
	Node& operator=(const Node& other) {
		data = other.data;
		delete next; // frees any existing node list we are pointing to
		next = other.next? new Node(*other.next): nullptr;  
		return this;
	}
};

Node n{0, nullptr};
n = n;   // self-assignment

struct Node {
	int data;
	Node *next;
	
	Node& operator=(const Node& other) {
		if (this == &other) {
			return *this; // protects self assignment
		}
		data = other.data;
		delete next; 
		next = other.next? new Node(*other.next): nullptr;  
		return this;
	}
};

// Self assignment examples:
*p = *q;
a[i] = a[j];

// Q: Would this work?:
if (*this == other) {
  return *this;
}
// No, because 
// 1. you could have two Nodes with the same data that would be equal, 
//    but not be self-assignment
// 2.	you don’t get == for free, you have to code it.

// Better implementation is (no problems):
struct Node {
  int data;
  Node* next;

  Node& operator=(const Node& other) {
    if (this == &other) {
      return *this;
    }
    Node* temp = other.next? new Node{*other.next}: nullptr;
    data = other.data;
    delete next;
    next = temp;
    return *this;
  }
};
// This version ensures that if 'new' fails, we don’t change our object. 
// Because if 'new' fails, we exit the function immediately.

// Alternative version: copy-and-swap idiom
import <utility>;

struct Node {
	int data;
  Node* next;

	void swap(Node& other) {
		std::swap(data, other.data);
		std::swap(next, other.next);
	}
	Node& operator=(const Node& other) {
		Node temp = other; // copy via copy ctor (deep copy)
		swap(temp);	// now temp has old values
		return *this;
	}
};
// After step 2, temp has the old values from our object. 
// When operator= function exits, temp will be destroyed 
//   and taking our old data with it.
// With copy-and-swap, the self-assignment test is not needed, 
//   but it would be an optimization.

// Why do we return *this? It allows us to do cascading assignment:
n1 = n2 = n3 = n4 = n5; // makes n1…n4 the same as n5.


// ********** Move Constructor and Move Assignment Operator **********

// Consider:
Node oddsOrEvens() { // returns a Node object or values
	Node odds{1, new Node{3, new Node{5, nullptr}}};
	Node evens{2, new Node{4, new Node{6, nullptr}}};
	char c;
	cin >> c;
	return c == '0' ? evens: odds;
}
// copy constructor / copy assignment operator are used to 
//   copy the data from odds or evens into n or m.
Node n = oddsOrEvens(); // copy constructor
Node m;
m = oddsOrEvens(); // copy assignment operator

// we can actually steal the data from the temporary odds/evens objects 
//   instead of copying it by move constructor and move assignment operator
struct Node {
  int data;
  Node* next;
	Node(Node&& other): data{other.data}, next{other.next} {
    // &&: rvalue reference to a temporary object
		other.next = nullptr;
	}
};
// The temporary Node other will still have its destructor called, 
//   but since it’s next field is now nullptr, the destructor doesn’t free anything. 
//   The temp object can be simply and efficiently. No copying of nodes was needed.
// Move constructor runs in constant time.
//   It also requires less memory because we don’t have to copy all the nodes.


// Move Assignment operator:
struct Node {
	int data;
  Node* next;

	Node& operator=(Node&& other) {
		swap(other); 
    // same swap we used for the copy assignment oprerator 
    //   with the copy and swap idiom.
		return *this;
	}
};
// When the temporary other Node is destroyed, it will take our old data with it.

// How is move different from copy?
// 1. Other parameter is NOT const because we need to steal from it 
//    and make it “empty”. Or we can swap our data into it (for assignment).
// 2. Other is an rvalue reference type parameter.
// 3. It is simpler to write a move operator than a copy operator.
// 4.	Moves are like shallow copies – little memory overhead.

// If you don’t define a move constructor or move assignment operator, 
//   the compiler will use the copy operations when the argument is a temporary object.

Node n = oddsOrEvens(); // Now this uses the move constructor. 
Node m;
m = oddsOrEvens(); // Now this uses the move assignment operator.
// We didn’t have to change the function or this code.

// Bottom line, returning objects by value from a function 
//   is often very fast and efficient.

// In summary, we can now state the Rule of Five (or the Big 5):
// If you need to write any one of the following, you usually need to write all five.
// -	Destructor
// -	Copy constructor
// -	Copy assignment operator
// -	Move constructor
// -	Move assignment operator
// But note that many classes don’t need any of these. 
//   The default implementation are fine.
// When do you need then?
//    Ownership: when the class owns resources (memory, file handles, etc.) 
//    that need to be managed.
