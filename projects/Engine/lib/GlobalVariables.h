#pragma once
#include <variant>
#include <string>
#include <map>
#include <Vector3.h>
#include "Vector2.h"
#include <json.hpp>
#include "Vector4.h"
#include "Color.h"

namespace YKEngine
{

/// <summary>
/// グローバル変数管理クラス。
/// グループごとに変数を管理し、ファイルに保存・読み込みが可能。
/// ファイル形式はJSON。
/// </summary>
class GlobalVariables {
public:
	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>グローバル変数管理クラスのインスタンス</returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成。
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 値のセット(int)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット(float)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット(Vector3)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 値のセット(Vector2)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);

	/// <summary>
	/// 値のセット(Vector4)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector4& value);

	/// <summary>
	/// 値のセット(Color)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Color& value);

	/// <summary>
	/// 値のセット(bool)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, bool value);
	

	/// <summary>
	/// 項目の追加(int)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value = 0);
	
	/// <summary>
	/// 項目の追加(float)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, float value = 0.0f);
	
	/// <summary>
	/// 項目の追加(Vector3)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value = { 0.0f, 0.0f, 0.0f });
	
	/// <summary>
	/// 項目の追加(Vector2)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value = { 0.0f, 0.0f });

	/// <summary>
	/// 項目の追加(Vector4)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector4& value = { 0.0f, 0.0f, 0.0f, 0.0f });
	
	/// <summary>
	/// 項目の追加(Color)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Color& value = { 0.0f, 0.0f, 0.0f, 0.0f });
	
	/// <summary>
	/// 項目の追加(bool)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, bool value = false);

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

	/// <summary>
	/// 値の取得(int)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(float)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	float GetFloatValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(Vector3)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(Vector2)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(Vector4)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	Vector4 GetVector4Value(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(Color)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	Color GetColorValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(bool)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <returns>値</returns>
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;

private:

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	using json = nlohmann::json;

	//TODO: jsonの項目にtypeを追加して型安全にする
	//項目
	using Item = std::variant<int32_t, float, Vector2, Vector3, Vector4, Color, bool>;

	//グループ
	using Group = std::map<std::string, Item>;

	/// <summary>
	/// 値の内部セット処理。
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void SetValueInternal(const std::string& groupName, const std::string& key, const Item& value);

	/// <summary>
	/// 項目の内部追加処理。
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー名</param>
	/// <param name="value">値</param>
	void AddItemInternal(const std::string& groupName, const std::string& key, const Item& value);

	void GetValueInternal(const std::string& groupName, const std::string& key, Item& outValue) const;

	//全データ
	std::map<std::string, Group> dates_;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

};

} // namespace YKEngine