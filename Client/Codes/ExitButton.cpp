#include "ExitButton.h"

#include "Client_Define.h"

int ExitButton::Update(const float& deltaTime)
{
	if (_isExit)
	{
		//std::cout << "dfasd" << std::endl;
		return -5;
	}

	return 0;
}

int ExitButton::LateUpdate(const float& deltaTime)
{
	return 0;
}

void ExitButton::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void ExitButton::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void ExitButton::OnCollision(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_index = 1;
		if (Engine::IsKeyDown(Input::DIM_LB))
		{
			_isExit = true;
			//exit(0);
		}
	}
}

void ExitButton::OnCollisionExit(CollisionInfo info)
{
	_index = 0;
}

bool ExitButton::Initialize()
{
	//Transform
	_pTransform->SetPosition(Vector3(640.f, 600.f, -2.f));

	//Texture
	_vecTextures.push_back(Engine::FindTexture(L"UI_Button_exit"));

	// Collider
	Engine::Collider* pCollider = AddCollider("ExitButton");
	pCollider->SetScale(Vector3(120.f, 60.f, 0.f));

	return true;
}

ExitButton* ExitButton::Create()
{
	ExitButton* pInstance = new ExitButton;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
