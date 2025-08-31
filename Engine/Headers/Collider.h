#pragma once
#include "Component.h"

namespace Engine
{
	class ICollisionEvent;
	class GameObject;
	class Collider : public Component
	{
		friend GameObject;	
	private:
		explicit Collider() : _ID(g_ID++) {}
		virtual ~Collider() = default;

	public:
		GameObject* GetOwner() { return _pOwner; }
		ICollisionEvent* GetCollisionEvent() { return _pCollisionEvent; }
		Vector3 GetScale() const { return _scale; }
		Vector3 GetPosition() const { return _position; }
		bool GetActive() const { return _isActive; }
		_uint GetID() const { return _ID; }		

		void SetScale(const Vector3& scale) { _scale = scale; }
		void InitalizeOffset(const Vector3& offset);
		void SetOffset(const Vector3& offset) { _currentOffset = offset; }
		void UpdateOffset(float angleDegrees);
		Vector3 GetOffset() const { return _currentOffset; }
		void SetActive(bool isActive) { _isActive = isActive; }
		void BindCollisionEvent(ICollisionEvent* pEvent) { _pCollisionEvent = pEvent; }
		void DrawCollider(Gdiplus::Graphics* pGraphics);

		bool operator==(const char* str) { return !strcmp(_name, str); }
		bool operator!=(const char* str) { return strcmp(_name, str); }

	private:
		bool Initialize(GameObject* pOwner, const char* name);
		void Free() override;

	public:
		static Collider* Create(GameObject* pOwner, const char* name);

	private:
		Vector3				_scale;
		Vector3				_position;
		Vector3				_orginOffset;
		Vector3				_currentOffset;

		GameObject*			_pOwner			 = nullptr;
		ICollisionEvent*	_pCollisionEvent = nullptr;		
		_uint				_ID				 = 0;
		bool				_isActive		 = true;

		static _uint		g_ID;

	};
}
