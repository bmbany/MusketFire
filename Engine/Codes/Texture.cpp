#include "Texture.h"

using namespace Engine;

void Engine::Texture::Free()
{
	for (auto& Bitmap : _vecOriginBitmap)
		delete Bitmap;

	_vecOriginBitmap.clear();
	_vecOriginBitmap.shrink_to_fit();

	for (auto& Bitmap : _vecFlipBitmap)
		delete Bitmap;

	_vecFlipBitmap.clear();
	_vecFlipBitmap.shrink_to_fit();
}

void Engine::Texture::AddImage(Gdiplus::Bitmap* pBitmap)
{ 
	_vecOriginBitmap.push_back(pBitmap);
}

void Engine::Texture::AddFlipImage(Gdiplus::Bitmap* pBitmap)
{
	pBitmap->RotateFlip(Gdiplus::Rotate180FlipY);
	_vecFlipBitmap.push_back(pBitmap);
}

Texture* Engine::Texture::Create()
{
	return new Texture;
}
