#pragma once
#include "Base.h"

namespace Engine
{
	class GameObject;
	class Layer : public Base
	{
	private:
		using ObjectData = std::map<const char*, std::list<GameObject*>>;

	private:
		explicit Layer() = default;
		virtual ~Layer() = default;

	public:
		void FixUpdate();
		int Update(const float& deltaTime);
		int LateUpdate(const float& deltaTime);
		void AddRenderer();

		void ClearObjectList(const char* listTag);
		void ClearAllObjectList();
		std::list<GameObject*>* GetObjectList(const char* listTag) { return &_objectData[listTag]; }
		GameObject* MyGetObject(const char* listTag, const char* objectTag);
		bool AddObject(const char* listTag, GameObject* pObject);

	private:
		void Free() override;

	private:
		ObjectData _objectData;

	public:
		static Layer* Create();
	};
}