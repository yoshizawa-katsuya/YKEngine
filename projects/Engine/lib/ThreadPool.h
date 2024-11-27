#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    
    //シングルトンインスタンスの取得
    static ThreadPool* GetInstance();

    void Initlaize();

    void Finalize();

    // タスクを追加する
    void enqueueTask(std::function<void()> task);

    template<class F, class... Args>
    void enqueueTask(F&& f, Args&&... args);

    // すべてのタスクが完了するのを待つ
    void waitForCompletion();

private:

    ThreadPool() = default;
    ~ThreadPool() = default;
    ThreadPool(ThreadPool*) = delete;
    const ThreadPool& operator=(ThreadPool&) = delete;


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

template<class F, class ...Args>
inline void ThreadPool::enqueueTask(F&& f, Args && ...args)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        activeTasks++; // タスクが追加されるたびにカウントをインクリメント
    }
    condition.notify_one();
}
