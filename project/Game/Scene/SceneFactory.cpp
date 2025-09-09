#include "SceneFactory.h"
#include "Game/Scene/TitleScene.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/ResultScene.h"
#include "Game/Scene/GameOverScene.h"
#include "Game/Scene/ParticleDebugScene.h"
#include "Game/Scene/FightScene.h"

SceneFactory::SceneFactory() {
}

SceneFactory::~SceneFactory() {
}

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();

	} else if (sceneName == "GAME") {
		newScene = new GameScene();

	} else if (sceneName == "RESULT") {
		newScene = new ResultScene();

	} else if (sceneName == "PARTICLEDEBUG") {
		newScene = new ParticleDebugScene();
	} else if (sceneName == "GAMEOVER") {
		newScene = new GameOverScene();
	}

	return newScene;
}
