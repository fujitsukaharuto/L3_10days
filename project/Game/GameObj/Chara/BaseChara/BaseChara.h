#pragma once

#include <vector>

#include "Game/OriginGameObject.h"
#include "Game/Collider/AABBCollider.h"

enum Gender {
	MAN,
	WOMAN
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
	int32_t hp;
};

struct SearchCollider {
	Vector3 pos;
	float radius;
};

/// <summary>
/// 敵キャラ
/// </summary>
class BaseChara : public OriginGameObject {
public:
	BaseChara(const CharaStatus& status, const Vector3& popPos);
	virtual ~BaseChara()override = default;

	void Update()override;

	void Draw(Material* mate = nullptr, bool is = false)override;

	void GetDamage(int32_t damage);

	void GetHeal(int32_t heal);

	void CheckIsTargetDead();

	bool GetIsAlive()const;
	CharaStatus GetStatus();
	void SetTarget(BaseChara* target);

protected:
	virtual void Search() {};
	void Action();
protected:

	// 索敵用コライダー
	SearchCollider collider;

	// ターゲット
	BaseChara* target_ = nullptr;

	// 
	// 変数
	// 

	bool isAlive_ = true;

	// 進行方向
	Vector3 moveDir_;

	// スピード
	float speed_ = 0.01f;
	// ターゲットに近づくときのスピード
	float approachSpeed_ = 0.02f;

	// ステータス
	CharaStatus status_;

	// 状態
	State state_;

	// 最大HP
	int32_t maxHp_;

	// 戦闘距離
	float fightRange_ = 3.0f;

	// 戦闘時行動クールタイム
	const float kActionCoolTime_ = 3.0f;
	float actionCoolTimer_ = 0.0f;

};