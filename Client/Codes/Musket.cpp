#include "Musket.h"
#include "MusketObject.h"
#include "Mouse.h"

#include "BitmapRenderer.h"
#include "Client_Define.h"

int Musket::Update(const float& deltaTime)
{
	return 0;
}

int Musket::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Musket::Render(Gdiplus::Graphics* pGraphics)
{
	_pBitmapRenderer->SetDrawInformation();
	_pBitmapRenderer->Draw();

	/*for (auto& Collider : _listColliders)
	{
		Collider->DrawCollider(pGraphics);
	}*/
}

void Musket::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Section, this);
}

void Musket::OnCollision(CollisionInfo info)
{
	for (auto& Object : _listMusket)
		Object->SetActive(true);

	if (*info.other == "Mouse")
	{
		SetActive(true);
	}
}

void Musket::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		SetActive(false);
	}
}

bool Musket::Initailize()
{
	SetName("Musket");

	// Transform
	_pTransform->SetPosition(Vector3(640.f, 509.f, -1.f));

	// Collider
	Engine::Collider* pCollider = AddCollider("Musket");
	pCollider->SetScale(Vector3(990.f, 400.f, 0.f));
	pCollider->SetOffset(Vector3(132.5f, 0.f, 0.f));

	// Component
	_vecTextures.push_back(Engine::FindTexture(L"UI_Frame"));
	_pBitmapRenderer = AddComponent <Engine::BitmapRenderer>("BitmapRenderer");
	_pBitmapRenderer->Initialize((*_vecTextures[0])[2], Engine::GetOriginHDC(), Engine::GetBackHDC(), _pTransform);

	_pMouse = dynamic_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));

	const char* MuscketObject[] = { "Flintlock", "CockPin", "FlashHole" };

	auto objectlist = Engine::GetObjectList((int)LayerGroup::Object, "Musket");

	auto isTargetName = [&](Engine::GameObject* object)
		{
			return std::any_of(std::begin(MuscketObject),
				std::end(MuscketObject), [&](const char* name)
				{
					return (*object) == name;
				});
		};

	std::for_each(objectlist->begin(), objectlist->end(), [&](Engine::GameObject* object)
		{
			if (isTargetName(object))
				_listMusket.push_back(dynamic_cast<MusketObject*>(object));
		});

	for (auto& Object : _listMusket)
		Object->SetActive(true);

	return true;
}

Musket* Musket::Create()
{
	Musket* pInstance = new Musket;
	if (pInstance->Initailize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
