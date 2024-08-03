// ********** Protected Visibility **********
// If you want to give subclasses access to members 
//   but not any code outside the class hierarchy, 
//   you can use 'protected visibility'.
class Book {
  protected:  // Accessible to subclasses and sub-sub classes and so on, but no one else
    string title, author;
    int length;
};
class Text: public Book {
public:
  void addAuthor(const string& newAuthor) {
    author += ", " + newAuthor; // author field is visible to Text
  }
};

// If we want to further protect our base class fields, 
//   we can use a protected mutator method:
class Book {
  // fields are private
protected:
  void setAuthor(const string& newAuthor);  
  // subclasses can call this method to change the author
public:
  string getAuthor() const;  // accessor method
};


// ********** Virtual Methods **********
// Consider adding an `isHeavy()` method to Book. We'll define it as follows:
// - ordinary books: heavy means > 200 pages
// - textbooks: heavy means > 500 pages
// - comic books: heavy means > 30 pages
class Book {
  bool isHeavy() const {
    return length > 200;
  }
};
class Comic: public Book {
  bool isHeavy() const {
    return length > 30;  // assume we have access to length
  }
};
// textbook is similar

Book b{"A small book", "Papa Smurf", 50}; // not heavy
Comic c{"A Big Comic", "mr.comic writer", 40, "Antman"}; // heavy
cout << b.isHeavy() << c.isHeavy(); // false true
b = c; // no compiling error
c = b; // error: 
// no match for ‘operator=’ (operand types are ‘Comic@comic’ and ‘Book@book’)
Book b = Comic{"A Big Comic", "mr.comic writer", 40, "Antman"};
cout << b.isHeavy(); // what do we get? Book::isHeavy() runs
// WHY?
// The Comic object is 'sliced' 
//   - the hero field is chopped off and the comic is coerced into a Book.

// When accessing objects through pointers, slicing doesn't happen.
Comic c{"A Big Comic", "mr.comic writer", 40, "Antman"};
Comic* cp = &c;
Book* bp = &c;
Book& br = c;
cout << c.isHeavy() // true
cout << cp->isHeavy() // true
cout << bp->isHeavy() // false
cout << br.isHeavy() // false
// Our comic object is acting like a Book 
//   when we use a Book pointer or reference. 
//   The behavior of the object depends on 
//   what type of pointer (or reference) you access it through.

// Q: How do we make a comic act like a Comic when pointed to by a Book pointer? 
// A: use virtual methods.
class Book {
  string title, author;
protected:
  int length;
public:
  virtual bool isHeavy() const { // we add the virtual keyword
    return length > 200; 
  } 
};
class Comic: public Book {
  string topic;
public:
  bool isHeavy() const override { return length > 30; }
  // don't need to specify virtual at the beginning, 
  //   but override tells compiler that 
  //   we are explicitly overriding the method (optional keyword), 
  //   but the compiler will check that there is actually 
  //   such a virtual method in the base class. 
  //   If isn't, you get a compile error
};
// Now:
comic* cp = &c;
Book* bp = &c;
Book& br = c;
cout << c.isHeavy() // true
cout << cp->isHeavy() // true
cout << bp->isHeavy() // true!
cout << br.isHeavy() // true!

// Virtual methods - choose which class method to run 
//   based on the actual type of the object at run time.
Book b = c; // This is still slicing. 
            // We copy the comic fields into a Book object 
            // and run the Book version of isHeavy()
cout << b.isHeavy() // false

// Now we can have a collection of different types of `Book`s:
Book* myBooks[20]; // array of 20 Book*
for (int i = 0; i < 20; ++i) {
  cout << myBooks[i]->isHeavy() << endl;
}
// The correct version of isHeavy() will run for each one.
// Accommodating multiple types(Book, Text, Comic) under one abstraction(Book) 
//   is called polymorphism ("multiple forms").
// Note: this is why a function `void f(istream& i)` can be passed an `ifstream` 
//       - `ifstream` is a subclass of `istream`.


// ********** Destruction Revisited **********
class X {
  int* x;
public:
  X(int n) : x{new int[n]} { }  
  ~X() { delete [] x; }
};
class Y : public X {
  int* y;
public:
  Y(int m, int n) : X{n}, y{new int[m]} {}
  ~Y() { delete [] y; }
};
X* myX = new Y{10, 20};
delete myX;
// This code actually leaks memory! Why? 
//   Because `delete myX` calls `~X()` but not `~Y()`. 
//   So only `x` is freed, not `y`.

// Solution is to make the destructor virtual, 
//   similar to how we made `isHeavy()` virtual.
class X {
  int* x;
public:
  X(int n) : x{new int[n]} { }
  virtual ~X() { delete [] x; }
};
// Added `virtual` keyword. This causes the correct destructor 
//   to be called based on the 'actual' type of the object.

// Object destruction follows these steps:
// 1. Destructor body runs
// 2. Fields' destructors run in reverse declaration order
// 3. Repeat steps 1-3 for the base class (new! to support subclassing)
// 4. Space is deallocated
// Always makes the destructor virtual in classes that are meant to be subclassed.
virtual ~X() // This destructor doesn't do anything 
// but we create it anyway so we can make the destructor virtual

// If a class is not meant to be subclassed, you can declare it `final`.
class Y final : public X { // final make sure that Y cannot be subclassed
  // 意思是Y不能有其他的subclass
}; 


// ********** Pure Virtual methods and Abstract classes **********
class Student {
protected:
  int numCourses;
public:
  virtual int fees() const = 0;  
  // = 0 signals that the fees() method has no implementation. 
  // fees is called a pure virtual method.
};
class Regular : public Student {
public:
  int fees() const override; // computes fees for regular students
};
class Coop : public Student {
public:
  int fees() const override;
};

// A class with one or more pure virtual methods cannot be instantiated, 
//   which is called an abstract class.
Student s; // ERROR! because Student cannot be instantiated
Regular r;
Coop c;
// subclasses of an abstract class are also abstract unless 
//   they implement 'all' pure virtual methods.

// An abstract class is generally used to provide 
//   a common set of fields and/or methods for a series of subclasses.
// Non-abstract classes are called concrete classes.
// In UML, we represent virtual and pure virtual methods using italics. 
//   Represent abstract classes by italicizing the class name. 
//   So can also use asterisks to show this when italics are hard to do.

// 20-purevit 
// big five associated with inheritance
