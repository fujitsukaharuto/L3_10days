#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>

#include <variant>
#include <json.hpp>

/// <summary>
/// ウェーブのデータ
/// </summary>
struct WaveData {
	// 出てくる敵のサイクルコンテナ
	std::vector<int32_t> enemyPopCycle_;
	// 敵が出てくる間隔
	float enemyPopCoolTime_;
};

/// <summary>
/// ウェーブ処理作成エディター
/// </summary>
class WaveEditor {
public:
	WaveEditor();
	~WaveEditor() = default;

	void Draw();

	std::vector<WaveData> GetData();
private:
	void LoadFile();
	void SaveFile();

private:

	// ゲームで使うデータ
	std::vector<WaveData> datas_;
};