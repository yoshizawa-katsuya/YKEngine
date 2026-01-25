#pragma once
#include <cassert>

namespace YKEngine
{

template <typename OwnerType>
class StateMachine;

/// <summary>
/// 状態の基底クラス
/// </summary>
/// <typeparam name="OwnerType">状態を持つオーナーの型</typeparam>
template <typename OwnerType>
class BaseState
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseState() = default;

protected:

	friend class StateMachine<OwnerType>;

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="owner">状態を持つオーナー</param>
	virtual void OnEnter(OwnerType* owner) {}

	/// <summary>
	/// 更新時の処理
	/// </summary>
	/// <param name="owner">状態を持つオーナー</param>
	virtual void OnUpdate(OwnerType* owner) = 0;

	/// <summary>
	/// 終了時の処理
	/// </summary>
	/// <param name="owner">状態を持つオーナー</param>
	virtual void OnExit(OwnerType* owner) {}

private:

	/// <summary>
	/// ステートマシンのセット
	/// </summary>
	void SetStateMachine(StateMachine<OwnerType>* stateMachine) {stateMachine_ = stateMachine;}

	/// <summary>
	/// ステートマシンから呼ばれる更新処理
	/// </summary>
	/// <param name="owner">状態を持つオーナー</param>
	void CallEnter(OwnerType* owner);
	
	/// <summary>
	/// ステートマシンから呼ばれる更新処理
	/// </summary>
	/// <param name="owner">状態を持つオーナー</param>
	void CallUpdate(OwnerType* owner);

	/// <summary>
	/// ステートマシンから呼ばれる終了処理
	/// </summary>
	/// <param name="owner">状態を持つオーナー</param>
	void CallExit(OwnerType* owner);

protected:

	// この状態を管理するステートマシンへのポインタ
	StateMachine<OwnerType>* stateMachine_ = nullptr;

};

template<typename OwnerType>
inline void BaseState<OwnerType>::CallEnter(OwnerType* owner)
{
	assert(stateMachine_ != nullptr && owner != nullptr);

	OnEnter(owner);
}

template<typename OwnerType>
inline void BaseState<OwnerType>::CallUpdate(OwnerType* owner)
{
	assert(stateMachine_ != nullptr && owner != nullptr);

	OnUpdate(owner);
}

template<typename OwnerType>
inline void BaseState<OwnerType>::CallExit(OwnerType* owner)
{
	assert(stateMachine_ != nullptr && owner != nullptr);

	OnExit(owner);
}

}