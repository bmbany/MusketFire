#pragma once
#include "Engine_Define.h"

namespace Engine
{
	class Base abstract
	{
	protected:
		explicit Base() = default;
		virtual ~Base() = default;

	public:
		void Release()
		{
			Free();
			delete this;
		}

	protected:
		virtual void Free() = 0;
	};
}