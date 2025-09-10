#pragma once
#include "Scene/BaseScene.h"


class GameOverScene :public BaseScene {
public:
	GameOverScene();
	~GameOverScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DebugGUI()override;
	void ParticleDebugGUI()override;

	void BlackFade();

private:

	void ApplyGlobalVariables();//値読み込みテスト用今度Objectクラス作って継承で使えるようにする

	std::unique_ptr<Object3dCommon> obj3dCommon = nullptr;
	std::unique_ptr<Object3d> sphere = nullptr;

	std::unique_ptr<Sprite> report_;
	std::unique_ptr<Sprite> goTitle_;
	std::unique_ptr<Sprite> retry_;
	std::unique_ptr<Sprite> frame_;
	std::unique_ptr<Sprite> back_;

	std::unique_ptr<Sprite> chain_;
	std::unique_ptr<Sprite> chain2_;

	std::unique_ptr<Sprite> cursorTex_;

	bool isRetry_ = false;
	float goTitleTime_ = 0.0f;

	float goTitleReturenTime_ = 0.0f;
	bool isGoTitle_ = false;
	bool isGoRetry_ = false;

	// sceneChange
	std::unique_ptr<Sprite> black_;
	float blackLimmite = 20.0f;
	float blackTime = 20.0f;
	bool isChangeFase = false;
};
