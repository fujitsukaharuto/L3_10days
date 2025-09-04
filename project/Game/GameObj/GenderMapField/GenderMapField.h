#pragma once

// C++
#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "GameObj/GenderBlock/GenderBlock.h"

/// <summary>
/// 性別マップ
/// </summary>
class GenderMapField {
public:
	GenderMapField();
	~GenderMapField() = default;

	void Update();
	void Draw();

	// マップをCSVファイルからロード
	void LoadMapFromCSV(const std::string& fileName);

	// マップ情報をブロックに適用
	void ApplyMapDataToBlock();

	// マップを取得
	const std::vector<std::vector<GenderType>>& GetMap();

private:
	//
	// マップ
	//

	// マップ本体
	std::vector<std::vector<GenderType>> map_;
	// ブロック
	std::vector<std::vector<GenderBlock>> blocks_;

	// 高さ
	const uint32_t kMapHeight_ = 20;
	// 幅
	const uint32_t kMapWidth_ = 20;


};

//
// 実装メモ
//


// 以下最低限必要なこと

// マップの指定したインデックスのタイプを取得できるようにする
// CSVデータからマップを読み込めるようにする


// 描画についての仕様

// 男女ブロックは背景描画のようなイメージ →　階段ブロックが重なったら見えなくなってしまっても構わない
