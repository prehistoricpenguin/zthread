//: c11:Restaurant.cpp
// The producer-consumer approach to thread cooperation.
#include "zthread/Condition.h"
#include "zthread/Mutex.h"
#include "zthread/Singleton.h"
#include "zthread/Thread.h"
#include <iostream>
#include <deque>

using namespace ZThread;
using namespace std;

class Order {
  int number;
public:

  Order(int num) : number(num) {}
  int getNumber() const { return number; }

  friend ostream& 
  operator<<(ostream& os, Order ord) { 
    return os << "Order " << ord.number; 
  }

};

class Restaurant : public Cancelable {
  
  Mutex lock;
  Condition notEmpty;
  Condition isEmpty;
  
  deque<Order> queue;
  bool canceled;

public:

  Restaurant() : notEmpty(lock), isEmpty(lock), canceled(false) {}

  virtual ~Restaurant() throw() {}

  Order get() {

    Guard<Mutex> g(lock);
      
    while (queue.size() == 0 && !canceled) 
      notEmpty.wait();
    
    if(queue.size() == 0) // Queue canceled
      throw Cancellation_Exception();  

    Order order = queue.front();
    queue.pop_front();

    // Wake empty waiters
    if(queue.size() == 0) 
      isEmpty.broadcast();

    return order;

  }

  void put(Order order) {

    Guard<Mutex> g(lock);

    if(canceled)
      throw Cancellation_Exception();

    queue.push_back( order );

    // Wake one waiter
    notEmpty.signal(); 

  }

  virtual void cancel() {

    Guard<Mutex> g(lock);

    canceled = true;
    notEmpty.broadcast(); 

  }

  virtual bool isCanceled() {

    Guard<Mutex> g(lock);

    return canceled; 

  }

};

class Display {
  Mutex lock;
public:

  void pickedUp(Order order) {
    Guard<Mutex> g(lock);
    cout << "Picked up " << order << endl;
  }

  void cookedUp(Order order) {
    Guard<Mutex> g(lock);
    cout << "Cooked up " << order << endl;
  }

};

class Customer : public Thread {
  Restaurant& restaurant;
public:

  Customer(Restaurant& r) : restaurant(r) {}
  virtual ~Customer() throw() {}

  virtual void run() throw() {

    try {

      while(!Thread::interrupted()) {

        Order order = restaurant.get();
        Singleton<Display>::instance()->pickedUp(order);

        Thread::sleep(2000);

      }

    } catch(Interrupted_Exception&) {
      // Exit
    } catch(Synchronization_Exception& e) {
      cerr << e.what() << endl;
    }

  }

};

class Chef : public Thread {
  Restaurant& restaurant;
  unsigned int count, limit;
public:

  Chef(Restaurant& r, unsigned int max = 10) : restaurant(r), count(0), limit(max) {}
  virtual ~Chef() throw() {}

  virtual void run() throw() {

    try {

      while(!Thread::interrupted()) {

        Order order(count++);

        restaurant.put(order);
        Singleton<Display>::instance()->cookedUp(order);

        Thread::sleep(1000);

        if(count >= limit)
          break;

      }

      restaurant.cancel();

    } catch(Interrupted_Exception&) {
      // Exit
    } catch(Synchronization_Exception& e) {
      cerr << e.what() << endl;
    }

  }

};

int main() {

  try {

    Restaurant restaurant;

    Customer t1(restaurant);
    Chef     t2(restaurant, 10);

    t1.start();
    t2.start();

    t1.join();
    t2.join();   

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

  return 0;

} ///:~