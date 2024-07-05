// ********** Parameter Passing **********

// pass by value
void inc(int n) { 
	n = n + 1; 
}
int x{5};
inc(x); // function gets a copy of x
cout << x << endl; // 5

// pass by pointer(a special kind of pass by value)
void inc(int *np) {
	(*np)++;
	// *np = *np + 1; // alternative
}
int x{5};
inc(&x);
cout << x << endl; // 6

// reference
int y = 10;
int &z = y; // z is a reference to y
z = 12; // now y = z = 12

// error: reference must be initialized
int &z;  
z = y;

int x = 5;
z = x; // z is like a constant, cannot change what z is an alias of
       // what actually happens here is that y(aka z) is assigned the value of x

int ints[5] = {1, 2, 3, 4, 5};
int& i2 = ints[2];
i2 = 30; // i2 = ints[2] = 30, array: {1, 2, 30, 4, 5}

// pass by reference
void inc(int &n) {
	n = n + 1; // n is bound to x at runtime so that n is an alias of x
}
int x{5};
inc(x);
cout << x << endl; // 6

// each call to inc causes a different variable to be bound to the n parameter
int x = 5; inc(x); // x = 6    
int y = 10; inc(y); // y = 11
int z = 15; inc(z); // z = 16

cin >> x; // >> is a read function that takes x by reference 
          // and can therefore change x

int* p1 = &x; // p1 is a pointer to x;
int& y = x; // y is a reference to x;

// Reference are like const pointers with automatic dereferencing.
// What can’t you do with references:
// 1. Leave them unintialized: 
int& z; // not ok   
int& z = x + y; // not ok
// 2. Create a pointer to a reference:    
int&* x; // would mean x is a pointer to an int reference
// 3. Create a reference to a reference:   
int&& x; // not ok
// 4. Create a array of references: 
int& r[3] = {x, y, z}; // not ok
// What can we do with references:
// Use them as function parameters: 
void inc(int& n);

// Passing parameters to functions using references
struct ReallyBig{/*...*/};
int f(ReallyBig rb) {} // copy of parameter, potentially slow
int g(ReallyBig& rb) {} // call by reference, no copying happens. 
// Fast! BUT!!! Function g can change rb in the caller
int h(const ReallyBig& rb) {} // call by reference, Fast!
// but h cannot change rb because it is const! 
// 'const' guarantes to the caller that rb will not be changed, 
//   compiler will ensure that h does not change rb.
// Advice: when passing data to functions, prefer pass-by-const-ref 
//   for anything larger than a pointer or int, 
//   unless the function needs to make a copy anyway.

int f(int& n);
f(5); // not ok because n could potentially be used to update the literal 5.
int g(const int& n);
g(5); // ok because compiler guarantees the literal 5 will not be changed

cin >> x;
cout << x; // ostream& operator<<(ostream&, const int&)
cout << 5;


// ********** Dynamic Memory Allocation ********** 
// physical memory model
// 1. code: programs that are loaded and executing
// 2. static data: literals, global vars
// 3. free store(heap): memory for 'new' operator
// 4. stack: function arguments, local storage

// allocate array of 10 ints on the heap
int* p = (int*) malloc(10 * sizeof(int));    
free(p);

// Don’t use malloc and free in C++. Use new and delete instead
Node *np = new Node;  
// new allocates memory from the heap, returns a pointer to the object. 
// new is type safe, returns a pointer of the right type
delete np; // delete memory from the heap, similar to calling free

int* ip = nullptr;
delete ip; // harmless because ip is the null pointer

Node* nodeArray = new Node[10]; // creates an array of 10 Node objects
delete [] nodeArray; // this tells operator that nodeArray is an array. 
// Note we don’t have to tell delete how many elements are in our array!

// Memory allocated with new must be deallocated with delete.
// Memory allocated with new[] must be deallocated with delete[].
// Mixing these is undefined behavior. Don’t do it.
