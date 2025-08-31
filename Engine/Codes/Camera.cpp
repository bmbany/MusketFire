#include "Camera.h"
#include "Transform.h"
#include "Timer.h"

using namespace Engine;

void Engine::Camera::FixedUpdate()
{	
	if ((*_pTimer)[0].isActive)
	{
		float radian = 3.141592f / 180 * (rand() % 360);
		Vector3 shakePosition = { 0.f, 1.f, 0.f };
		shakePosition.RotationZ(radian);
		_pTransform->SetPosition(shakePosition * _shakePower);
		_shakePower *= 0.95f;
	}
}

void Engine::Camera::Update(const float& deltaTime)
{
	_pTimer->Update(deltaTime);	
}

void Engine::Camera::LateUpdate(const float& deltaTime)
{
	if (_pTimer->IsOverTime(0, _shakeTime))
	{
		_shakePower = 0.f;
		_shakeTime = 0.f;
		_pTimer->SetActive(0, false);
		_pTransform->SetPosition(_prevPosition);
	}
}

void Engine::Camera::CameraShake(float shakeTime, float shakePower)
{
	if ((*_pTimer)[0].isActive)
		return;

	_shakeTime = shakeTime;
	_shakePower = shakePower;
	_pTimer->SetActive(0, true);
	_prevPosition = _pTransform->GetPosition();
}

Vector3 Engine::Camera::CameraPosition()
{
	return _pTransform->GetPosition();
}

bool Engine::Camera::Initialize()
{
	_pTransform = Transform::Create("Transform");
	_pTimer = Timer::Create("Timer");
	_pTimer->AddTimer(1);

	return true;
}

void Engine::Camera::Free()
{
	SafeRelease(_pTransform);
	SafeRelease(_pTimer);
}

Camera* Engine::Camera::Create()
{
	Camera* pInstance = new Camera;
	if (pInstance->Initialize())
		return pInstance;

	SafeRelease(pInstance);
	return nullptr;
}
