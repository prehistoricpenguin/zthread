#include <zthread/PoolExecutor.h>
#include <zthread/FastMutex.h>
#include <zthread/Thread.h>

#include <iostream>

#define TASKS 25
#define POOL_SIZE 4

using namespace std;
using namespace ZThread;

class Task : public Runnable {

  int n;

public:

  virtual ~Task() throw() { }

  virtual void run() throw() {

    Thread::sleep(1000);
    cerr << "Task " << n << endl;

  }

};

int main(int argc, char* argv[]) {

  try {
    
    PoolExecutor<FastMutex> executor(POOL_SIZE);
    for(int n = 0; n < TASKS; n++) {

      executor.execute(RunnablePtr(new Task));
      cerr << "Add task" << endl;

    }
    
    cerr << "Waiting for tasks to complete" << endl;

    executor.cancel(); // accept no more tasks
    Thread::sleep(5000);

  } catch(Synchronization_Exception& e) { 
    cerr << e.what() << endl; 
  } catch(...) {
    cerr << "Unexpected error" << endl;
  }

#ifdef WIN32
  cerr << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif
  return 0;

}
