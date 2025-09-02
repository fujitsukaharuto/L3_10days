#pragma once

#include "Game/OriginGameObject.h"
#include "Game/Collider/AABBCollider.h"

#include "GameObj/Climber/FrontCollider/ClimberFrontCollider.h"
#include "GameObj/Climber/TopCollider/TopCollider.h"

enum class ClimberDir {
	LEFT,
	RIGHT
};

enum class ClimberState {
	IDLE,
	MOVE,
};

/// <summary>
/// 人間
/// </summary>
class Climber: public OriginGameObject {
public:
	Climber();
	~Climber()override = default;

	void Initialize()override;
	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;
	void DebugGUI()override;


	// 折り返し処理
	void Turn();

	// 上る処理
	void Up();

private:
	const uint32_t kBlockWidth_ = 9;
	const float kBlockSize_ = 2.0f;
	const Vector3 kStartPos_ = { 0.0f, kBlockSize_, 0.0f };

	const float leftWall_ = 0.0f;
	const float rightWall_ = kBlockSize_ * float(kBlockWidth_);

	const float speed_ = 0.1f;
	ClimberDir dir_ = ClimberDir::RIGHT;
	ClimberState state_ = ClimberState::IDLE;

	// コライダー用オブジェクト
	std::unique_ptr<ClimberFrontCollider> frontCollider_;
	std::unique_ptr<ClimberTopCollider> upCollider_;
};

// 実装メモ
// ブロック1マス分は2.0f
// マップチップフィールドの番号を使った方がスマートな実装になりそうだがひとまずコライダーでごり押し実装する
// 前方と斜め上、斜め下にコライダーを持つ