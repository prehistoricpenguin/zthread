//: c11:SleepingBarber.cxx
// The Sleeping Barber problem.
#include "zthread/CountingSemaphore.h"
#include "zthread/Semaphore.h"
#include "zthread/Thread.h"
#include <iostream>

#include "zthread/Singleton.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"

using namespace ZThread;
using namespace std;

class Display {
  Mutex lock;
public:

  void barberSleep() {

    Guard<Mutex> g(lock);
    cout << "Barber sleeping..." << endl;

  }

  void barberAwake() {

    Guard<Mutex> g(lock);
    cout << "Barber awakened, cutting hair" << endl;

  }

  void barberDone() {

    Guard<Mutex> g(lock);
    cout << "Barber finished cutting hair" << endl;

  }

  void shopEnter(int id) {

    Guard<Mutex> g(lock);
    cout << "Customer " << id << " entering shop, waiting for barber." << endl;

  }

  void shopExit(int id) {

    Guard<Mutex> g(lock);
    cout << "Customer " << id << " woke up, leaving shop." << endl;

  }

  void shopFull(int id) {
  
    Guard<Mutex> g(lock);
    cout << "Barber shop full!, Customer " << id << " leaving." << endl;

  }


  void shopHaircut(int id) {

    Guard<Mutex> g(lock);
    cout << "Customer " << id << " getting haircut." << endl;

  }

};

class Generator {

  Mutex lock;
  int id;

public:

  Generator() : id(0) {}

  int next() {
    Guard<Mutex> g(lock);
    return ++id;
  }

};

class BarberShop {

  //! Customers waiting for a haircut
  CountingSemaphore customer;
  //! Haircut completed
  CountingSemaphore barber;
  //! Keep track of when a customer is done
  Semaphore complete;
  //! Serialize data access
  Semaphore mutex;

  //! Number of seats in the shop
  int seats;
  int waiting;

public:

  BarberShop(int n=3) : customer(0), barber(0), complete(0), mutex(1) {
    waiting = 0;
    seats   = n;
  }

  void waitForHaircut(int id) {

    mutex.wait();

    // Leave if the shop is full
    if(waiting < seats) {

      waiting++;

      Singleton<Display>::instance()->shopEnter(id);

      // Let the barber know someone is waiting
      customer.post();     
      mutex.post();

      // Wait for the barber to wake up or to 
      // finish with someone's hair
      barber.wait();

      Singleton<Display>::instance()->shopHaircut(id);

      Thread::sleep(2000);

      // Let the barber know the customer woke up
      complete.post();

      Singleton<Display>::instance()->shopExit(id);

    } else {

      Singleton<Display>::instance()->shopFull(id);

      mutex.post();

    }

  }

  void waitForCustomer() {

    Singleton<Display>::instance()->barberSleep();

    // Wait for a customer to show up
    customer.wait();

    Singleton<Display>::instance()->barberAwake();

    // Serialize access to the waiting variable
    mutex.wait();

    // Update the waiter count
    waiting--;
    
    barber.post();
    mutex.post();

    // Wait for the customer to wake up
    complete.wait();

    Singleton<Display>::instance()->barberDone();

  }

};

class Customer : public Thread {

  BarberShop& shop;
  int id;

public:
  Customer(BarberShop& bs) : shop(bs) { 
    id = Singleton<Generator>::instance()->next();
  }

  virtual ~Customer() throw() {}

  virtual void run() throw() {

    try {

      shop.waitForHaircut(id);

    } catch(Interrupted_Exception&) {
    } catch(Synchronization_Exception& e) {
      cerr << e.what() << endl;
    }

  }

  void stop() {
    interrupt();
    join();
  }

};

class Barber : public Thread {

  BarberShop& shop;

public:
  Barber(BarberShop& bs) : shop(bs) { }

  virtual ~Barber() throw() {}

  virtual void run() throw() {

    try {

      while(!Thread::interrupted()) 
        shop.waitForCustomer();

    } catch(Interrupted_Exception&) {
    } catch(Synchronization_Exception& e) {
      cerr << e.what() << endl;
    }

  }

  void stop() {
    interrupt();
    join();
  }

};

int main() {

  try {

    BarberShop shop;

    Barber   b(shop);
    b.start();

    Customer c[7] = {shop, shop, shop, shop, shop, shop, shop};

    for(int i = 0; i < 7; i++)
      c[i].start();

    // Run for 10 seconds
    Thread::sleep(10000);

    b.stop();

    for(int i = 0; i < 7; i++)
      c[i].stop();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

} ///:~