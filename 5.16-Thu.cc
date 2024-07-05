// Data Source: Input Device -> Device Driver -> Input Library
//                           |
//                           |
//                        Our Program
//                           |
//                           |
// Data Destinations:     Output Library -> Device Driver -> Output Device

// Our job as programmers:
// 1. Set up I/O streams to the appropriate data source and destination
// 2. Read from and write to those streams

// The C++ standard library provides the type 'istream' 
//   to deal with streams with input and the type 'ostream'
//   to deal with the streams with output.
// istreams: cin, ifstream, istringstream
// ostreams: cout, cerr, ofstream, ostringstream

// 'a', "hello", 123, (2,9) --> ostream --> buffer --> somewhere
    // turns 'values' of various types into byte sequences
    // sends them somewhere
// 'a', "hello", 123, (2,9) <-- istream <-- buffer <-- somewhere
    // gets bytes from somewhere
    // turns them into 'values' of various types
// istreams provide features for reading the kind of output provided by ostreams
// buffer: data structure ostreams use to store data 
//         while communicating with the operating system
// Generally, an istream provides features 
//   for reading kinds of output produces by ostreams

// ********** Overloading **********
// C: 
int negInt(int n) { 
  return -n;
}
bool negBool(bool b) { 
  return !b;
}
// C++: 
int neg(int n) { 
  return -n;
}
bool neg(bool b) { 
  return !b
};
neg(5); // the compile will look for function neg that takes int as a parameter
neg(false); // the compile will look for function neg that takes bool as a parameter
int x = 5;
neg(x);
bool x = false;
neg(x);
auto x = 5; // int x = 5, compiler can still figure it out
neg(5.0);   //double, not gonna work
neg("Hello");   //this is c-style string(char *), not gonna work


void f1(int x, int y = 13);
void f1(int x, string s = " ");
f1(5); // compile error, because compiler cannot determine which f1 to use

int f1(int x, int y);
bool f1(int x, int y);
// not valid, two declarations with same parameter types but different return types

// Overloading: functions have the same name, 
//   but must have different number of parameters and/or types of paramters.

void f(int a);
void f(int a, int b);
void f(string a);
// valid, all differs from each other

void f(int a);
void f(int b);
// not valid


// ********** User Defined Types **********
// built-in types: int, bool, float, double, char* / int*, char[]
// user-defined types(anything not a built-in type): 
//   vector(standard library types), 
//   string(standard library types), 
//   Node(our own user-defined types), 
//   Vector(our own user-defined types), 
//   Point(our own user-defined types)

struct Node {
	int data;
	Node *next;  // Node is incomplete if we omit the * and put Node next
}; // don’t forget this semicolon

struct Node {
	int data;
	Node *next;
} n1, n2; // define Node1 Node2 -> legal
Node n3; // works too

struct Vector {
	int x, y;
};
Vector v1;
Vector v2;

struct std::string {
	int length;
	char *text;
};
// conceptually, the std::string type could be this type of struct.


// ********** Constants **********
// symbolic names that you give to values

const int maxGrade = 100;
// constant cannot be changed, constant must be initialized
// 不initialize会有compiling error: uninitialized 'const maxGrade'

const int maxGrade;
maxGrade = 100;    // not valid

for (int i = 0; i < 10; i++) {
	// n is not defined
	const int n = i * 10 + random();  
  // n has a different value for each loop iteration, but that is ok.
	n = n + 1; // not valid because n is a constant, triggers a compile error
}

// Advice: declare as many things const as you can, helps catch errors

// Example using Node from earlier:
Node n1;
n1.data = 5;
n1.next = nullptr; // in C++, use nullptr instead of NULL in this course
Node n2{5, nullptr}; // {} are universal initialization syntax

// same:
int i = 0;
int i{0};
int i{};

const int i = 13;
const Node n3 = n1; // n3 gets a copy of n1, and n3 is immutable
// shallow copy, not a deep copy
