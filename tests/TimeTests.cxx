#include "zthread/Time.h"
#include "zthread/Thread.h"
#include <unistd.h>

#include <assert.h>
#include <iostream>
using namespace std;
using namespace ZThread;

// Test the Time class

int main() {

  Time t1;

  cout << t1.seconds() << '.' << t1.milliseconds() << 's' << endl;

  usleep(100*1000);

  Time t2;

  cout << t2.seconds() << '.' << t2.milliseconds() << 's' << endl;

  sleep(3);

  Time t3;

  cout << t3.seconds() << '.' << t3.milliseconds() << 's' << endl;

  t3 -= 5000;

  cout << t3.seconds() << '.' << t3.milliseconds() << 's' << endl;

  t3 += 5000000;

  cout << t3.seconds() << '.' << t3.milliseconds() << 's' << endl;

  return 0;

}
