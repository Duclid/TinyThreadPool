#include "../include/thread_pool.hpp"
#include <memory>
#include <thread>

using namespace std;

/**
 * @brief Init thread pool
 *
 * @param num init num thread
 */
void ThreadPool::Init(int num) {
  // lock mux
  unique_lock<mutex> lock(mux_);
  // init
  this->thread_nums_ = num;

  cout << "Init Thread Pool" << endl;
}

/**
 * @brief Start thread pool
 *
 */
void ThreadPool::Start() {
  // lock
  unique_lock<mutex> lock(mux_);

  // check thread_pool not init
  if (thread_nums_ == 0) {
    cerr << "Thread pool not init !" << endl;
    return;
  }

  // check if thread_pool has start
  if (!threads_.empty()) {
    cerr << "Thread pool has start !" << endl;
    return;
  }

  // init threads
  for (int i = 0; i < thread_nums_; i++) {
    auto th = make_shared<thread>(&ThreadPool::Run, this);
    threads_.push_back(th);
  }
}

/**
 * @brief Stop thread pool
 *
 */
void ThreadPool::Stop() {
    is_exit_ = true;
    cv_.notify_all();
    	for (auto& th : threads_) {
		th->join();
	}
	unique_lock<mutex> lock(mux_);
	threads_.clear();
}

/**
 * @brief Add task into tasks
 *
 * @param Task
 */
void ThreadPool::AddTask(std::shared_ptr<DTask> task) {
  // lock
  unique_lock<mutex> lock(mux_);
  tasks_.push_back(task);
  lock.unlock();
  // notice GetTask
  cv_.notify_one();
}

/**
 * @brief From Task list Get the Task object
 *
 * @return std::shared_ptr<DThread>
 */
std::shared_ptr<DTask> ThreadPool::GetTask() {
  unique_lock<mutex> lock(mux_);
  // check tasks is empty, wait
  if (tasks_.empty()) {
    cv_.wait(lock);
  }

  // if thread_pool will stop
  if (is_exit()) {
    return nullptr;
  }

  // if cv_ is notice_all
  if (tasks_.empty()) {
    return nullptr;
  }

  auto task = tasks_.front();
  tasks_.pop_front();
  return task;
}

/**
 * @brief 线程池入口函数
 *
 */
void ThreadPool::Run(){

    while(!is_exit()){
        // allocate thread run task
        auto task = GetTask();
        ++run_task_counts_;
        if(!task){
            continue;
        }

        try{
            //cout << "| Thread : " << this_thread::get_id() << " run |" << endl;
            auto re = task->Run();
            task->SetValue(re);
        }catch(...){

        }
        --run_task_counts_;
    }
}