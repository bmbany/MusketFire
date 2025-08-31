#pragma once
#include "Base.h"

namespace Engine
{
	class Transform;
	class Timer;
	class Camera : public Base
	{
	private:
		explicit Camera() = default;
		virtual ~Camera() = default;

	public:
		void FixedUpdate();
		void Update(const float& deltaTime);
		void LateUpdate(const float& deltaTime);
		void CameraShake(float shakeTime, float shakePower);
		Vector3 CameraPosition();

	private:
		// Base을(를) 통해 상속됨
		bool Initialize();
		void Free() override;

	public:
		static Camera* Create();

	private:
		Vector3		_prevPosition;
		Transform*	_pTransform = nullptr;
		Timer*		_pTimer = nullptr;
		float		_shakeTime = 0.f;
		float		_shakePower = 0.f;
	};
}