#include "zthread/Time.h"
#include "TimeStrategy.h"


using namespace ZThread;

Time::Time() {
  
  // System startup time
  static TimeStrategy firstHelper;
  TimeStrategy helper;
  
  _seconds = (helper.seconds() - firstHelper.seconds());
  _milliseconds = (helper.milliseconds() - firstHelper.milliseconds());

}


