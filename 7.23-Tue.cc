// ********** Exception Safety Guarantees **********
// This means : if program execution leaves a function because of an exception,
//              that the program is not left in a broken or unusable state.

// Specifically, the levels of exception safety for a function f (in decreasing
//   order of safety) are :
// 1. no-throw guarantee : f promises to never throw or propagate an exception
//    and will always complete successfully. If an exception occurs internally,
//    it will be handed internally and won't be observed by clients. If f can't
//    fully accomplish its task, it may indicate failure some other way 
//    (such as setting an internal flag or returning a result code).
// 2. strong guarantee : if f throws or propagates an exception, the state of 
//    the program will be as if f had not been called. If f changes some state,
//    it must 'undo' those changes to restore the original state 'before'
//    throwing an exception. In other words, from the outside it must appear
//    that either : 
//    -- f succeeded in doing everything it was asked to do (normal exit)
//    OR
//    -- nothing happened except an exception was thrown to indicate an error
// 3. basic guarantee : if f throws or propagates an exception, the program 
//    will be in some valid (maybe unspecified) state. No resources are leaked,
//    no corrupted data structures, and all class invariants are intact.
// 4. no exception satefy : f makes none of the above guarantees

// Examples of no-throw guarantee:
// 1. pointer assignment never throws. 
node->next = other.next; // Ex.
// 2. delete(to delete an object) never throws, because dtors should never throw      
// 3. 
int i;
std::cin >> i; // if not successful, sets the fail() flag.
// 4. Lots of built-in facilities in C++ provide this guarantee, they never throws.

// Examples of strong guarantee:
class A {
public:
  void g(); // offers the strong guarantee. may throw.
};
class B {
public:
  void h(); // offers the strong guarantee. may throw.
};
class C {
  A a;
  B b;
public:
  void f() {
    a.g();
    b.h();
  }
};
// What safety level does f offers? Let's analyze f :
// 1. If `a.g()` throws, any changes it made are "undone" and the f propagates
//    the exception. At this point we have the strong guarantee.
// 2. What if `b.h()` throws, the effects of `a.g()` must be 'undone' to have f
//    offer the strong guarantee. Very hard or impossible if `a.g()` has 
//    non-local side effect. 
//    If `A::g()` or `B::h()` only have local side-effects (they only change 
//    themselves), we can rewrite f to offer the srong guarantee. One way to do
//    this is to use a copy-and-swap idiom.
void f() {
  A tempa = a; // create a copy of a and b that we will operator on
  B tempb = b;

  tempa.g(); // If these throw, the original `a` and `b` are still intact
  tempb.h();

  a = tempa; // swap our `a` and `b` with the modified ones
  b = tempb;
}
// mostly corret. the last line is a problem. if it throws, we've partially 
// updated our state (`a` has been updated but not `b`)
// It woule be better if the last two lines were guaranteed to never throw.
// Solution : recall that assigning pointers can never throw. Access C's 
//   internal state through a pointer - called pImpl idiom 
//   (pointer to implementation) :
struct CImpl {
  A a;
  B b;
};  
class C {
  unique_ptr<CImpl> pImpl{new CImpl}; 
  // construct CImpl on the heap but using a smart pointer to manage it 
public:
  void f() {
    auto temp = make_unique<CImpl>(*pImpl); 
    // copy our state using cImpl's copy ctor

    temp->a.g();
    temp->b.h();

    std::swap(pImpl, temp); // swapping pointers / unique_ptrs is no-throw
  }
};
void swap(T a, T b) { // std::swap may throw if T is an object
  T temp = a;
  a = b;
  b = temp;
}
// Now `f` offers the srong guarantee. It guarantees that if `f` throws an 
// exception, the internal state of the C object is as though `f` had never run.
// Note : if either `A::g()` or `B::h()` offer no exception safety guarantees,
// then in general neither can `C::f()`.

// Examples of basic guarantee:
class Node {
  Node& operator=(const Node& other) { // copy assignment
    if (this == &other) return *this;
    delete next;
    next = nullptr; // removes dangling pointer
    next = other.next ? new Node{*other.next} : nullptr;
    // may throw. If it does, `next` stays as `nullptr`.
    data = other.data;
    return *this;
  }
};  



// ********** Exception Safety and the STL : vector **********
// vector class : 
// -- encapsulates a heap-allocated array
// -- when a stack-allocated vector goes out of scope or a heap-allocated vector
//    is deleted, the internal array is freed and the objects in the array are
//    destroyed (if applicable).
void f() {
  vector<C> v1;             // vector of objects
  vector<C*> v2;            // vector of pointers
  vector<unique_ptr<C>> v3; // vector of smart pointer objects
} // v1, v2, v3 go out of scope
// -- v1 : C dtor runs on all objects in the vector
// -- v2 : ptrs don't have dtors. objects are not deleted.
//         The vector v2 is not considered to own the objects.
// -- v3 : unique_ptr dtor runs on all objects, which deletes each smart 
//         pointer's C object. Don't need explicit delete statements.

// In summary:
// vector<C>              owns the objects
// vector<C*>             does not own the objects
// vector<unique_ptr<C>>  owns the objects

// How does vector provide the strong guarantee? Consider emplace_back.
// If the array is full (size == capacity)
//    -- allocates a new, larger array
//       if allocation fails, the old array is still intact, exits with exception
//    -- copy the objects over (using copy ctor)
//       if a copy ctor throws, need to undo the work done so far:
//       delete objects already copied, delete new array, 
//       old array is still intact, exit with an exception
//    -- delete the old array and the old objects
//    -- exit successfully

// The strong guarantee comes at a price: copying is expensive, 
// and the old objects will just be thrown away, so it is wasteful too.

// Wouldn't moving the objects from the old to the new array be more efficient?
// - allocate the new, larger array
// - move the objects over (using move ctor)
// - delete the old array

// The problem: if a move ctor throws, then emplace_back can’t offer 
// the strong guarantee, because the objects that have already been moved 
// would no longer be intact.
// But if the move ctor offers the 'no-throw guarantee', 
// then emplace_back can and will use the move ctor (faster). 
// Otherwise it will use the copy ctor (slower).

// Therefore, your move operations should provide the no-throw guarantee, 
// if possible, and you should indicate that they do:
class C {
public:
  C( C&& ) noexcept; // move ctor
  C& operator=( C&& ) noexcept; // move assignment 
  // noexcept keyword states the method does not throw
};
// If you know a function will never throw or propagate an exception, 
// declare it noexcept. This faciliates optimization. 
// At a minimum, moves should be noexcept.
