#pragma once
#include <vector>


/// <summary>
/// 接触記録
/// </summary>
class HittingRecord {
public:

	//履歴追加
	void HistoryAdd(uint32_t number);

	//履歴チェック
	bool HistoryCheck(uint32_t number);

	//履歴抹消
	void Clear();

private:

	//履歴
	std::vector<uint32_t> history_;

};
