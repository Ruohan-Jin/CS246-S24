// 24-observer
// ********** 1. Observer Pattern **********
// used to implement a publish/subscribe model
// One class generates/updates data: subject/publisher
// Many classes receive data and react to it: observer/subscriber

// subject: manages some important data
// observers: receive updates when the subject's data changes
// others: not an observer, but could be one

// UML Class Diagram:
// observers : subject keeps track of the observers that want to be notified
// -state    : represents the data that the subject is responsible for
// Observer::notify() : notify is called then the state changes
// ConcreteObserver   : does something with the data when notified
 ------------------
|Subject           |
|------------------|                       				---------
|                  |          			observers    |Observer | 
|------------------|  #---------------------->   |---------|
|+attach(Observer) |               				0..*   |         |
|+detach(Observer) |             					 				--------- 
|+notifyObservers()|            								 |+notify()|    
 ------------------              					        ---------
				  @				 				 				 				 				    @
					|				 				 				 				 				    |	
					|				 				 				 				 				    |	
					|				 				 				 				 				    |	
					|				 				 				 				 				    |	
 ------------------			 				 				 				      |	
|ConcreteSubject   |			 				 				 				    |	
|------------------|                       				 -----------------
|-state            |    subject      			    		|ConcreteObserver | 
|------------------|  <----------------------#    |---------------- |
|+getState()       |               				        |                 |
|									 |             					 				 ----------------- 
|									 |            								  |+notify()        |    
 ------------------              					         -----------------
// #是空心菱形，@是空心三角形

// sequence of calls:
// 1. Observers are attached to the Subject, indicates desire to be notified
// 2. Subject's state is updated
// 3. `Subject::notifyObservers()` is called 
//    - it calls each Observer's notify method
// 4. Observer calls ConcreteSubject::getState() 
//    to query the state and react accordingly, could loop to step1
// 5. When observers shut down or are no longer interested, 
//    they detach from the Subject.

// UML Diagram
 ------------------
|Subject           |
|------------------|                       				---------
|                  |          			observers    |Observer | 
|------------------|#------------------------>   |---------|
|+attach(Observer) |               				0..*   |         |
|+detach(Observer) |             					 				--------- 
|+notifyObservers()|            								 |+notify()|    
 ------------------              					        ---------
				  @				 				 				 				 				    @
					|				 				 				 				 				    |	
					|				 				 				 				 				    |	
					|				 				 				 				 				    |	
					|				 				 				 				 				    |	
 -------------------------			 				 				 			|	
|HorseRace                |			 				 				 			|	
|-------------------------|                       -----------------
|-in : ifstream    	      |    subject      	   |Bettor           | 
|-lastWinner : string     |											 |---------------- |                 
|-------------------------|  <----------------#  |-name : string   |
|+runRace() : boolean     |               			 |-myHorse : string|
|+getLastWinner() : string|             					----------------- 
|									        |            					 |+notify()        |    
 -------------------------              				  -----------------
				  |                                        		 | 
					|hr                                  bettors | 0..* 
					|              ------------                  |  
					| 						|TestHarness |                 |  
					|____________$|------------|$________________|  
												|            |
												 ------------ 
												|+main()     |    
												 ------------
// @是空心三角形，#是空心菱形，$是實心菱形

// TestHarness 
// $ : indicate composition, TestHarness is responsible for 
//     creating/destroying the HorseRace and Bettor objects
// +main():
// 1. create `HorseRace`.
// 2. create `Bettor`s and attach to `HorseRace`.
// 3. call `runRace()`.
// 4. call `notifyObservers()`.
// 5. repeat steps3-4 until all races have been run
// 6. clean up

// Important Points:
// 1. Dtor `~Subject()` is pure virtual - that makes `Subject` abstract
// 2. The Subject doesn't know anything about the Observers 
//    other than the fact that they all have the `notify()` method
// 3. New types of Observers can be created and added at any time 
//    without changing the code in the Subject.
// 4. Order of notification is generally not guaranteed.
// 5. Subject controls the state. Observers query for it. 
//    Alternatively the Subject can push the data to the Observers.
// 6. State is not defined in the abstract base class Subject 
//    because it varies from one subject to the next
// 7. You can reuse Subjects and Observers independently of one another
// 8. Concrete Subject uses implementation inheritance to get 
//    the basic functionality of attaching, detaching, and notifying for free
// 9. Interface inheritance allows all subjects to be used 
//    interchangeably and same for Observers


// ********** 2. Decorator Pattern **********

// client code
Pizza* p = new CrustAndSauce;
p = new StuffedCrust(p);
p = new Topping("cheese", p);
p = new Topping("pepperoni", p);
std::cout << "Your " << p->desc() << "pizza costs $" << p->cost() << endl;
// A CrustAndSauce object is a Pizza(through inheritance)
// A CrustAndSauce object wrapped in a decorator is still a Pizza
// We start with a crustAndSauce object
// Decorate it with a StuffedCrust object
// Decorate it with a Topping object
// Decorate it with a Topping object
// Call the cost() method which relies on delegation to add up all the costs

class Pizza {
public:
  virtual float cost() const = 0;
  virtual ~Pizza() = default;
};

class crustAndSauce : public Pizza {
public:
  float cost() const override {
    return 5.99;
  }
};

class Decorator : public Pizza {
protected:
  Pizza* component;
public:
  Decorator(Pizza* p) : component{p} { }
  virtual ~Decorator() { delete component; } 
  // virtual so subclass dtor will be called when using a base class pointer
};

class StuffedCrust : public Decorator {
public:
  StuffedCrust(Pizza *p) : Decorator{p} { }
  float cost() const override {
    return component->cost() + 2.69;
  }
};

// Important Points:
// 1. Decorators have the same supertypes as the objects they decorate
// 2. You can use one or more decorators to wrap an object
// 3. We can pass around a decorated object in place of the original object
// 4. The decorator adds its own behavior before and/or after 
//    delegating to the object it decorates
// 5. objects can be decorated at any time so we can decorate objects at runtime
// 6. Decorator pattern is an alternative to subclassing for extending bahavior
