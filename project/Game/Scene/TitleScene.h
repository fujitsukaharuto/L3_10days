#pragma once
#include "Scene/BaseScene.h"
#include "Game/TestBaseObj.h"
#include "Game/GameObj/SkyDome/SkyBox.h"
#include "Game/Collider/CollisionManager.h"

#include "Game/GameObj/Block/Mino.h"
#include "Game/GameObj/Map/MapField.h"

class TitleScene:public BaseScene {
public:
	TitleScene();
	~TitleScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DebugGUI()override;
	void ParticleDebugGUI()override;

	void BlackFade();

private:

	void ApplyGlobalVariables();//値読み込みテスト用今度Objectクラス作って継承で使えるようにする

	std::unique_ptr<Object3dCommon> obj3dCommon = nullptr;
	std::unique_ptr<SkyBox> skybox_;
	std::unique_ptr<AnimationModel> terrain_ = nullptr;

	std::unique_ptr<Mino> b1_;
	std::unique_ptr<Mino> b2_;

	std::unique_ptr<MapField> map_;

	ParticleEmitter emit;

	std::unique_ptr<CollisionManager> cMane_;
	std::unique_ptr<AABBCollider> terrainCollider_;

	// sceneChange
	std::unique_ptr<Sprite> black_;
	float blackLimmite = 20.0f;
	float blackTime = 20.0f;
	bool isChangeFase = false;
	bool isParticleDebugScene_ = false;
};
