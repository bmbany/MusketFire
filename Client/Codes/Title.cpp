#include "Title.h"

// Object
#include "Mouse.h"

// UI
#include "StartButton.h"
#include "ExitButton.h"
#include "ControlsButton.h"
#include "UI.h"


// Scene
#include "BattleField.h"

#include "CollisionManager.h"
#include "Client_Define.h"

int Title::Update(const float& deltaTime)
{
    if (_pStartButton->IsSceneChange())
    {
        Engine::ChagneScene(BattleField::Create());
        Engine::StopSound((int)SoundGroup::BGM);
    }

    return 0;
}

int Title::LateUpdate(const float& deltaTime)
{
    _pCollisionMgr->CheckCollision(
        Engine::GetObjectList((int)LayerGroup::UI, "UI"),
        Engine::GetObjectList((int)LayerGroup::UI, "UI"));

    return 0;
}

bool Title::Initialize()
{
    _pCollisionMgr = Engine::CollisionManager::Create();

    UI::GDI_Info info;
    info.textureTag = L"UI_Title";
    info.position = { 640.f, 360.f, 0.f };
    info.scale = { 1.f, 1.f, 0.f };
    info.index = 0;
    info.name = "Title";
    info.originHDC = Engine::GetOriginHDC();
    info.backHDC = Engine::GetBackHDC();

    Engine::AddObjectInLayer((int)LayerGroup::UI, "BackGround", UI::Create(info));

    UI::GDIPlus_Info gdiplus_info;
    gdiplus_info.textureTag = L"UI_Title";
    gdiplus_info.position = { 640.f, 220.f, -1.f };
    gdiplus_info.scale = { 1.f, 1.f, 0.f };
    gdiplus_info.index = 1;
    gdiplus_info.name = "Title";

    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", UI::Create(gdiplus_info));

    _pStartButton = StartButton::Create();
    _pExitButton = ExitButton::Create();
	_pControlsButton = ControlsButton::Create();

    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pStartButton);
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pControlsButton);
    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pExitButton);
    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", Mouse::Create());

    Engine::MyPlaySound("BGM_BGM_Title", (int)SoundGroup::BGM, true);

    return true;
}

void Title::Free()
{
    Engine::SafeRelease(_pCollisionMgr);    
}

Title* Title::Create()
{
    return new Title;
}
