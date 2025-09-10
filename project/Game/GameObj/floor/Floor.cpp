#include "Floor.h"

Floor::Floor() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");
	OriginGameObject::GetModel()->transform.translate = { 35.0f,6.0f,-62.0f };
	OriginGameObject::GetModel()->transform.scale = { 17.84f,2.16f,5.0f };
}

void Floor::Update() {

}

void Floor::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);
}

void Floor::DebugDraw() {
	Vector3 temp;
	Vector3 tempScale;
	temp = OriginGameObject::GetModel()->transform.translate;
	tempScale = OriginGameObject::GetModel()->transform.scale;
#ifdef _DEBUG
	ImGui::Begin("Floor");
	ImGui::DragFloat3("Pos", &temp.x, 0.01f);
	ImGui::DragFloat3("Scale", &tempScale.x, 0.01f);

	OriginGameObject::GetModel()->transform.translate = temp;
	OriginGameObject::GetModel()->transform.scale = tempScale;

	ImGui::End();
#endif // _DEBUG
}
