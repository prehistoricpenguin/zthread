#include "zthread/Time.h"
#include "TimeStrategy.h"


using namespace ZThread;

Time::Time() {
  
  // System startup time
  static TimeStrategy firstHelper;
  TimeStrategy helper;

  Time then(firstHelper.seconds(), firstHelper.milliseconds());
  Time now(helper.seconds(), helper.milliseconds());

  now -= then;

  _seconds = now.seconds(); 
  _milliseconds = now.milliseconds();

}


