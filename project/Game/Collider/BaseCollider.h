#pragma once
#include <string>
#include <functional>
#include "Math/Vector/Vector3.h"
#include "Model/Object3d.h"


class OriginGameObject;

struct ColliderInfo {
	std::string tag;
	Vector3 pos;
	Vector3 worldPos;
	OriginGameObject* owner = nullptr; //コライダーを持つオブジェクトのポインタ
};

enum class CollisionState {
	CollisionEnter,
	CollisionStay,
	CollisionExit,
	None,
};

enum class ColliderType {
	Type_None = 0,      // ビットが立っていない状態
	Type_Block = 1 << 0, // block
	Type_Player = 1 << 1, // プレイヤー
	//Type_Frame = 1 << 2, // フレーム
};

// operator
inline ColliderType operator|(ColliderType lhs, ColliderType rhs) {
	using T = std::underlying_type_t<ColliderType>;
	return static_cast<ColliderType>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
inline ColliderType& operator|=(ColliderType& lhs, ColliderType rhs) {
	lhs = lhs | rhs;
	return lhs;
}
inline ColliderType operator&(ColliderType lhs, ColliderType rhs) {
	using T = std::underlying_type_t<ColliderType>;
	return static_cast<ColliderType>(static_cast<T>(lhs) & static_cast<T>(rhs));
}
inline ColliderType& operator&=(ColliderType& lhs, ColliderType rhs) {
	lhs = lhs & rhs;
	return lhs;
}



class BaseCollider {
public:
	BaseCollider();
	virtual ~BaseCollider() = default;

	virtual void OnCollisionEnter(const ColliderInfo& other) = 0;
	virtual void OnCollisionStay(const ColliderInfo& other) = 0;
	virtual void OnCollisionExit(const ColliderInfo& other) = 0;

	virtual void SaveCollider(const std::string& filePath) = 0;

	void InfoUpdate();

	void SetParent(Trans* parent) { parent_ = parent; }
	void SetTag(const std::string& tag) { info.tag = tag; }
	void SetOffset(const Vector3& pos) { offset_ = pos; }
	void SetPos(const Vector3& pos) { info.pos = pos; }
	void SetOwner(OriginGameObject* owner) { info.owner = owner; }
	void SetIsCollisonCheck(bool is) { isCollisionCheck_ = is; }

	const std::string GetTag()const { return info.tag; }
	Vector3 GetPos();
	Vector3 GetWorldPos();
	OriginGameObject* GetOwner();
	bool GetIsCollisonCheck() { return isCollisionCheck_; }

	ColliderType GetType() const { return type_; }
	ColliderType GetTargetType() const { return targetType_; }

	std::list<BaseCollider*> hitList_;

protected:
	ColliderInfo info;
	Trans* parent_ = nullptr;
	bool isCollisionCheck_ = true;
	Vector3 offset_ = { 0.0f,0.0f,0.0f };

	ColliderType type_;       //* 自身のタイプ
	ColliderType targetType_; //* 衝突相手のタイプ

	const std::string kDirectoryPath_ = "resource/Json/Collider/";
};