//
// This example demonstrates the thread safe singleton and use of 
// Guard object to perfom scoped locking
//
#include <zthread/Thread.h>

#include <zthread/FastMutex.h>
#include <zthread/FastRecursiveMutex.h>
#include <zthread/Mutex.h>
#include <zthread/RecursiveMutex.h>

#include <iostream>

using namespace ZThread;
using namespace std;

Mutex m;
FastMutex fm;
FastRecursiveMutex frm;
RecursiveMutex rm;


int main(int argc, char* argv[]) {

  try {

    cerr << "Acquiring locks" << endl;

    m.acquire(); cerr << "Locked Mutex" << endl;

    fm.acquire(); cerr << "Locked Fast Mutex" << endl;
    rm.acquire(); cerr << "Locked Recursive Mutex" << endl; 
    frm.acquire(); cerr << "Locked Fast Recursive Mutex" << endl;

    //fm.acquire(); // will deadlock
    //m.acquire(); // will deadlock

    cerr << "Acquiring recursive locks" << endl;

    rm.acquire();  // OK
    frm.acquire(); // OK

    try {

      m.acquire(); // will throw deadlock exception

    } catch(Synchronization_Exception& e) { 
      cerr << e.what() << " OK" << endl; 
    }

    rm.release();  // OK
    frm.release(); // OK

    m.release();
    fm.release();
    rm.release(); 
    frm.release();

    try {

      m.release();

    } catch(Synchronization_Exception& e) { 
      cerr << e.what() << " OK" << endl; 
    }

    try {

      rm.release(); // will throw invalid exception

    } catch(Synchronization_Exception& e) { 
      cerr << e.what() << " OK" << endl; 
    }

  } catch(Synchronization_Exception& e) { cerr << e.what() << endl; }

#ifdef WIN32
  cerr << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif
  return 0;

}


