#include "Renderer.h"
#include "GameObject.h"

using namespace Engine;

void Engine::Renderer::Render_GameObject(Gdiplus::Graphics* pGraphics)
{
    if (!_isSetUp || _isMovie)
        return;

    RECT rc;
    GetWindowRect(_info.hWnd, &rc);  
    PatBlt(_backMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, BLACKNESS);

    for (int i = 0; i < _info.size; i++)
    {
        if (nullptr != _sortInfo[i])
            _renderGroup[i].sort(_sortInfo[i]);

        for (auto& Object : _renderGroup[i])
            Object->Render(pGraphics);

        _renderGroup[i].clear();
    }

    BitBlt(_info.hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, _backMemDC, 0, 0, SRCCOPY);
}

void Engine::Renderer::SetUp_Renderer(const RenderGroupInfo& info)
{
    if (_isSetUp)
        return;

    _info = info;
    _renderGroup.resize(_info.size);
    _sortInfo.resize(_info.size);
    _backBitmap = CreateCompatibleBitmap(_info.hdc, _info.width, _info.height);
    _backMemDC = CreateCompatibleDC(_info.hdc);
    SelectObject(_backMemDC, _backBitmap);

    _isSetUp = true;
}

void Engine::Renderer::SetSortGroup(int sortGroup, bool(*sortFunc)(GameObject*, GameObject*))
{
    _sortInfo[sortGroup] = sortFunc;
}

bool Engine::Renderer::AddRenderGroup(int group, GameObject* pObject)
{    
    if (nullptr == pObject || pObject->IsDead())
        return false;

    _renderGroup[group].push_back(pObject);

    return true;
}

void Engine::Renderer::ResetRenderGroup()
{
    for (int i = 0; i < _info.size; i++)
        _renderGroup[i].clear();
}

void Engine::Renderer::Free()
{
    for (auto& Group : _renderGroup)
        Group.clear();

    _renderGroup.clear();
    _renderGroup.shrink_to_fit();

    ReleaseDC(_info.hWnd, _backMemDC);
    DeleteObject(_backBitmap);
}

Renderer* Engine::Renderer::Create()
{
    return new Renderer;
}
