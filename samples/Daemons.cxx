//: c11:Daemons.cpp
// Daemon threads spawn other daemon threads.
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class DaemonSpawn : public Runnable {
public:

  virtual ~DaemonSpawn() throw() {}

  virtual void run() throw() {

    try {

      Thread::sleep(3000);

    } catch(Interrupted_Exception&) {
      // Exit the task on error
    }

  }
 
};

class DaemonTask : public Runnable {
public:

  virtual ~DaemonTask() throw() {}

  virtual void run() throw() {
    
    try {
      
      Thread t[10];

      // Start the non-daemon threads
      for(int i = 0; i < 10; i++) {
        //if(i % 2)

        t[i].run(new DaemonSpawn);
        //     cout << "t[" << i << "] = " << t[i].isDaemon() << endl;
      }

      // Non daemon threads must be joined or canceled
      // in order for the program to exit.
      for(int i = 0; i < 10; i++) 
        t[i].join();

    } catch(Interrupted_Exception&) {
      // Exit the task on error
    }

  }

};

int main() {

  Thread t;
  t.run(new DaemonTask);
    

} ///:~
