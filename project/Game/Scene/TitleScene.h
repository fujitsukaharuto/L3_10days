#pragma once
#include "Scene/BaseScene.h"
#include "Game/TestBaseObj.h"
#include "Game/GameObj/SkyDome/SkyBox.h"
#include "Game/Collider/CollisionManager.h"

#include "Game/GameObj/Block/Mino.h"
#include "Game/GameObj/Map/MapField.h"

#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"
#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"

#include "GameObj/BattleSystem/BattleSystem.h"
#include "Editor/WaveEditor/WaveEditor.h"
#include "Editor/EnemyTableEditor/EnemyTableEditor.h"

#ifdef _DEBUG
#include "Game/Editor/UsableMinoEditor.h"
#endif

class TitleScene :public BaseScene {
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
	void EditorModeSwitchUI();
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

	// 戦闘システム
	std::unique_ptr<BattleSystem> battleSystem_;
	// 自軍のマネージャ
	std::unique_ptr<FriendlyManager> friendlyManager_;
	// 敵のマネージャ
	std::unique_ptr<EnemyManager> enemyManager_;

	// ウェーブ作成エディタ
	std::unique_ptr<WaveEditor> waveEditor_;

	// 敵テーブル作成エディタ
	std::unique_ptr<EnemyTableEditor> enemyTableEditor_;

	bool isEditorMode_ = false;

	// sceneChange
	std::unique_ptr<Sprite> black_;
	float blackLimmite = 20.0f;
	float blackTime = 20.0f;
	bool isChangeFase = false;
	bool isGameover_ = false;
	bool isParticleDebugScene_ = false;

#ifdef _DEBUG
	UsableMinoEditor minoEditor_;
#endif
};
