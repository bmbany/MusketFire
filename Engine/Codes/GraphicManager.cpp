#include "GraphicManager.h"

using namespace Engine;

void Engine::GraphicManager::SetUpGrahpic(const HDC& hdc)
{
	_pGraphic = Gdiplus::Graphics::FromHDC(hdc);
}

bool Engine::GraphicManager::Initialize()
{	
	Gdiplus::GdiplusStartup(&_gpToken, &_gpsi, NULL);

	return true;
}

void Engine::GraphicManager::Free()
{
	delete _pGraphic;
	Gdiplus::GdiplusShutdown(_gpToken);
}

GraphicManager* Engine::GraphicManager::Create()
{
	GraphicManager* pInstance = new GraphicManager;

	if (pInstance->Initialize())
		return pInstance;

	SafeRelease(pInstance);
	return nullptr;
}
