#include "GameObject.h"

using namespace Engine;

Collider* Engine::GameObject::FindCollider(const char* name)
{
	for (auto& Collider : _listColliders)
	{
		if (!strcmp(Collider->_name, name))
			return Collider;
	}

	return nullptr;
}

void Engine::GameObject::DrawCollider(Gdiplus::Graphics* pGraphics)
{
#ifdef _DEBUG
	for (auto& Collider : _listColliders)
		Collider->DrawCollider(pGraphics);
#endif
}

void Engine::GameObject::Free()
{
	for (auto& component : _listComponents)
		SafeRelease(component);

	for (auto& collider : _listColliders)
		SafeRelease(collider);
	
	_vecTextures.clear();
	_vecTextures.shrink_to_fit();
	_listComponents.clear();
	_listColliders.clear();
}

Engine::GameObject::GameObject()
{
	_pTransform = AddComponent<Transform>("Transform");
	_pSpriteRenderer = AddComponent<SpriteRenderer>("SpriteRenderer");
	_pSpriteRenderer->SetTargetTransform(_pTransform);
}

Engine::GameObject::~GameObject()
{
	Free();
}

void Engine::GameObject::UpdateComponent(const float& deltaTime)
{
	for (auto& Component : _listComponents)
		Component->Update(deltaTime);

	for (auto& Collider : _listColliders)
		Collider->_position = _pTransform->GetPosition() + Collider->_currentOffset;
}

Collider* Engine::GameObject::AddCollider(const char* name)
{
	Collider* pCollider = Collider::Create(this, name);
	_listColliders.push_back(pCollider);

	return pCollider;
}
