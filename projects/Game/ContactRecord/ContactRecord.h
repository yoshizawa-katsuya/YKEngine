#pragma once
#include <vector>
//接触記録
class ContactRecord
{
public:
	//履歴追加
	void AddRecord(uint32_t number);

	//履歴チェック
	bool HistoryCheck(uint32_t number);

	//履歴抹消
	void Clear();

private:
	std::vector<uint32_t> history_;
};

