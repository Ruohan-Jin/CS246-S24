// Vec implementation file (vector-impl.cc)
module vec; // This file is part of module vec, can have multiple implementation files
// Don't need to import Vec, compiler implicitly imports the interface

Vec operator+(const Vec& v1, const Vec& v2) { // don't need export keyword, 
// not export Vec operator+
  return {v1.x + v2.x, v1.y + v2.y};
}

// You can have types, functions, etc. that are 
//   used internally by the module hidden from the uses of the module
int helper1(...) {...} 
// Not exposed to users of module vec. 
// Why? Because it's not exported.
