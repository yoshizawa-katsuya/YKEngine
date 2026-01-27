#include "GlobalVariables.h"
#include <fstream>
#include <Windows.h>

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

GlobalVariables* GlobalVariables::GetInstance()
{
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() {

#ifdef USE_IMGUI
	if (!ImGui::Begin("Global Variables", nullptr))
	{
		ImGui::End();
		return;
	}
	

	for (std::map<std::string, Group>::iterator itGroup = dates_.begin();
		itGroup != dates_.end(); ++itGroup) {

		//グループ名を取得
		const std::string& groupName = itGroup->first;
		//グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::TreeNode(groupName.c_str()))
		{
			continue;
		}

		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.begin();
			itItem != group.end(); ++itItem) {

			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			//int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item))
			{
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::DragInt(itemName.c_str(), ptr, 0.01f);
			}

			//float型の値を保持していれば
			else if (std::holds_alternative<float>(item))
			{
				float* ptr = std::get_if<float>(&item);
				ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
			}

			//Vector2型の値を保持していれば
			else if (std::holds_alternative<Vector2>(item))
			{
				Vector2* ptr = std::get_if<Vector2>(&item);
				ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
			}

			//Vector3型の値を保持していれば
			else if (std::holds_alternative<Vector3>(item)) 
			{
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
			}

			// Vector4型の値を保持していれば
			else if (std::holds_alternative<Vector4>(item)) 
			{
				Vector4* ptr = std::get_if<Vector4>(&item);
				ImGui::DragFloat4(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
			}

			// ColorEdit4で編集できるようにする
			else if (std::holds_alternative<Color>(item)) 
			{
				Color* ptr = std::get_if<Color>(&item);
				ImGui::ColorEdit4(itemName.c_str(), reinterpret_cast<float*>(ptr));
			}

			// bool型の値を保持していれば
			else if (std::holds_alternative<bool>(item)) 
			{
				bool* ptr = std::get_if<bool>(&item);
				ImGui::Checkbox(itemName.c_str(), ptr);
			}
		}

		//改行
		ImGui::Text("\n");

		//jsonファイル保存ボタン
		if (ImGui::Button("Save")) 
		{
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::TreePop();
	}

	ImGui::End();

#endif // USE_IMGUI
}

void GlobalVariables::CreateGroup(const std::string& groupName) 
{

	//指定名のオブジェクトがなければ追加する
	dates_[groupName];

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value)
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) 
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) 
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector2& value)
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector4& value)
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Color& value)
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, bool value) 
{
	SetValueInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value)
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) 
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value) 
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector2& value)
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector4& value)
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Color& value)
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, bool value)
{
	AddItemInternal(groupName, key, value);
}

void GlobalVariables::SaveFile(const std::string& groupName)
{

	//グループを検索
	std::map<std::string, Group>::iterator itGroup = dates_.find(groupName);

	//未登録チェック
	assert(itGroup != dates_.end());

	json root;

	root = json::object();

	//jsonオブジェクト登録
	root[groupName] = json::object();

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
		itItem != itGroup->second.end(); ++itItem) {

		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item)) 
		{
			//int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item);
		}

		// float型の値を保持していれば
		else if (std::holds_alternative<float>(item)) 
		{
			//floatの値を登録
			root[groupName][itemName] = std::get<float>(item);
		}

		// Vector2型の値を保持していれば
		else if (std::holds_alternative<Vector2>(item))
		{
			//float型のjson配列登録
			Vector2 value = std::get<Vector2>(item);
			root[groupName][itemName] = json::array({ value.x, value.y });
		}

		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item))
		{
			//float型のjson配列登録
			Vector3 value = std::get<Vector3>(item);
			root[groupName][itemName] = json::array({value.x, value.y, value.z});
		}

		// Vector4型の値を保持していれば
		else if (std::holds_alternative<Vector4>(item))
		{
			//float型のjson配列登録
			Vector4 value = std::get<Vector4>(item);
			root[groupName][itemName] = json::array({value.x, value.y, value.z, value.w});
		}

		// Color型の値を保持していれば
		else if (std::holds_alternative<Color>(item))
		{
			//float型のjson配列登録
			Color value = std::get<Color>(item);
			root[groupName][itemName] = json::array({value.r, value.g, value.b, value.a});
		}

		// bool型の値を保持していれば
		else if (std::holds_alternative<bool>(item)) 
		{
			// boolの値を登録
			root[groupName][itemName] = std::get<bool>(item);
		}
	}

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) 
	{
		std::filesystem::create_directory(dir);
	}

	//書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込み用ファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込み用に開く
	ofs.open(filePath);

	//ファイルオープン失敗?
	if (ofs.fail()) {
		std::string message = "Failed open data file write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	//ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();

}

void GlobalVariables::LoadFiles()
{

	// ディレクトリがなければスキップする
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir))
	{
		return;
	}

	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it)
	{
	
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.json以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		//ファイル読み込み
		LoadFile(filePath.stem().string());

	}

}

void GlobalVariables::LoadFile(const std::string& groupName)
{

	//読み込むJSONファイルのファイルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);

	//ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open data file write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();

	//グループを確認
	json::iterator itGroup = root.find(groupName);

	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem)
	{
	
		//アイテム名を取得
		const std::string& itemName = itItem.key();
	
		//int32_t型の値を保持していれば
		if (itItem->is_number_integer()) 
		{
			//int32_t型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		
		}

		// float型の値を保持していれば
		else if (itItem->is_number_float())
		{
			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}

		// 要素数2の配列であれば
		else if (itItem->is_array() && itItem->size() == 2)
		{
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(groupName, itemName, value);
		}

		// 要素数3の配列であれば
		else if (itItem->is_array() && itItem->size() == 3) 
		{
			// float型のjson配列登録
			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);
		}

		// 要素数4の配列であればColorとして登録
		else if (itItem->is_array() && itItem->size() == 4) 
		{
			// float型のjson配列登録
			Color value = {itItem->at(0), itItem->at(1), itItem->at(2), itItem->at(3)};
			SetValue(groupName, itemName, value);
		}

		// bool型の値を保持していれば
		else if (itItem->is_boolean()) 
		{
			// bool型の値を登録
			bool value = itItem->get<bool>();
			SetValue(groupName, itemName, value);
		}
	}

}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const 
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<int32_t>(outValue);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const 
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<float>(outValue);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<Vector3>(outValue);
}

Vector2 GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key) const
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<Vector2>(outValue);
}

Vector4 GlobalVariables::GetVector4Value(const std::string& groupName, const std::string& key) const
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<Vector4>(outValue);
}

Color GlobalVariables::GetColorValue(const std::string& groupName, const std::string& key) const
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<Color>(outValue);
}

bool GlobalVariables::GetBoolValue(const std::string& groupName, const std::string& key) const
{
	Item outValue;
	GetValueInternal(groupName, key, outValue);
	return std::get<bool>(outValue);
}

void GlobalVariables::SetValueInternal(const std::string& groupName, const std::string& key, const Item& value)
{
	// グループの参照を取得
	Group& group = dates_[groupName];
	// 設定した項目をstd::mapに追加
	group[key] = value;

}

void GlobalVariables::AddItemInternal(const std::string& groupName, const std::string& key, const Item& value)
{
	// 項目が未登録の場合のみ追加
	if (dates_.find(groupName)->second.find(key) == dates_.find(groupName)->second.end())
	{
		SetValueInternal(groupName, key, value);
	}
}

void GlobalVariables::GetValueInternal(const std::string& groupName, const std::string& key, Item& outValue) const
{
	assert(dates_.find(groupName) != dates_.end());
	// グループの参照を取得
	const Group& group = dates_.at(groupName);

	assert(group.find(key) != group.end());

	// 指定した項目の値を取得
	outValue = group.at(key);
}
