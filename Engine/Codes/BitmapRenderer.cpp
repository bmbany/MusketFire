#include "BitmapRenderer.h"
#include "Transform.h"

using namespace Engine;

void Engine::BitmapRenderer::Initialize(Gdiplus::Bitmap* pBitmap, HDC originHDC, HDC targetHDC, Transform* pTarget, bool isTransparent)
{
	_pTransform = pTarget;
	_targetHDC = targetHDC;
	_isTransparent = isTransparent;

	pBitmap->GetHBITMAP(Gdiplus::Color(255, 0, 255), &_hBitmap);
	_hDC = CreateCompatibleDC(originHDC);

	HBITMAP preBitmap = (HBITMAP)SelectObject(_hDC, _hBitmap);
	DeleteObject(preBitmap);

	//∫Ò∆Æ∏  ¡§∫∏
	GetObject(_hBitmap, sizeof(BITMAP), &_bitmap);
}

void Engine::BitmapRenderer::Initialize(HDC targetHDC, Transform* pTarget, bool isTransparent)
{
	_pTransform = pTarget;
	_targetHDC = targetHDC;
	_isTransparent = isTransparent;
}

void Engine::BitmapRenderer::AddBitmap(Gdiplus::Bitmap* pBitmap, HDC originHDC)
{
	BitmapInfo info;
	pBitmap->GetHBITMAP(Gdiplus::Color(255, 0, 255), &_hBitmap);
	info.hdc = CreateCompatibleDC(originHDC);

	HBITMAP preBitmap = (HBITMAP)SelectObject(info.hdc, _hBitmap);
	DeleteObject(preBitmap);

	//∫Ò∆Æ∏  ¡§∫∏
	GetObject(_hBitmap, sizeof(BITMAP), &info.bitamp);

	_bitmapInfo.push_back(info);
}

void Engine::BitmapRenderer::SetDrawInformation(const Vector3& cameraPosition)
{
	int offsetWidth = static_cast<int>(_pTransform->GetScale().x * (_bitmap.bmWidth >> 1));
	int offsetHeight = static_cast<int>(_pTransform->GetScale().y * (_bitmap.bmHeight >> 1));

	_drawRect.X = static_cast<INT>(_pTransform->GetPosition().x - offsetWidth - cameraPosition.x);
	_drawRect.Y = static_cast<INT>(_pTransform->GetPosition().y - offsetHeight - cameraPosition.y);
	_drawRect.Width = offsetWidth << 1;
	_drawRect.Height = offsetHeight << 1;
}

void Engine::BitmapRenderer::SetDrawInformation(int index, const Vector3& cameraPosition)
{
	int offsetWidth = static_cast<int>(_pTransform->GetScale().x * (_bitmapInfo[index].bitamp.bmWidth >> 1));
	int offsetHeight = static_cast<int>(_pTransform->GetScale().y * (_bitmapInfo[index].bitamp.bmHeight >> 1));

	_drawRect.X = static_cast<INT>(_pTransform->GetPosition().x - offsetWidth - cameraPosition.x);
	_drawRect.Y = static_cast<INT>(_pTransform->GetPosition().y - offsetHeight - cameraPosition.y);
	_drawRect.Width = offsetWidth << 1;
	_drawRect.Height = offsetHeight << 1;
}

void Engine::BitmapRenderer::Draw()
{
	if (_isTransparent)
	{
		GdiTransparentBlt
		(_targetHDC,
			_drawRect.X, _drawRect.Y,
			_drawRect.Width, _drawRect.Height,
			_hDC,
			0, 0,
			_drawRect.Width, _drawRect.Height, RGB(255, 0, 255)
		);
	}
	else
	{
		BitBlt(_targetHDC, _drawRect.X, _drawRect.Y, _drawRect.Width, _drawRect.Height, _hDC, 0, 0, SRCCOPY);
	}
}

void Engine::BitmapRenderer::Draw(int index)
{
	if (_isTransparent)
	{
		GdiTransparentBlt
		(_targetHDC,
			_drawRect.X, _drawRect.Y,
			_drawRect.Width, _drawRect.Height,
			_bitmapInfo[index].hdc,
			0, 0,
			_drawRect.Width, _drawRect.Height, RGB(255, 0, 255)
		);
	}
	else
	{
		BitBlt(_targetHDC, _drawRect.X, _drawRect.Y, _drawRect.Width, _drawRect.Height, _bitmapInfo[index].hdc, 0, 0, SRCCOPY);
	}
}

void Engine::BitmapRenderer::Free()
{
	for (auto& Bitmap : _bitmapInfo)
		DeleteObject(Bitmap.hdc);

	DeleteObject(_hBitmap);
	DeleteObject(_hDC);
}

BitmapRenderer* Engine::BitmapRenderer::Create(const char* name)
{
	return new BitmapRenderer(name);
}
