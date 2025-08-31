#pragma once

#include "Base.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Collider.h"

namespace Engine
{	
	class GameObject abstract : public Base
	{
	protected:
		explicit GameObject();
		virtual ~GameObject();

	public:
		virtual void FixedUpdate() {}
		virtual int Update(const float& deltaTime) = 0;
		virtual int LateUpdate(const float& deltaTime) = 0;
		virtual void Render(Gdiplus::Graphics* pGraphics) = 0;
		virtual void AddRenderer() = 0;
		virtual void OnCollision(CollisionInfo info) {}
		virtual void OnCollisionEnter(CollisionInfo info) {}
		virtual void OnCollisionExit(CollisionInfo info) {}
		void UpdateComponent(const float& deltaTime);

	public:
		bool operator==(const char* str)
		{
			if (nullptr == _name)  return false;
			return !strcmp(_name, str);
		}
		bool operator!=(const char* str)
		{ 
			if (nullptr == _name)  return false;
			return strcmp(_name, str); 
		}
		Transform* GetTransform() { return _pTransform; }
		std::list<Collider*>& GetColliders() { return _listColliders; }
		Collider* GetCollider(const char* name) { return FindCollider(name); }
		const char* GetName() { return _name; }
		bool IsDead() const { return _isDead; }
		bool IsActive() const { return _isActive; }

		void SetDead() { _isDead = true; }
		void SetName(const char* name) { _name = name; }
		void SetActive(bool isActive) { _isActive = isActive; }

	protected:
		Collider* AddCollider(const char* name);
		Collider* FindCollider(const char* name);
		template<typename T>
		T* AddComponent(const char* name)
		{
			T* pComponent = T::Create(name);
			_listComponents.push_back(pComponent);

			return pComponent;
		}

		template<typename T>
		T* GetComponent(const char* name)
		{
			for (auto& iter : _listComponents)
			{
				if (!strcmp(iter->GetName(), name))
					return static_cast<T*>(iter);
			}

			return nullptr;
		}
		void DrawCollider(Gdiplus::Graphics* pGraphics);
	
	private:
		std::list<Component*>	_listComponents;
		std::list<Collider*>	_listColliders;
		const char*				_name = nullptr;
		bool					_isDead = false;
		bool					_isActive = true;

	protected:
		std::vector<Texture*>	_vecTextures;
		Transform*				_pTransform	= nullptr;
		SpriteRenderer*			_pSpriteRenderer = nullptr;

	protected:
		virtual void Free();
	};
}