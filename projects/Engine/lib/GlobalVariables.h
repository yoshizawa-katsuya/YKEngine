#pragma once
#include <variant>
#include <string>
#include <map>
#include <Vector3.h>
#include <json.hpp>

//グローバル変数
class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	//毎フレーム処理
	void Update();

	//グループの作成
	void CreateGroup(const std::string& groupName);

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	//値のセット(bool)
	void SetValue(const std::string& groupName, const std::string& key, bool value);
	

	//項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	// 項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	// 項目の追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
	//項目の追加(bool)
	void AddItem(const std::string& groupName, const std::string& key, bool value);

	///<summary>
	///ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);

	//値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	using json = nlohmann::json;

	//項目
	using Item = std::variant<int32_t, float, Vector3, bool>;


	//グループ
	using Group = std::map<std::string, Item>;

	//全データ
	std::map<std::string, Group> datas_;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

};
