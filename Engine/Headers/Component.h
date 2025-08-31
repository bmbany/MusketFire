#pragma once
#include "Base.h"

namespace Engine
{
	class Component : public Base
	{
	protected:
		explicit Component() = default;
		virtual ~Component() = default;

	public:
		virtual int Update(const float& deltaTime) { return 0; };
		const char* GetName() { return _name; }

	protected:
		// Base��(��) ���� ��ӵ�
		void Free() = 0;

	protected:
		const char* _name = nullptr;
	};
}