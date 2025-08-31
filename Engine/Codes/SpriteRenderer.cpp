#include "SpriteRenderer.h"
#include "Transform.h"

using namespace Engine;

void Engine::SpriteRenderer::SetTargetTransform(Transform* pTarget)
{
	_pTransform = pTarget;
}

void Engine::SpriteRenderer::SetDrawInformation(Gdiplus::Bitmap* pBitmap, const Vector3& cameraPosition)
{
	if (nullptr == pBitmap)
		return;

	int offsetWidth = static_cast<int>(_pTransform->GetScale().x * (pBitmap->GetWidth() >> 1));
	int offsetHeight = static_cast<int>(_pTransform->GetScale().y * (pBitmap->GetHeight() >> 1));

	_drawRect.X = _pTransform->GetPosition().x - offsetWidth - cameraPosition.x;
	_drawRect.Y = _pTransform->GetPosition().y - offsetHeight - cameraPosition.y;
	_drawRect.Width = offsetWidth << 1;
	_drawRect.Height = offsetHeight << 1;
}

void Engine::SpriteRenderer::SetRotate(const float& angle)
{
	// 중심 위치 계산
	float centerX = float(_drawRect.X + _drawRect.Width * 0.5f);
	float centerY = float(_drawRect.Y + _drawRect.Height * 0.5f);

	// 변환 설정
	_rotateMatrix.Reset();
	_rotateMatrix.Translate(centerX, centerY);
	_rotateMatrix.Rotate(angle);
	_rotateMatrix.Translate(-centerX, -centerY);
}

void Engine::SpriteRenderer::SetColorKey(Gdiplus::Color lowColor, Gdiplus::Color highColor)
{
	_imageAttributes.SetColorKey(lowColor, highColor, Gdiplus::ColorAdjustTypeBitmap);
}

void Engine::SpriteRenderer::SetColorMatrix(int row, int column, float value)
{
	_colorMatrix.m[row][column] += value;

	if (1.f < _colorMatrix.m[row][column]) _colorMatrix.m[row][column] = 1.f;
	if (0.f > _colorMatrix.m[row][column]) _colorMatrix.m[row][column] = 0.f;

	_imageAttributes.SetColorMatrix(&_colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);
}

void Engine::SpriteRenderer::Draw(Gdiplus::Graphics* pGraphic, Gdiplus::Bitmap* pBitmap)
{
	pGraphic->DrawImage(pBitmap, _drawRect, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), Gdiplus::UnitPixel, &_imageAttributes);
}

void Engine::SpriteRenderer::SetTrasnform(Gdiplus::Graphics* pGraphic)
{
	pGraphic->SetTransform(&_rotateMatrix);
}

void Engine::SpriteRenderer::ResetTransform(Gdiplus::Graphics* pGraphic)
{
	pGraphic->ResetTransform();
}

void Engine::SpriteRenderer::Free()
{
}

SpriteRenderer* Engine::SpriteRenderer::Create(const char* name)
{
	return new SpriteRenderer(name);
}
