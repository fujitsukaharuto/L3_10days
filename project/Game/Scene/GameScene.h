#pragma once
#include "Scene/BaseScene.h"
#include "Engine/Model/AnimationData/AnimationModel.h"
#include "Game/GameObj/SkyDome/SkyBox.h"
#include "Game/GameObj/FollowCamera.h"
#include "Game/TestBaseObj.h"
#include "Game/Collider/CollisionManager.h"


class GameScene :public BaseScene {
public:
	GameScene();
	~GameScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DebugGUI()override;
	void ParticleDebugGUI()override;

	void BlackFade();
	void LoadSceneLevelData(const std::string& name)override;

private:

	void ApplyGlobalVariables();//値読み込みテスト用今度Objectクラス作って継承で使えるようにする

	std::unique_ptr<Object3dCommon> obj3dCommon = nullptr;
	std::unique_ptr<CollisionManager> cMane_;

	std::unique_ptr<AnimationModel> terrain = nullptr;
	std::unique_ptr<SkyBox> skybox_;




	bool isBackTitle_ = false;

	// sceneChange
	std::unique_ptr<Sprite> black_;
	float blackLimmite = 20.0f;
	float blackTime = 20.0f;
	bool isChangeFase = false;
};
