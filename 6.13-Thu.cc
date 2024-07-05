// ********** Copy / Move Elision **********

Vec makeAVec() { // return by value
  return {0,0}; // uses default ctor to create Vec
}
Vec v = makeAVec(); // copy or move ctor depending on if we have a move ctor

// In certain cases, the compiler is required to 
//   skip calling the copy/move ctors.
// Here, 'makeAVec()' writes its result directly 
//   into the space occupied by 'v' in the caller, 
//   rather than copying it later. 
void doSomething(Vec v) { 
  // some code
}
doSomething(makeAVec()); // result of makeAVec is written directly 
                         // into the parameter v of doSomething().
                         // No copy or move.
// Elision happens even if dropping the ctor calls 
//   would change the behaviour of the program 
//   (e.g., if the ctors printed something). 
//   You are not expected to know 'when' it happens, just that it can happen.


// ********** Member Operators **********
// Notice 'operator=' was a member function and not a standalone function.
// Previous operators we have written were standalone functions.

struct Vec {
  int x, y;
};
Vec operator+(const Vec& v1, const Vec& v2) { // standalone function
  // doesn't have access to 'this'
}

// writing it as a member fn:
struct Vec {
  int x, y;
  Vec operator+(const Vec& other) { // member fn
    return {x + other.x, y + other.y};
  }
  Vec operator*(const int k) { // implements the v * k function
    return {x * k, y * k};
  }
};
Vec v{5, 4};
Vec v2 = v + v;
Vec v3 = v * 10;
Vec v4 = 10 * v;  // Does not match the function signature above because  
                  // it has an int on the left and vector on the right, 
                  // while *operator has the opposite.
                  // It cannot be a member function. It must be standalone.
// standalone:                  
Vec operator*(int k, const Vec& v2) { // allows us to multiply an integer by a vector
  // some code
}

// standalone version but could be written as a member fn
Vec& operator+=(Vec& v1, const Vec& v2) { 
  v1.x += v2.x;  // int +=operator (regular) Not recursion
  v1.y += v2.y;
  return v1;
}
int i;
i += 10;
Vec v{5, 4};
Vec v2{10, 10};
v += v2;

// Now we can write the regular operator+ version in terms of the += operator
Vec operator+(const Vec& v1, const Vec& v2) {
  Vec temp{v1}; // use copy ctor
  return temp += v2; // use above operator+= to modify temp and then return it
}

// Question:
struct Vec{
  int x, y;

  ostream& operator<<(ostream& out) { // what is wrong with this?
    out << x << ',' << y;
    return out;
  }
};
v << cout; // we would have to write it like this. Not "standard"
// So define operator<< and operator>> as standalone.

// Also note that some operators must be member:
// operator=
// operator[]
// operator->
// operator()
Vec::operator=(...) // if member function defined outside the struct


// ********** Array of Objects **********

// let's say Vec is defined as
struct Vec {
  int x, y;
  Vec(int x, int y): x{x}, y{y} {} // ctor replaces the default no-arg ctor.
};
Vec v; // won't work
Vec v{5, 4}; // will work
// ERROR:
Vec vectors[15];       // These lines of code don't work because 
Vec* vp = new Vec[15]; // the compiler will try to default constructor each vector.

// Options:
// 1. Create a default ctor for Vec.
//    create a new one or make x and y parameters have default values.
// 2. For stack arrays: 
Vec vectors[15] = {Vec{0, 0}, Vec{1, 1}, ... , Vec{14, 14}}; 
// need 15 of these
// 3. For heap arrays: create an array of pointers.
Vec** vp = new Vec*[15];
for (int i = 0; i < 15; ++i) {
  vp[i] = new Vec{0, 0};
}
// free 
for (int i = 0; i < 15; ++i) {
  delete vp[i];
  delete [] vp;
}

// BTW, you can initialize arrays of built-in types easily:
Vec** vp = new Vec*[15] = {nullptr}; 
// initializes list will be used to initialize the array elements. 
// If the list is too short (which in this case), 
// the compiler initializes the rest with 0. 
// Use this technique instead of a loop.


// ********** Const Objects **********

struct Student {
  int assns, mt, final;

  float grade() {
    return assns * 0.4 + mt * 0.2 + final * 0.4;
  }
};

int f(const Student& s) { // const objects arise often, especially as parameters.
                          // const objects cannot be modified. 
                          // The compiler guarantees this.

  s.grade(); // no danger of modifying s. // BUT compiler doesn't know that 
             // grade() doesn't modify the Student object. 
             // Thus, this won't compile!!
}

// solution:
struct Student {
  int assns, mt, final;

  float grade() const {...} // the const keyword guarantees 
                            // grade() will not modify the object.
};

float Student::grade() const { // you need to write const keyword 
                               // in the declaration and definition
}

// The compiler checks that const method don't modify fields. 
//   grade() is not allowed to modify assns, mt, final.

// If you have a const object, like s above, 
//   you can only call const methods on it. 
//   Only const methods may be called on const objects. 
//   But if you have a non-const object, 
//   you can either call const or non-const methods on it.
