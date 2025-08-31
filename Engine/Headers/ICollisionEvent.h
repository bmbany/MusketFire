#pragma once

namespace Engine
{
	class ICollisionEvent abstract
	{
	protected:
		explicit ICollisionEvent() = default;
		virtual ~ICollisionEvent() = default;

	public:
		virtual int GetDamage() = 0;
	};
}