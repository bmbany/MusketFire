#include "Ending.h"
#include "Title.h"

#include "Vfw.h"
#include "Client_Define.h"
#include "GameManager.h"

int Ending::Update(const float& deltaTime)
{
    return 0;
}

int Ending::LateUpdate(const float& deltaTime)
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

bool Ending::Initialize()
{
    Engine::GameManager::GetInstance()->SetPlayMovie(true);
    _videoHWND = MCIWndCreate(Engine::GetHWND(), nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, L"../Resources/Movie/Intro.wmv");
    MoveWindow(_videoHWND, 0, 0, WINCX, WINCY, FALSE);
    MCIWndPlay(_videoHWND);

    return true;
}

Ending* Ending::Create()
{
    return new Ending;
}

void Ending::Free()
{
    MCIWndClose(_videoHWND);
}
