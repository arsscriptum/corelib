
//==============================================================================
//
//   test_blocking_queue.cpp    --- test for blocking queues
//
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================



#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>
#include "blocking_queue.h"


using namespace Async;

std::mutex ioLock;

void test(BlockingQueue<std::string>* pQ)
{
  std::string msg;
  do
  {
    msg = pQ->deQ();
    {
      std::lock_guard<std::mutex> l(ioLock);
      std::cout << "\n  thread deQed " << msg.c_str();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while(msg != "quit");
}

int main()
{

  BlockingQueue<std::string> q;
  std::thread t(test, &q);

  for(int i=0; i<15; ++i)
  {
    std::ostringstream temp;
    temp << i;
    std::string msg = std::string("msg#") + temp.str();
    {
      std::lock_guard<std::mutex> l(ioLock);
      std::cout << "\n   main enQing " << msg.c_str();
    }
    q.enQ(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
  q.enQ("quit");
  t.join();

  std::cout << "\n";
  std::cout << "\n  Making move copy of BlockingQueue";
  std::cout << "\n -----------------------------------";

  std::string msg = "test";
  q.enQ(msg);
  std::cout << "\n  before move:";
  std::cout << "\n    q.size() = " << q.size();
  std::cout << "\n    q.front() = " << q.front();
  BlockingQueue<std::string> q2 = std::move(q);  // move assignment
  std::cout << "\n  after move:";
  std::cout << "\n    q2.size() = " << q2.size();
  std::cout << "\n    q.size() = " << q.size();
  std::cout << "\n    q2 element = " << q2.deQ() << "\n";

  std::cout << "\n  Move assigning state of BlockingQueue";
  std::cout << "\n ---------------------------------------";
  BlockingQueue<std::string> q3;
  q.enQ("test");
  std::cout << "\n  before move:";
  std::cout << "\n    q.size() = " << q.size();
  std::cout << "\n    q.front() = " << q.front();
  q3 = std::move(q);
  std::cout << "\n  after move:";
  std::cout << "\n    q.size() = " << q.size();
  std::cout << "\n    q3.size() = " << q3.size();
  std::cout << "\n    q3 element = " << q3.deQ() << "\n";

  std::cout << "\n\n";
}
