//: c11:ResponsiveUI.cxx
// Very simple Threading example.
#include "zthread/ConcurrentExecutor.h"
#include "zthread/Mutex.h"
#include <fstream>
#include <iostream>

using namespace ZThread;
using namespace std;


class SaveTask : public Runnable {

  ofstream out;
  string text;

public:

  SaveTask(const string& file, const string& data) : text(data) {
    out.open(file.c_str());
  }

  virtual ~SaveTask() throw() { 
    out.close();
  }

  virtual void run() throw() {
    // Write the text, include a delay to simulate alot of work
    out << text;
    Thread::sleep(1000);
  }

};


int main() {

  ConcurrentExecutor<Mutex> executor;
  string buf;

  cout << "Enter the EOF character to quit:" << endl;
  
  do {

    // Read input from the console
    string s;
    getline(cin, s);
  
    buf += s;
    buf += '\n';
    
    // Whenever we hit a blank line, save in the background.
    if(s.size() < 1 || cin.eof())
      executor.execute(new SaveTask("output.txt", buf));

  } while(cin.good());

  cout << "Shutting down..." << endl;

  executor.cancel();

  return 0;

} ///:~
