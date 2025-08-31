#include "PaperBottom.h"
#include "BitFlag.h"
#include "PaperCartidge.h"
#include "Timer.h"

int PaperBottom::Update(const float& deltaTime)
{
	SettingGravity(deltaTime);

	UpdateCartidgeDead();

	if (Engine::IsKeyPress(DIK_SPACE) && _pPaperCartidge->IsOnSplit())
	{
		_pBitFlag->OnFlag(FlagSplit);
	}
	else
	{
		UpdatePosition(deltaTime);
	}

	if (_pBitFlag->CheckFlag(FlagSplit))
	{
		if ((*_pTimer)[0].isActive == false)
			(*_pTimer)[0].isActive = true;
	}

	if ((*_pTimer)[0].elapsed > 1.f)
	{
		_pPaperCartidge->SetUseable();
		SetDead();
	}

	return 0;
}

int PaperBottom::LateUpdate(const float& deltaTime)
{

	return 0;
}

void PaperBottom::Render(Gdiplus::Graphics* pGraphics)
{
	if (_pBitFlag->CheckFlag(FlagSplit))
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
		Vector3 cameraPosition = Engine::CameraPosition();
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
	}
}

void PaperBottom::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Item, this);
}

void PaperBottom::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;
}

void PaperBottom::OnCollisionExit(CollisionInfo info)
{

}

bool PaperBottom::IsSplit() const
{
	return _pBitFlag->CheckFlag(FlagSplit);
}

void PaperBottom::UpdateCartidgeDead()
{
	if (_pPaperCartidge == nullptr)
		SetDead();
	else if (_pPaperCartidge->IsDead())
		SetDead();
}

void PaperBottom::UpdatePosition(const float& deltaTime)
{
	if (_pPaperCartidge->IsDead() || nullptr == _pPaperCartidge)
	{
		SetDead();
		return;
	}

	if (!_pBitFlag->CheckFlag(FlagSplit))
	{
		_pTransform->SetPosition(Vector3(
			_pPaperCartidge->GetTransform()->GetPosition().x - 27.f,
			_pPaperCartidge->GetTransform()->GetPosition().y, 0.f));
	}
}

bool PaperBottom::Initialize(PaperCartidge* pPaperCartidge)
{
	_pPaperCartidge = pPaperCartidge;

	SetName("PaperBottom");

	_pItemFlag = AddComponent<Engine::BitFlag>("ItemFlag");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_vecTextures.push_back(
		Engine::FindTexture(L"Item_PaperCartidge_Bottom"));

	_pTransform->SetScale(Vector3(0.7f, 0.7f, 0.f));
	_oldScale = _pTransform->GetScale();

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(1);

	_pTransform->SetPosition(Vector3(
		_pPaperCartidge->GetTransform()->GetPosition().x - 25.f,
		_pPaperCartidge->GetTransform()->GetPosition().y, 0.f));

	return true;
}

void PaperBottom::Free()
{
}

PaperBottom* PaperBottom::Create(PaperCartidge* pPaperCartidge)
{
	PaperBottom* pInstance = new PaperBottom;
	if (pInstance->Initialize(pPaperCartidge))
	{
		//std::cout << "PaperBottom Create" << std::endl;
		return pInstance;
	}

	Engine::SafeRelease(pInstance);
	return nullptr;
}
