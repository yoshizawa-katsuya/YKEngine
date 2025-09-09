#pragma once

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	kPlayerSpawn, // プレイヤースポーン 
	kDoorTrigger, // ドアトリガー
	kDoor,       // ドア
	kTransparentBlock, // 透明ブロック
	kSpine, // トゲ
	kGoal,  // ゴール
};