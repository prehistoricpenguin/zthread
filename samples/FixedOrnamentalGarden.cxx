//: c11:FixedOrnamentalGarden.cxx
// Right way to share a resource
#include "zthread/Condition.h"
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

class Count { 
  Mutex lock;
  Random random;
  int value;
public:

  Count() : value(0) {}

  int increment() {

    lock.acquire();

    int temp = value;
    if(random(2) % 2) Thread::yield();
    value = ++temp;    

    lock.release();

    return temp;

  }

};

class Entrance;

class Display {
  Mutex lock;
public:
  void show(const Entrance&, int);
};

class Entrance : public Runnable {

  static Count count;
  
  int value;
  int id;

public:

  Entrance(int n) : value(0), id(n) {}
  virtual ~Entrance() throw() {}

  virtual void run() throw() {

    try {

      while(!Thread::interrupted()) {

        int total = count.increment();
        int Entrance  = value++;

        Singleton<Display>::instance()->show(*this, total);
        Thread::sleep(500);

      }

    } catch(Synchronization_Exception& e) {
      cerr << *this << e.what() << endl;
    }

  }

  operator int() const { return value; }

  int identity() const { return id; }

  friend ostream& 
  operator<<(ostream& os, const Entrance& e) {
    return os << "Entrance " << e.identity() << ": " << (int)e;
  }

};

Count Entrance::count;

void Display::show(const Entrance& e, int total) {
  lock.acquire();
  cout << e << " Total: " << total << endl;
  lock.release();
}

int main() { 

  cout << "Press <ENTER> to quit" << endl;

  try {
        
    Thread   t[2];
    
    t[0].run(new Entrance(1));
    t[1].run(new Entrance(2));
      
    string s;
    getline(cin, s);

    t[0].interrupt();
    t[1].interrupt();

    t[0].join();
    t[1].join();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

  return 0;

} ///:~