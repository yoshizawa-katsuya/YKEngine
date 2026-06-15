#pragma once
#include <memory>
#include <functional>
#include "BaseState.hpp"

namespace YKEngine
{
/// <summary>
/// 状態を管理するクラス
/// </summary>
/// <typeparam name="OwnerType">状態を管理する対象の型</typeparam>
template<typename OwnerType>
class StateMachine
{
public:

	/// <summary>
	/// 開始関数
	/// </summary>
	/// <param name="owner">状態を管理する対象</param>
	void Start(OwnerType* owner);

	/// <summary>
	/// 状態変更関数
	/// </summary>
	/// <typeparam name="StateType">変更後の状態の型</typeparam>
	/// <typeparam name="ArgType">変更後の状態のコンストラクタ引数の型</typeparam>
	/// <param name="args">変更後の状態のコンストラクタ引数</param>
	template<typename StateType, typename... ArgType>
	void ChangeState(ArgType...args);

	/// <summary>
	/// 状態を更新する関数
	/// </summary>
	void Update();

private:

	OwnerType* owner_ = nullptr; //!< 状態を管理する対象

	std::unique_ptr<BaseState<OwnerType>> currentState_ = nullptr; //!< 現在の状態

	std::function<void()> changeStateFunc_; //!< 状態変更関数

};

template<typename OwnerType>
inline void StateMachine<OwnerType>::Start(OwnerType* owner)
{
	owner_ = owner;
	changeStateFunc_ = [](){};
}

template<typename OwnerType>
inline void StateMachine<OwnerType>::Update()
{
	changeStateFunc_(); // 状態変更命令を実行
	changeStateFunc_ = [](){}; // 状態変更命令をクリア

	if (currentState_ != nullptr)
	{
		currentState_->CallUpdate(owner_); // 現在の状態を更新
	}
}

template<typename OwnerType>
template<typename StateType, typename ...ArgType>
inline void StateMachine<OwnerType>::ChangeState(ArgType ...args)
{
	// ステートの変更命令を格納
	changeStateFunc_ = [&]()
		{
			// オーナーが存在しない場合は何もしない
			if (owner_ == nullptr)
			{
				return;
			}

			// 現在のステートが存在する場合は終了処理を行う
			if (currentState_ != nullptr)
			{
				currentState_->CallExit(owner_);
				currentState_.reset();
			}

			// 新しいステートを生成し、開始処理を行う
			currentState_ = std::make_unique<StateType>(args...);
			if (currentState_ == nullptr)
			{
				return;
			}
			// 新しいステートにステートマシンを設定
			currentState_->SetStateMachine(this);
			// 新しいステートの開始処理を行う
			currentState_->CallEnter(owner_);
		};

}

}