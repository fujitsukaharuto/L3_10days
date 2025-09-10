#pragma once
#include "Scene/BaseScene.h"
#include "Engine/Model/AnimationData/AnimationModel.h"
#include "Game/GameObj/SkyDome/SkyBox.h"
#include "Game/GameObj/FollowCamera.h"
#include "Game/TestBaseObj.h"
#include "Game/Collider/CollisionManager.h"
#include "Game/GameObj/Map/MapField.h"
#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"
#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"

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

	std::unique_ptr<AnimationModel> ni_ = nullptr;
	std::unique_ptr<AnimationModel> nn_ = nullptr;
	std::unique_ptr<AnimationModel> ge_ = nullptr;
	std::unique_ptr<AnimationModel> nn2_ = nullptr;
	std::unique_ptr<AnimationModel> factory_ = nullptr;
	//std::unique_ptr<Object3d> title = nullptr;

	std::unique_ptr<FriendlyManager> friendlyManager_ = nullptr;
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	Vector3 tPos;

	std::unique_ptr<MapField> map_;

	bool isBackTitle_ = false;

	// sceneChange
	std::unique_ptr<Sprite> black_;
	float blackLimmite = 20.0f;
	float blackTime = 20.0f;
	bool isChangeFase = false;
};
