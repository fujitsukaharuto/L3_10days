#include "BaseChara.h"

BaseChara::BaseChara(const CharaStatus& status, const Vector3& popPos) {
	// ステータスを受け取る
	status_ = status;

	// ステートを初期化
	state_ = State::Search;

	// 最大hpを取得
	maxHp_ = status_.hp;

	OriginGameObject::Initialize();
	OriginGameObject::CreateModel(status_.name);
	OriginGameObject::GetModel()->transform.translate = popPos;

}

void BaseChara::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);

}

void BaseChara::Action() {
	switch (status_.gender) {
	case MEN:

		break;
	case WOMEN:

		break;
	}
}
