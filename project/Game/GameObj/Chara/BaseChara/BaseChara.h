#pragma once

#include <vector>

#include "Game/OriginGameObject.h"
#include "Game/Collider/AABBCollider.h"

enum Gender {
	MEN,
	WOMEN
};

enum class Tag {
	Friendly,
	Enemy
};

enum State {
	Search,
	Approach,
	Fight,
};

struct CharaStatus {
	std::string name;
	Gender gender;
	uint32_t power;
	uint32_t hp;
};

struct SearchCollider {
	Vector3 pos;
	float radius;
};

/// <summary>
/// 敵キャラ
/// </summary>
class BaseChara: public OriginGameObject {
public:
	BaseChara(const CharaStatus& status, const Vector3& popPos);
	virtual ~BaseChara()override = default;

	void Update()override;

	void Draw(Material* mate = nullptr, bool is = false)override;

	CharaStatus GetStatus();

	void SetTarget(BaseChara* target);

protected:
	virtual void Search() {};

	void Action();

	void CheckIsDeadTarget();

protected:

	// 索敵用コライダー
	SearchCollider collider;

	// ターゲット
	BaseChara* target_ = nullptr;

	// 
	// 変数
	// 

	// ステータス
	CharaStatus status_;

	// 状態
	State state_;

	// 最大HP
	uint32_t maxHp_;

};