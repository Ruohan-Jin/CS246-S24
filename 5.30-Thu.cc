// dependency graph for 05-separate/example3
// 指向vector.cc: vector-impl.cc, main.cc, linearAlg.cc
// 指向linearAlg.cc: linearAlg.cc, main.cc

// A module itself can import the modules it needs for its own implementation

// We provide a bash script called 'compile' in the tools directory 
//   if you want to partially automate compilation

// Benefits of modules over Header Files
// 1.	Faster compilation. 
//    The module interface file is compiled only once and used many times.
// 2.	If a module A imports module B to use as part of its implementation, 
//    the module B content are 'not exposed to' users of module A.
// 3.	Modules can be imported in any order.
// 4.	Non-exported implementation details (like helper functions) 
//    aren’t visible outside the module.
// 5.	You can use modules and head files together. 
//    Helps when incrementally upgrading a large system to use modules, 
//    can do parts of the system separately over time.

// In this course, stick to one or the other 
//   (usually the assignment tells you which one to use).


// ********** Classes **********

// can put functions inside structs -> generally call these classes

// student.cc
export module student;

export struct Student {
	int assns, mt, final;
	float grade();
};


// student-impl.cc               
module student;

float Student::grade() { // 'Student::' tells compiler that 
                         // this is the function grade inside the student class
                         // '::' is the scope resolution operator
                         // 'c::f' means f in the context of class C.
	return assns * 0.4 + mt * 0.2 + final * 0.4; 
  // 'assns, mt, final' these are fields of the receiver object 
  // - the object on which grade() is called

  // this function actually has a hidden parameter called 'this' 
  //   - pointer (not a reference) to the receiver object
	return this->assns * 0.4 + this->mt * 0.2 + this->final * 0.4; 
  // equivalent to before, compiler inserts 'this->' automatically so we don’t have to
}


// client
import student;
Student s{60, 70, 80}; 
std::cout << s.grade() << std::endl; 
// s is the receiver object, uses the values in object s
// 'class' is essentially a structure type that can contain functions.
// 'object' is an instance of a class.
// Student - class, s - instance / object, {60, 70, 80} - initialization list
// grade() is called a 'method' or 'member function'


Struct Student {
	int assns, mt, final;
	float grade() {
		return assns * 0.4 + mt * 0.2 + final * 0.4;
	}
};
// when the function body in coded directly in the struct definition like this, 
//   the code ends up being inlined whenever grade is used. 
//   Because of this, don’t code your methods in the struct.


Student s;
s.this->assns; // Not valid syntax. 'this' is only available inside the method body.
Student *p = &s; // &s == this inside the method body


// ********** Initializing Objects **********
Student s{60, 70, 80}; // assns = 60, mt = 70, final = 80
			                 // uses the order of the fileds in Student
// Better way to initialize objects is to use 'constructor(ctor)'

struct Student {
	int assns, mt, final;
	float grade();
	Student(int assns, int mt, int final); // ctor
};

// ctor:
// 1. same name as class
// 2. can be defined with 0 or more parameters 
//    that can be used to initialize new objects
// 3. no return type, not even void
Student::Student(int assns, int mt, int final) {
  this->assns = assns;
  this->mt = mt;
  this->final = final;
  // 'this->' is needed to tell compiler which assns we're referring to 
}
// Don't need to use this -> if names are unambiguous
// If parameters are name a m f, we can just do:
Student::Student(int a, int m, int f) {
  assns = a;
  mt = m;
  final = f;
}

// Equivalent:
Student s{60, 70, 80} // causes ctor to run. 
                      // The values are passed to the ctor function.
Student s = Student{60, 70, 80}; // older syntax not recommended
Student s = {60, 70, 80};

Student* sp = new Student{60, 70, 80}; // allocates a student object on the heap
delete sp; // free memory

// Why write constructors? What are the advantages?
// -	They are functions
// -	Write arbitrary complex initialization code
// -	Default parameters
// -	Overloading
// -	Validation, sanity checks
// -	Ensures our objects are initialized properly 
//      and that the object is logically valid

// Default values example:
Student::Student(int assns = 0, int mt = 0, int final = 0) {
  this->assns = assns;
  this->mt = mt;
  this->final = final;
}
Student s2{60, 70}; // final gets default value 0
Student s3;         // all 3 fields get default values of 0
Student s3{};       // same as previous

// What if you don't write a constructor?
// -- every class comes with a default constructor
//    default-constructs all fields that are objects
// -- if you write any constructor, the default ctor goes away

Student Vec{
  int x, y;
  Vec(int x, int y) {
    this->x = x;
    this->y = y;
  }
};
Vec v; // error!
Vec v{1, 2}; // ok

struct Basis {
  Vec v1, v2;
};
Basis b; // this won't compile, because Vec doesn't have a ctor that takes no params

struct Basis {
  Vec v1, v2; // error
  Basis() {
    v1 = Vec(0, 1);
    v2 = Vec(1, 0);
  }
}
Basis b; // this also doesn't compile

// Object Creation Steps
// When an object is created, 3 steps:
// 1. Space is allocated
// 2. Fields are constructed in declaration order
//    ctors run for fields that are objects
// 3. ctor body runs

// initialization of v1 and v2 must happen in step 2. How can we accomplish this?
