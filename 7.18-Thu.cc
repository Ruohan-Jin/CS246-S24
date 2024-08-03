// ********** 1. Casting **********

// Recall C-style casting:
double d  = 3.14;
int x = (int) d;

Node n;
int* np = (int*) &n; // Force C++ to treat a Node* as an int*

// Casting should be avoided, in particular, C-style castings should be avoided
// in C++. If you must cast, C++ provides casting operations.


// ***** static_cast *****
// this is for sensible casts that have well-defined semantics
// If try to do a static_cast but it's not a valid conversion, get compile error

// Ex. double to int
f(int x);
f(double d);
int main () {
  double d;
  f(static_cast<int>(d)); // call int-version of f, decimal gets truncated
}

// Ex. super-class to sub-class pointers
Book* b = new Text{};
Text* t = static_cast<Text*>(b); // target type is in <>
// You're taking responsiblity that b actually points to a `Text` object.
// You're telling the compiler "trust me".

// will invoke a conversion ctor if one exists (recall this is a one-arg ctor)
struct Vec {
  int x, y;
  explicit Vec(int n) : x{n}, y{n} { }
}; 
Vec v = static_cast<Vec>(5); // invokes the int to Vec conversion ctor above


// ***** reinterpret_cast *****
// Unsafe, implementation dependent, weird conversions.
// Most uses of reinterpret_cast result in undefined behaviour.
// Tell the compiler to treat the expression as the indicated type.
// No code (cpu isntructions) are generated and nothing happens at run time.

// Ex.Force a Student to be treated as a Turtle
Student s;
Turtle* t = reinterpret_cast<Turtle*>(&s);


// ***** const_cast *****
// convert between const and non-const
// this is the only C++ cast that can cast away constness
// if you have to do this, why would you make it const in the first place

// difference between `const_cast` and `mutable`:
// -- `mutable` is only for class member fields
void g(int* p);
void f(const int* p) {
  g(p); // will not work
  g(const_cast<int*>(p)); // ok if we're certain that g does not modify the int,
                          // but why not just also make g take a const int*
}


// ***** dynamic_cast *****
// safely converts pointers and references to object 
// up and down for the inheritance hierarchy

Book* bp = /*some code*/;
Text* tp = static_cast<Text*>(bp); // safe to do this? Not sure. 
// Depends on what bp is pointing at. 
// If not a `Text`, we get undefined behaviour when accessing `Text` members.

// It would be better to do a tentative cast first: try it and see if succeeds.
Text* tp = dynamic_cast<Text*>(bp); // attempt cast, return nullptr is invalid
if (tp) {
  std::cout << tp->getTopic();
} else {
  std::cout << "Not a Text";
}

// Note: dynamic_cast only works on classes with at least one virtual method
// 将A* dynamic_cast为B*的要求：A必须有virtual method


// Previous examples used raw pointers but 
// we can also cast smart pointers (unique_ptr, shared_ptr) : 
// static_pointer_cast, reinterpret_pointer_cast, 
// const_pointer_cast, dynamic_pointer_cast
// Stay within the type: cast shared_ptrs to shared_ptrs


// dynamic_cast also works as references:
Text t{/*some code*/};
Book& b = t;
Text& t2 = dynamic_cast<Text&>(b);
// If b points to a Text, then t2 is a reference to some Text.
// If not, a std::bad_cast exception is thrown.



// ********** 2. Static Fields and Methods **********
// static members (fields and methods in a class) are associated with class 
// itself, and not with any particular instance (object) of the class

struct Student {
  inline static int numInstances = 0; 
  // `inline` lets you initialize it inline in your class

  Student(/*some code*/) : /*some code*/ {
    ++numInstances;
  }

  // static member functions / methods don't depend on a specific instance.
  // Can only call static variables / methods
  // For their computation, don't have a `this`.
  static void howMany() {
    std::cout << numInstances << std::endl;
  }
};

int main() {
  Student s1, s2;
  std::cout << Student::numInstances << std::endl;
  Student::howMany(); // 2
}

// without `inline`, you would need to write
struct Student {
  static int numInstances;
};
int Student::numInstances = 0;



// ********** Factory Method Pattern **********
// Problem : we want to write a video game with two kinds of enemies, turtles
// and bullets. The system sends turtles and bullets to us, but bullets become
// more frequent in later levels.

// abstract base classes, concrete sub-classes
// Base Class :      Enemy
// Derived Classes : Turtle, Bullet
// Base Class :      Level
// Derived Classes : Easy, Hard

// We don't want to hard-code the enemy generation policy. 
// It should be flexible at run-time.
// We will create a Factory method in `Level` that creates enemies.
class Level {
public:
  virtual Enemy* createEnemy() = 0; // Factory Method
};

// We can have different policies by creating sub-classes:
class Easy : public Level {
public:
  Enemy* createEnemy() override; // create mostly turtles
};

class Hard : public Level {
public:
  Enemy* createEnemy() override; // create mostly bullets
};

// switch policies at run-time:
Level* easyLevel = new Easy;
Level* hardLevel = new Hard;

// start easy
Level* currentLevel = easyLevel;

while (true) {
  if (/*some code*/) {
    currentLevel = hardLevel; // at some point we switch
  }
  // call factory method to create an enemy
  Enemy* enemy = currentLevel->createEnemy(); 
}

// Definition : The Factory Method Pattern defines an interface for creating
//   an object, but lets sub-classes decide which class to instantiate. 
//   Factory Method lets a class defer instantiation to sub-classes.

// This pattern is also knowns as the virtual constructor pattern.
// Important Points:
// -- Factory Method relies on inheritance : object creation is delegated to 
//    sub-classes, which implement the factory method to create objects.
// -- This pattern promotes loose coupling by reducing the dependency of your
//    application on concrete classes. Your application only depends on the 
//    abstract interface, not on specific concrete classes.

// Another example of the OO Guiding Principle of 
// "Program to interfaces, not implementations" : 
//   Abstract base classes define the interface. Your client code should be 
//   written to use base class pointers and call the methods in the base class
//   interface. The interface is the abstraction. Concrete sub-classes can be 
//   swapped in and out to provide a variety of behaviours for this abstraction. 
//   Promotes loose coupling.
