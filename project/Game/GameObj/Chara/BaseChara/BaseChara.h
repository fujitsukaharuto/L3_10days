#pragma once

#include "Game/OriginGameObject.h"

/// <summary>
/// 敵キャラ
/// </summary>
class BaseChara: public OriginGameObject {
public:
	BaseChara(const std::string& modelName, const Vector3& popPos);
	~BaseChara()override;

	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;

	void Attack();

protected:
	// 
	// 定数
	// 

	const float kMaxHp_ = 50.0f;
	const float kAttackPower_ = 10.0f;

	// 
	// 変数
	// 

	float hp_ = 0.0f;


};