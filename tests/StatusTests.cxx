#if !defined(STATUS_HEADER)
#  define STATUS_HEADER "../src/Status.h"
#endif

#include STATUS_HEADER

#include <assert.h>
#include <iostream>
using namespace std;
using namespace ZThread;

// This test runs through the transitions the Status class is responsible for

int main() {

  cout << "Status Test:" << endl;
  Status s;

  // Test SIGNALED 
  s.push(Status::SIGNALED);
  assert(s.pending(Status::SIGNALED));
  assert(s.next() == Status::SIGNALED);
  assert(!s.pending(Status::SIGNALED));

  // Test INTERRUPTED 
  s.push(Status::INTERRUPTED);
  assert(s.pending(Status::INTERRUPTED));
  assert(s.next() == Status::INTERRUPTED);
  assert(!s.pending(Status::INTERRUPTED));

  // Test SIGNALED absorbs TIMEDOUT
  s.push(Status::SIGNALED);
  s.push(Status::TIMEDOUT);
  assert(s.pending(Status::SIGNALED));
  assert(s.next() == Status::SIGNALED);
  assert(!s.pending(Status::TIMEDOUT));
  assert(!s.pending(Status::SIGNALED));
  assert(!s.pending(Status::ANYTHING));

  // Test TIMEDOUT preferred over INTERRUPTED
  s.push(Status::INTERRUPTED);
  s.push(Status::TIMEDOUT);
  assert(s.pending(Status::ANYTHING));
  assert(s.next() == Status::TIMEDOUT);
  assert(!s.pending(Status::TIMEDOUT));
  assert(s.next() == Status::INTERRUPTED);
  assert(!s.pending(Status::ANYTHING));

  // Test SIGNALED preferred over INTERRUPTED
  s.push(Status::INTERRUPTED);
  s.push(Status::SIGNALED);
  assert(s.pending(Status::ANYTHING));
  assert(s.next() == Status::SIGNALED);
  assert(!s.pending(Status::SIGNALED));
  assert(s.next() == Status::INTERRUPTED);
  assert(!s.pending(Status::ANYTHING));


  // Test interest mask 
  s.push(Status::SIGNALED);
  s.push(Status::INTERRUPTED);
  s.interest(Status::INTERRUPTED);   

  assert(s.pending(Status::INTERRUPTED));
  assert(s.next() == Status::INTERRUPTED);

  assert(!s.pending(Status::ANYTHING));

  s.interest(Status::ANYTHING);   
  assert(s.next() == Status::SIGNALED);
  assert(!s.pending(Status::ANYTHING));

  s.push(Status::CANCELED);
  assert(!s.pending(Status::ANYTHING));
 
  cout << "Status Test:  PASSED" << endl;

  return 0;

}

