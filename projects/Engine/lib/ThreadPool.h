#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

/// <summary>
/// 複数のスレッドでタスクを並列に実行するためのクラス。
/// タスクの追加、完了待機が可能。
/// タスクの実行中に新しいタスクを追加可能。
/// タスクの完了を待つ機能も提供。
/// </summary>
class ThreadPool {
public:
    
    /// <summary>
	/// シングルトンインスタンスを取得する。
    /// </summary>
	/// <returns>ThreadPoolのシングルトンインスタンス</returns>
    static ThreadPool* GetInstance();

    /// <summary>
	/// スレッドプールを初期化する。
    /// </summary>
    void Initlaize();

    /// <summary>
	/// スレッドプールを終了する。
    /// </summary>
    void Finalize();

    /// <summary>
	/// タスクをキューに追加する。
    /// </summary>
	/// <param name="task">実行するタスク(関数オブジェクト)</param>
    void enqueueTask(std::function<void()> task);

    /// <summary>
	/// 可変引数を持つ関数をタスクとしてキューに追加する。
    /// </summary>
	/// <typeparam name="F">関数オブジェクトの型</typeparam>
	/// <typeparam name="Args">関数の引数の型</typeparam>
	/// <param name="f">関数オブジェクト</param>
	/// <param name="args">関数の引数</param>
    template<class F, class... Args>
    void enqueueTask(F&& f, Args&&... args);

    /// <summary>
	/// すべてのタスクの完了を待機する。
    /// </summary>
    void waitForCompletion();

private:

    ThreadPool() = default;
    ~ThreadPool() = default;
    ThreadPool(ThreadPool*) = delete;
    const ThreadPool& operator=(ThreadPool&) = delete;


    /// <summary>
	/// ワーカースレッドのメインループ。
    /// </summary>
    void worker();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::condition_variable finishedCondition_; // タスク完了を通知するための条件変数
    bool isStop_;

    std::atomic<int> activeTasks_; // 実行中または待機中のタスクのカウンター
};

template<class F, class ...Args>
inline void ThreadPool::enqueueTask(F&& f, Args && ...args)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        tasks_.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        activeTasks_++; // タスクが追加されるたびにカウントをインクリメント
    }
    condition_.notify_one();
}
