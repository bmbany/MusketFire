#include "TargetFrame.h"
#include "Mouse.h"


int TargetFrame::Update(const float& deltaTime)
{
	return 0;
}

int TargetFrame::LateUpdate(const float& deltaTime)
{
	return 0;
}

void TargetFrame::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[1]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[1]);
}

void TargetFrame::AddRenderer()
{
	if(IsActive())
	{
		Engine::AddRenderGroup((int)RenderGroup::UI, this);
	}
}

void TargetFrame::OnCollision(CollisionInfo info)
{
	
}

void TargetFrame::OnCollisionExit(CollisionInfo info)
{
	_pBitFlag->OffFlag(FlagLockOn);
}

bool TargetFrame::Initialize(const Vector3& position)
{
	SetName("TargetFrame");

	//transform
	_pTransform->SetPosition(Vector3(position));
	_pTransform->SetScale(Vector3(1.3f, 1.3f, 0.f));

	//Component
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	//Texture
	_vecTextures.push_back(Engine::FindTexture(L"UI_TargetFrame"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	return true;
}

TargetFrame* TargetFrame::Create(const Vector3& position)
{
	TargetFrame* pInstance = new TargetFrame;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}

void TargetFrame::Free()
{

}

void TargetFrame::SetPosition(const Vector3& position)
{
	_pTransform->SetPosition(position);
}



void TargetFrame::SetFlag(const _ullong& flag)
{
	_pBitFlag->SetFlag(flag);
}