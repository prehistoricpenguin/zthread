//: c11:Joining.cpp
// Understanding join().
#include "zthread/Thread.h"
#include <iostream>
#include <string>

using namespace ZThread;
using namespace std;

class Sleeper : public Runnable {
  string name;
  int duration;
public: 
  Sleeper(const std::string& s, int sleepTime) 
    : name(s), duration(sleepTime) { }
  
  virtual ~Sleeper() throw() {}

  virtual void run() throw() {
    try {
      Thread::sleep(duration);
      cout << name << " has awakened" << endl;
    } catch (Interrupted_Exception&) {
      cout << name << " was interrupted. " << " interrupted(): " << Thread::interrupted() << endl;     
    }    
  }
  
};



int main() {

  Handle<Sleeper> sleepy(new Sleeper("Sleepy", 1500));
  Handle<Sleeper> grumpy(new Sleeper("Grumpy", 1500));


} ///:~

