#include "Intro.h"
#include "Title.h"

#include "Vfw.h"
#include "Client_Define.h"
#include "GameManager.h"

int Intro::Update(const float& deltaTime)
{
    return 0;
}

int Intro::LateUpdate(const float& deltaTime)
{
    if (MCIWndGetLength(_videoHWND) <= MCIWndGetPosition(_videoHWND))
    {
        Engine::ChagneScene(Title::Create());
        Engine::GameManager::GetInstance()->SetPlayMovie(false);
    }

    if (Engine::IsKeyDown(DIK_ESCAPE))
    {
        Engine::ChagneScene(Title::Create());
        Engine::GameManager::GetInstance()->SetPlayMovie(false);
    }

    return 0;
}

bool Intro::Initialize()
{
    Engine::GameManager::GetInstance()->SetPlayMovie(true);
    _videoHWND = MCIWndCreate(Engine::GetHWND(), nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, L"../Resources/Movie/Intro.wmv");
    MoveWindow(_videoHWND, 0, 0, WINCX, WINCY, FALSE);
    MCIWndPlay(_videoHWND);

    return true;
}

Intro* Intro::Create()
{
    return new Intro;
}

void Intro::Free()
{
    MCIWndClose(_videoHWND);
}
