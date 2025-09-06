#pragma once

#include "Scene/BaseScene.h"

class FightScene:public	BaseScene {
public:
	FightScene();
	~FightScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	std::unique_ptr<Object3dCommon> obj3dCommon = nullptr;

};