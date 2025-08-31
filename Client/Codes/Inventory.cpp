#include "Inventory.h"

#include "BitmapRenderer.h"
#include "Client_Define.h"

int Inventory::Update(const float& deltaTime)
{
	if (Engine::IsKeyDown(DIK_END))
	{
		for (auto& Item : _inventoryItems)
			Item->SetDead();

		_inventoryItems.clear();
	}

	return 0;
}

int Inventory::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Inventory::Render(Gdiplus::Graphics* pGraphics)
{
	_pBitmapRenderer->SetDrawInformation();
	_pBitmapRenderer->Draw();
}

void Inventory::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Section, this);
}

void Inventory::OnCollision(CollisionInfo info)
{
	if (*info.other == "Mouse" || *info.other->GetOwner() == "Rammer")
		return;	

	_inventoryItems.insert(info.other->GetOwner());
}

void Inventory::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
		return;

	_inventoryItems.erase(info.other->GetOwner());
}

bool Inventory::Initialize()
{
	SetName("Inventory");
	SetActive(false);

	// Transform
	_pTransform->SetPosition(Vector3(145.f, 509.f, -2.f));

	// Collider
	Engine::Collider* pCollider = AddCollider("Inventory");
	pCollider->SetScale(Vector3(265.f, 400.f, 0.f));

	// Component
	_vecTextures.push_back(Engine::FindTexture(L"UI_Frame"));
	_pBitmapRenderer = AddComponent<Engine::BitmapRenderer>("BitmapRenderer");
	_pBitmapRenderer->Initialize((*_vecTextures[0])[0], Engine::GetOriginHDC(), Engine::GetBackHDC(), _pTransform);

	// Default
	_pMouse = dynamic_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));

	return true;
}

Inventory* Inventory::Create()
{
	Inventory* pInstance = new Inventory;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
