//: c11:DiningPhilosophers.cxx
// Dining Philosohophers w/ Deadlock
#include "zthread/Condition.h"
#include "zthread/Guard.h"
#include "zthread/Mutex.h"
#include "zthread/Singleton.h"
#include "zthread/Thread.h"

#include <fstream>
#include <iostream>
#include <string>

#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace ZThread;
using namespace std;

#define PHILOSOPHERS  5

class Random { 
public:

  Random() {
  
    // Seed the random number generator
    struct seed_t {
      seed_t() { srand((unsigned)time(0)); }
    };

    static struct seed_t seed;
    
  }

  unsigned long operator()(unsigned long max) {
    return static_cast<unsigned long>(rand()) % max;
  }

};

class Philosopher;

class Display {
  Mutex lock;
public:
  void thinking(const Philosopher& p);
  void eating(const Philosopher& p);
  void left(const Philosopher& p);
  void right(const Philosopher& p);
};

class Chopstick {

  Mutex lock;
  Condition notTaken;
  bool taken;

public:

  Chopstick() : notTaken(lock), taken(false) {}

  void take() {

    Guard<Mutex> g(lock);
  
    while(taken)
      notTaken.wait();

    taken = true;

  }

  void drop() {

    Guard<Mutex> g(lock);
    
    taken = false;
    notTaken.signal();

  }

};

class Philosopher : public Runnable {

  Chopstick& left;
  Chopstick& right;
  int id; 

  Random random;

public:

  Philosopher(Chopstick& l, Chopstick& r, int ident) 
    : left(l), right(r), id(ident) {}

  virtual ~Philosopher() throw() {}

  virtual void run() throw() {

    try {

      while(!Thread::interrupted()) {

        Singleton<Display>::instance()->thinking(*this);
        Thread::sleep(random(5)*1000);     

        // Hungry
        Singleton<Display>::instance()->right(*this);
        right.take();

        Singleton<Display>::instance()->left(*this);
        left.take();        

        // Eating
        Singleton<Display>::instance()->eating(*this);
        Thread::sleep(random(5)*1000);     

        right.drop();
        left.drop();

      }

    } catch(Synchronization_Exception& e) {
      cerr << *this << e.what() << endl;
    }

  }

  int identity() const { return id; }

  friend ostream& 
  operator<<(ostream& os, const Philosopher& p) {
    return os << "Philosopher " << p.identity();
  }

};

void Display::thinking(const Philosopher& p) {
  Guard<Mutex> g(lock);
  cout << p << " thinking" << endl;
}

void Display::eating(const Philosopher& p) {
  Guard<Mutex> g(lock);
  cout << p << " eating" << endl;
}

void Display::left(const Philosopher& p) {
  Guard<Mutex> g(lock);
  cout << p << " grabbing left" << endl;
}

void Display::right(const Philosopher& p) {
  Guard<Mutex> g(lock);
  cout << p << " grabbing right" << endl;
}

int main() { 

  cout << "Press <ENTER> to quit" << endl;

  try {

    Thread    t[PHILOSOPHERS];
    Chopstick c[PHILOSOPHERS];

    for(int i = 0; i < PHILOSOPHERS; i++) {

      int j = (i+1) > (PHILOSOPHERS-1) ? 0 : (i+1);

      t[i].run(new Philosopher(c[i], c[j], i));

    }

    // Wait for any key
    string s;
    getline(cin, s);

    for(int i = 0; i < PHILOSOPHERS; i++)
      t[i].interrupt();

    for(int i = 0; i < PHILOSOPHERS; i++)
      t[i].join();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

  return 0;

} ///:~