#include "ThreadPool.h"
#include <cassert>

ThreadPool* ThreadPool::GetInstance()
{
    static ThreadPool instance;
    return &instance;
}

void ThreadPool::Initlaize()
{
    isStop_ = false;
    activeTasks_ = 0;

    uint32_t numThreads = std::thread::hardware_concurrency() - 1;
    assert(numThreads > 0);

    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back([this] { worker(); });
    }
}

void ThreadPool::Finalize()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        isStop_ = true;
    }
    condition_.notify_all();

    for (std::thread& worker : workers_) {
        worker.join();
    }
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        tasks_.push(task);
        activeTasks_++; // タスクが追加されるたびにカウントをインクリメント
    }
    condition_.notify_one();
}

void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(queueMutex_);
    finishedCondition_.wait(lock, [this] {
        // タスクキューが空であり、すべてのタスクが完了しているか確認
        return tasks_.empty() && (activeTasks_ == 0);
    });
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            condition_.wait(lock, [this] {
                return isStop_ || !tasks_.empty();
            });

            if (isStop_ && tasks_.empty()) return;

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        // タスクを実行
        task();

        // タスク完了を通知
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            activeTasks_--; // 実行中のタスクカウントをデクリメント
        }
        finishedCondition_.notify_one(); // すべてのタスクが完了した可能性を通知
    }
}