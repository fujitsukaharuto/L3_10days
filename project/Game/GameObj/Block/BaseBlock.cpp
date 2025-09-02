#include "BaseBlock.h"

#include "Engine/Particle/ParticleManager.h"
#include "Engine/Editor/JsonSerializer.h"
#include "Engine/Math/Random/Random.h"

BaseBlock::BaseBlock() {
}

BaseBlock::~BaseBlock() {
}

void BaseBlock::Initialize() {
}

void BaseBlock::Update() {
}

void BaseBlock::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
}

void BaseBlock::DebugGUI() {
}

void BaseBlock::OnCollisionEnter([[maybe_unused]] const ColliderInfo& other) {
}

void BaseBlock::OnCollisionStay([[maybe_unused]] const ColliderInfo& other) {
}

void BaseBlock::OnCollisionExit([[maybe_unused]] const ColliderInfo& other) {
}

void BaseBlock::Debug() {
}
