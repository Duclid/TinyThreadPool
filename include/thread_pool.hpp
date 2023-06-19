#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include <iostream>
#include <memory>
#include <condition_variable>


class DTask{
public:
    virtual void Run() = 0;
};


class ThreadPool{

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

private:

    bool is_exit_ = false;          // 线程池退出标志
    int thread_nums_ = 0;           // 线程池线程数量
    std::mutex mux_;                // 线程池锁
    std::condition_variable cv_;    // 条件变量

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