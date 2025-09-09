#pragma once
#include "Scene/BaseScene.h"


class ResultScene :public BaseScene {
public:
	ResultScene();
	~ResultScene();

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
	std::unique_ptr<Sprite> frame_;
	std::unique_ptr<Sprite> back_;
	std::unique_ptr<Sprite> chain_;

	std::unique_ptr<Sprite> cursorTex_;

	float goTitleTime_ = 0.0f;

	// sceneChange
	std::unique_ptr<Sprite> black_;
	float blackLimmite = 20.0f;
	float blackTime = 20.0f;
	bool isChangeFase = false;
};
