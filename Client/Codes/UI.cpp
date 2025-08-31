#include "UI.h"
#include "BitmapRenderer.h"

#include "Client_Define.h"

Vector3 UI::GetScale()
{
	return _scale;
}

void UI::SetScale(Vector3 scale)
{
	_scale = scale;
}

void UI::AddScale(Vector3 scale)
{
	_scale += scale;
}

int UI::Update(const float& deltaTime)
{
	if (nullptr != _pTarget)
		_pTransform->SetPosition(_pTarget->GetPosition() + _position);

	_pTransform->SetScale(_scale);

	return 0;
}

int UI::LateUpdate(const float& deltaTime)
{
	return 0;
}

void UI::Render(Gdiplus::Graphics* pGraphics)
{
	if (nullptr != _pBitmapRenderer)
	{
		_pBitmapRenderer->SetDrawInformation();
		_pBitmapRenderer->Draw();
	}
	else
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_uiGdiplusInfo.index]);
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_uiGdiplusInfo.index]);
	}
}

void UI::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

bool UI::Initialize(const GDIPlus_Info& info)
{
	_uiGdiplusInfo = info;
	_pTransform->SetPosition(_uiGdiplusInfo.position);
	_pTransform->SetScale(_uiGdiplusInfo.scale);
	_vecTextures.push_back(Engine::FindTexture(_uiGdiplusInfo.textureTag));
	SetName(_uiGdiplusInfo.name);

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255));

	_position = _uiGdiplusInfo.position;
	_scale = _uiGdiplusInfo.scale;
	_pTarget = _uiGdiplusInfo.pTarget;

	return true;
}

bool UI::Initialize(const GDI_Info& info)
{
	_uiGdiInfo = info;
	_pTransform->SetPosition(_uiGdiInfo.position);
	_pTransform->SetScale(_uiGdiInfo.scale);
	SetName(_uiGdiInfo.name);

	_position = _uiGdiInfo.position;
	_scale = _uiGdiInfo.scale;
	_pTarget = _uiGdiInfo.pTarget;

	Engine::Texture* pTexture = Engine::FindTexture(info.textureTag);
	_pBitmapRenderer = AddComponent<Engine::BitmapRenderer>("BitmapRenderer");
	_pBitmapRenderer->Initialize(pTexture->GetImage(info.index, false), info.originHDC, info.backHDC, _pTransform, info.isTransparent);

	return true;
}

UI* UI::Create(GDIPlus_Info& info)
{
	UI* pInstance = new UI;
	if (pInstance->Initialize(info))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}

UI* UI::Create(GDI_Info& info)
{
	UI* pInstance = new UI;
	if (pInstance->Initialize(info))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
