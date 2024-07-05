// ********** Object Creation Steps **********
// When an object is created, 3 steps:
// 1. Space is allocated
// 2. Fields are constructed in declaration order
//    ctors run for fields that are objects
// 3. ctor body runs

// ********** Member Initialization List(MIL) **********
Student::Student(int assns, int mt, int final) : assns{assns}, mt{mt}, final{final} { }
// MIL         : assns{assns}, mt{mt}, final{final}
// fields      前面的assns, mt, final
// parameters  {}里面的assns, mt, final

// MIL runs during step 2
// MIL must be part of the definition of the constructor. 
//   It cannot be in the declaration.
Student(...); // declaration in class definition
Student::Student(...): /*MIL*/ { } // definition

// General syntax of a MIL:
Student::Student(int assns, int mt, int final) : field{ctor arguments}, field{}, ... {}
// :                        ------    introduces the MIL
// field{ctor arguments}    ------    like calling a ctor runs in step 2
// { }(最后的括号)            ------    ctor body runs in step 3

Basis::Basis() : v1{0, 1}, v2{1, 0} {
  // some code
}
// MIL          : v1{0, 1}, v2{1, 0}            runs in step2
//              {//some code}                   runs in step3

Basis::Basis(const Vec& v1, const Vec& v2): v1{v1}, v2{v2} {
  // 前面的v1, v2         refer to the fields of Basis
  // {}里面的v1, v2       refer to the parameters
}

// Default values in the struct:
struct Basis {
  Vec v1 {1, 0}; // If a MIL doesn't mention a field, 
  Vec v2 {0, 1}; // these values are used
  Basis() { // uses default values above
    // some code
  }
  Basis(const Vec& v1, const Vec& v2): v1{v1}, v2{v2} {
    // some code
  }
};
Basis b; // use default values
Basis b{Vec{5, 6}, Vec{-7, 4}}; // uses the parameters, 
                                // calls this ctor(上面Basis(const...))的

// with a MIL, the fields are initialized in declaration order, 
//   not in the order provided in the MIL
Basis::Basis(const Vec& v1, const Vec& v2): v2{v2}, v1{v1} {}

struct Student {
  int assns, int mt, int final; // not objects
  string name; // object

  Student(int a, int m, int f, const string& n) {
    assns = a;
    mt = m;
    final = f;
    name = n; // runs during step3, but it's actually overwriting 
              // the default empty string that was stored in name during step2
              // Reassignment.
  }

  Student(int a, int m, int f, const string& n): assns{a}, mt{m}, final{f}, name{n} {
    // name is initialized from n during step2
    // This is more efficient! no reassignment in step3
  } 
};

// MIL must be used :
//   -- for fields that are objects with no default ctor
//   -- for fields that are const or references, must be initialized in step2
// embrace the MIL! use it as much as possible

// can even use fields in MIL that were initialized earlier in MIL
// Ex.      
... : x{rand()}, y{x}
// y is initialized to the value of field x, which is a random number

