#pragma once

#include <condition_variable>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class DTask {
public:
  virtual int Run() = 0;

  auto GetReturn() {
    // 阻塞等待 set_value
    return p_.get_future().get();
  }

  void SetValue(int v) { p_.set_value(v); }

private:
  std::promise<int> p_; // 接受返回值
};

class ThreadPool {

public:
  /**
   * @brief Init thread pool
   *
   * @param num init num thread
   */
  void Init(int num);

  /**
   * @brief Start thread pool
   *
   */
  void Start();

  /**
   * @brief Stop thread pool
   *
   */
  void Stop();

  /**
   * @brief Add task into tasks
   *
   * @param Task
   */
  void AddTask(std::shared_ptr<DTask> Task);

  /**
   * @brief From Task list Get the Task object
   *
   * @return std::shared_ptr<DThread>
   */
  std::shared_ptr<DTask> GetTask();

  bool is_exit() { return is_exit_; }

  int GetRunTaskCount() { return run_task_counts_; }

private:
  bool is_exit_ = false;             // 线程池退出标志
  int thread_nums_ = 0;              // 线程池线程数量
  std::mutex mux_;                   // 线程池锁
  std::condition_variable cv_;       // 条件变量
  std::atomic<int> run_task_counts_; // 统计运行任务数量

  // 等待任务的线程队列
  std::vector<std::shared_ptr<std::thread>> threads_;
  // 任务队列
  std::list<std::shared_ptr<DTask>> tasks_;

private:
  /**
   * @brief 线程池入口函数
   *
   */
  void Run();
};