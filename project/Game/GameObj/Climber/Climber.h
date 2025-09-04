#pragma once

#include "Game/OriginGameObject.h"

class MapField;

/// <summary>
/// 人間
/// </summary>
class Climber : public OriginGameObject {
public:
	Climber(MapField* mapField);
	~Climber()override = default;

	void Initialize()override;
	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;
	void DebugGUI()override;

public:
	bool CanAvoidBlock();

	void AvoidFeatureBlock();
	void OnDropped();

	void OldUp();

private:
	std::vector<int> CalcGroundBlocks();

private:
	const uint32_t kBlockWidth_ = 20;
	const float kBlockSize_ = 2.0f;
	const Vector3 kStartPos_ = { 0.0f, kBlockSize_, 0.0f };

	std::vector<Vector3> movePositions;

	const MapField* mapField_;
};

// 実装メモ
// ブロック1マス分は2.0f
// マップチップフィールドの番号を使った方がスマートな実装になりそうだがひとまずコライダーでごり押し実装する
// 前方と斜め上、斜め下にコライダーを持つ