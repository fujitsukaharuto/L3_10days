#pragma once

#include "Game/OriginGameObject.h"

enum Gender {
	MEN,
	WOMEN
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

/// <summary>
/// 敵キャラ
/// </summary>
class BaseChara: public OriginGameObject {
public:
	BaseChara(const CharaStatus& status, const Vector3& popPos);
	virtual ~BaseChara()override = default;

	void Update()override {};

	void Draw(Material* mate = nullptr, bool is = false)override;

	void Action();

protected:

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