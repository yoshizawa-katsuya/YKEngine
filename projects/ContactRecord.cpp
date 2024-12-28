#include "ContactRecord.h"
#include <algorithm>

void ContactRecord::AddRecord(uint32_t number)
{
	//履歴に登録
	history_.push_back(number);
}

bool ContactRecord::HistoryCheck(uint32_t number)
{
	return std::any_of(history_.begin(), history_.end(), [number](uint32_t x) { return x == number; });

}

void ContactRecord::Clear()
{
	//履歴を抹消
	history_.clear();
}
