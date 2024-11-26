#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    // タスクを追加する
    void enqueueTask(std::function<void()> task);

    // すべてのタスクが完了するのを待つ
    void waitForCompletion();

private:
    // ワーカースレッドの関数
    void worker();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable finishedCondition; // タスク完了を通知するための条件変数
    bool stop;

    std::atomic<int> activeTasks; // 実行中または待機中のタスクのカウンター
};