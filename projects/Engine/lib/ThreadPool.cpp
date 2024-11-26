#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false), activeTasks(0) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { worker(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();

    for (std::thread& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
        activeTasks++; // タスクが追加されるたびにカウントをインクリメント
    }
    condition.notify_one();
}

void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(queueMutex);
    finishedCondition.wait(lock, [this] {
        // タスクキューが空であり、すべてのタスクが完了しているか確認
        return tasks.empty() && (activeTasks == 0);
        });
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] {
                return stop || !tasks.empty();
                });

            if (stop && tasks.empty()) return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        // タスクを実行
        task();

        // タスク完了を通知
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            activeTasks--; // 実行中のタスクカウントをデクリメント
        }
        finishedCondition.notify_one(); // すべてのタスクが完了した可能性を通知
    }
}