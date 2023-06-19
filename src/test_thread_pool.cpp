#include "../include/thread_pool.hpp"
#include <iostream>
#include <memory>
#include <thread>

using namespace std;

class MyTask : public DTask {
public:
  MyTask(string name) : name_(name) {
    cout << "Construct MyTask : " << name << endl;
  }

  MyTask(const MyTask &other) {
    cout << "Copy construct MyTask : " << other.name_ << endl;
    this->name_ = other.name_;
  }

  void Run() {
    cout << "============================================================="
         << endl;
    cout << "== Thread : " << this_thread::get_id() << " Mytask" << name_
         << endl;
    cout << "============================================================="
         << endl;

    // 模拟任务
    for (int i = 0; i < 10; i++) {
      cout << "." << endl;
      this_thread::sleep_for(500ms);
    }
  }

  ~MyTask() { cout << "Distruct MyTask : " << name_ << endl; }

  string GetName() { return name_; }

private:
  string name_;
};

int main(int argc, char **argv) {

  ThreadPool thread_pool;
  thread_pool.Init(3);
  thread_pool.Start();

  auto task1 = make_shared<MyTask>("task_001");
  thread_pool.AddTask(task1);

  auto task2 = make_shared<MyTask>("task_002");
  thread_pool.AddTask(task2);

  auto task3 = make_shared<MyTask>("task_003");
  thread_pool.AddTask(task3);

  auto task4 = make_shared<MyTask>("task_004");
  thread_pool.AddTask(task4);

  auto task5 = make_shared<MyTask>("task_005");
  thread_pool.AddTask(task5);

  //this_thread::sleep_for(5s);
  thread_pool.Stop();

  getchar();
  return 0;
}
