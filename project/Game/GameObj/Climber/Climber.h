#pragma once

#include "Game/OriginGameObject.h"
#include "Game/Collider/AABBCollider.h"

#include "GameObj/Climber/FrontCollider/ClimberFrontCollider.h"
#include "GameObj/Climber/FrontTopCollider/FrontTopCollider.h"
#include "GameObj/Climber/BottomCollider/ClimberBottomCollider.h"
#include "GameObj/Climber/FrontBottomCollider/FrontBottomCollider.h"

enum class ClimberDir {
	LEFT,
	RIGHT
};

enum class ClimberState {
	IDLE,
	MOVE,
};

class CollisionManager;

/// <summary>
/// 人間
/// </summary>
class Climber: public OriginGameObject {
public:
	Climber(CollisionManager* cMana);
	~Climber()override = default;

	void Initialize()override;
	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;
	void DebugGUI()override;


	// 折り返し処理
	void Turn();

	// 上る処理
	void Up();

	// 上るリクエスト
	void ColFront();
	void ThereFrontUpBlock();
	void ThereBottomBlock();
	void ThereFrontBottomBlock();

private:
	const uint32_t kBlockWidth_ = 20;
	const float kBlockSize_ = 2.0f;
	const Vector3 kStartPos_ = { 0.0f, kBlockSize_, 0.0f };

	const float leftWall_ = 0.0f;
	const float rightWall_ = kBlockSize_ * float(kBlockWidth_);

	const float speed_ = 0.1f;
	ClimberDir dir_ = ClimberDir::RIGHT;
	ClimberState state_ = ClimberState::IDLE;

	bool isColFront_ = false;
	bool isThereFrontUpBlock_ = false;
	bool isThereBottomBlock_ = false;
	bool isThereFrontBottomBlock_ = false;

	// コライダー用オブジェクト
	std::unique_ptr<ClimberFrontCollider> frontCollider_;
	std::unique_ptr<ClimberFrontTopCollider> upCollider_;
	std::unique_ptr<ClimberBottomCollider> bottomCollider_;
	std::unique_ptr<ClimberFrontBottomCollider> frontBottomCollider_;

	CollisionManager* cMana_;
};

// 実装メモ
// ブロック1マス分は2.0f
// マップチップフィールドの番号を使った方がスマートな実装になりそうだがひとまずコライダーでごり押し実装する
// 前方と斜め上、斜め下にコライダーを持つ