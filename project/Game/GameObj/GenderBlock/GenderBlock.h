#pragma once
#include "Game/OriginGameObject.h"

/// <summary>
/// 性別
/// </summary>
enum class GenderType {
	NONE,
	DEFAULT,
	MEN,
	WOMEN
};

/// <summary>
/// 性別ブロック
/// </summary>
class GenderBlock: public OriginGameObject {
public:
	GenderBlock();
	~GenderBlock() = default;

	void SetGenderType(const GenderType& type);

	void Draw(Material* mate = nullptr, bool is = false)override;

private:
	// 現在の状態
	GenderType currentGender_ = GenderType::NONE;


};