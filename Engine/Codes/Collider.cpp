#include "Collider.h"

using namespace Engine;

_uint Collider::g_ID = 0;


void Engine::Collider::InitalizeOffset(const Vector3& offset)
{
	_orginOffset = offset;
	_currentOffset = offset;
}

void Engine::Collider::UpdateOffset(float angleDegrees)
{
	const float PI = 3.141592f;
	const float Angle2Radian = PI / 180.f;

	float angleRadians = angleDegrees * Angle2Radian;

	Vector3 newOffset;
	newOffset.x = _orginOffset.x * cos(angleRadians) - _orginOffset.y * sin(angleRadians);
	newOffset.y = _orginOffset.x * sin(angleRadians) + _orginOffset.y * cos(angleRadians);
	newOffset.z = _orginOffset.z;

	SetOffset(newOffset);
}

void Engine::Collider::DrawCollider(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 255, 0));
	Gdiplus::Point pt[4];

	pt[0] = { int(_position.x - _scale.x * 0.5f),  int(_position.y - _scale.y * 0.5f) };
	pt[1] = { int(_position.x + _scale.x * 0.5f),  int(_position.y - _scale.y * 0.5f) };
	pt[2] = { int(_position.x + _scale.x * 0.5f),  int(_position.y + _scale.y * 0.5f) };
	pt[3] = { int(_position.x - _scale.x * 0.5f),  int(_position.y + _scale.y * 0.5f) };

	pGraphics->DrawLine(&pen, pt[0], pt[1]);
	pGraphics->DrawLine(&pen, pt[1], pt[2]);
	pGraphics->DrawLine(&pen, pt[2], pt[3]);
	pGraphics->DrawLine(&pen, pt[3], pt[0]);
}

bool Engine::Collider::Initialize(GameObject* pOwner, const char* name)
{
	_pOwner = pOwner;
	_name = name;
	_isActive = true;

	return true;
}

void Engine::Collider::Free()
{
}

Collider* Engine::Collider::Create(GameObject* pOwner, const char* name)
{
	Collider* pInstance = new Collider;
	if (pInstance->Initialize(pOwner, name))
		return pInstance;

	SafeRelease(pInstance);
	return nullptr;
}
