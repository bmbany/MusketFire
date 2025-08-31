#include "GameManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "Layer.h"
#include "Renderer.h"
#include "Camera.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "GraphicManager.h"
#include "CollisionManager.h"

using namespace Engine;

Engine::GameManager::GameManager()
{
    _pInputMgr = InputManager::Create();
    _pTimeMgr = TimeManager::Create();
    _pTextureMgr = TextureManager::Create();
    _pSoundMgr = SoundManager::Create();
    _pGrahpicMgr = GraphicManager::Create();
    _pRenderer = Renderer::Create();
    _pCamera = Camera::Create();
}

void Engine::GameManager::FixedUpdateGame(int count)
{
    if (!_isSetUp)
        return;

    if (_isSceneChange)
        return;    

    _elapsed += _pTimeMgr->GetDeltaTime();

    float fixed = 1.f / count;
    
    if (_elapsed >= fixed)
    {
        for (auto& Layer : _vecLayer)
            Layer->FixUpdate();

        _elapsed -= fixed;
    }

    _pCamera->FixedUpdate();
}

int Engine::GameManager::UpdateGame()
{
    if (!_isSetUp)
        return 0;

    if (_isSceneChange)
        return 0;

    int isEvent = 0;

    _pInputMgr->Update();
    _pTimeMgr->Update();

    float deltaTime = _pTimeMgr->GetDeltaTime();

    for (auto& Layer : _vecLayer)
    {
        isEvent = Layer->Update(deltaTime);
        if (-5 == isEvent)
            return isEvent;
    }

    isEvent = _pScene->Update(deltaTime);
    _pCamera->Update(deltaTime);

    return isEvent;
}

int Engine::GameManager::LateUpdateGame()
{
    if (!_isSetUp)
        return 0;

    if (_isSceneChange)
    {
        _isSceneChange = false;
        return 0;
    }

    int isEvent = 0;
    
    float deltaTime = _pTimeMgr->GetDeltaTime();

    for (auto& Layer : _vecLayer)
        Layer->LateUpdate(deltaTime);

    isEvent = _pScene->LateUpdate(deltaTime);
    _pCamera->LateUpdate(deltaTime);

    _pSoundMgr->Update(deltaTime);

    return isEvent;
}

void Engine::GameManager::RenderGame()
{
    if (!_isSetUp)
        return;

    for (auto& Layer : _vecLayer)
        Layer->AddRenderer();
    
    _pRenderer->Render_GameObject(_pGrahpicMgr->GetGraphic());
}

void Engine::GameManager::Initialize(const GameDefaultSetting& info)
{
    Renderer::RenderGroupInfo renderInfo;
    renderInfo.hdc = info.hdc;
    renderInfo.hWnd = info.hWnd;
    renderInfo.size = info.renderGroupSize;
    renderInfo.width = info.width;
    renderInfo.height = info.height;

    _pInputMgr->Initailize(info.hInstance, info.hWnd);
    _pSoundMgr->SetUpSound(info.maxSoundGroup);
    _pRenderer->SetUp_Renderer(renderInfo);
    _pGrahpicMgr->SetUpGrahpic(_pRenderer->GetBackDC());

    _vecLayer.reserve(info.layerSize);
    for (int i = 0; i < info.layerSize; i++)
        _vecLayer.push_back(Layer::Create());

    _originHDC = info.hdc;
    _hWnd = info.hWnd;
}

HWND Engine::GameManager::GetHWND()
{
    return _hWnd;
}

void Engine::GameManager::SetSlowTime(float rate, float time)
{
    _pTimeMgr->SetSlowTime(rate, time);
}

bool Engine::GameManager::ChagneScene(Scene* pScene)
{
    if (nullptr == pScene)
        return false;

    if (nullptr != _pScene)
        SafeRelease(_pScene);

    for (auto layer : _vecLayer)
        layer->ClearAllObjectList();

    _pScene = pScene;
    _pScene->Initialize();
    _isSetUp = true;   
    _isSceneChange = true;

    return true;
}

std::list<GameObject*>* Engine::GameManager::GetObjectList(int layerGroup, const char* listTag)
{
    return _vecLayer[layerGroup]->GetObjectList(listTag);
}

GameObject* Engine::GameManager::MyGetObject(int layerGroup, const char* listTag, const char* objectTag)
{
    return _vecLayer[layerGroup]->MyGetObject(listTag, objectTag);
}

void Engine::GameManager::Free()
{
    std::for_each(_vecLayer.begin(), _vecLayer.end(), SafeRelease<Layer*>);
    _vecLayer.clear();
    _vecLayer.shrink_to_fit();

    SafeRelease(_pCamera);
    SafeRelease(_pRenderer);
    SafeRelease(_pScene);
    SafeRelease(_pInputMgr);
    SafeRelease(_pTimeMgr);
    SafeRelease(_pTextureMgr);
    SafeRelease(_pSoundMgr);
    SafeRelease(_pGrahpicMgr);
    SafeRelease(_pCollisionMgr);
}

bool Engine::GameManager::LoadTexture(const wchar_t* filePath)
{
    return _pTextureMgr->LoadTexture(filePath);
}

HDC Engine::GameManager::GetOriginHDC()
{
    return _originHDC;
}

HDC Engine::GameManager::GetBackHDC()
{
    return _pRenderer->GetBackDC();
}

bool Engine::GameManager::AddRenderGroup(int renderGroup, GameObject* pObject)
{
    return _pRenderer->AddRenderGroup(renderGroup, pObject);
}

void Engine::GameManager::SetSortGroup(int sortGroup, bool(*sortFunc)(GameObject*, GameObject*))
{
    _pRenderer->SetSortGroup(sortGroup, sortFunc);
}

void Engine::GameManager::SetPlayMovie(bool isPlay)
{
    _pRenderer->_isMovie = isPlay;
}

void Engine::GameManager::CameraShake(float shakeTime, float shakePower)
{
    _pCamera->CameraShake(shakeTime, shakePower);
}

Vector3 Engine::GameManager::CameraPosition()
{
    return _pCamera->CameraPosition();
}

Texture* Engine::GameManager::FindTexture(const wchar_t* textureTag)
{
    return _pTextureMgr->FindTexture(textureTag);
}

void Engine::GameManager::LoadSound(const char* filePath)
{
    _pSoundMgr->LoadSound(filePath);
}

void Engine::GameManager::MyPlaySound(const char* soundTag, int groupID, bool isLoop)
{
    _pSoundMgr->MyPlaySound(soundTag, groupID, isLoop);
}

void Engine::GameManager::MyPlaySound(const char* soundTag, int groupID, bool isLoop, const Vector3& position)
{
    if (nullptr == _pSoundTarget) return;

    Vector3 targetPosition = _pSoundTarget->GetTransform()->GetPosition();
    float distance = (targetPosition - position).Length();

    if (700.f >= distance)
        _pSoundMgr->MyPlaySound(soundTag, groupID, isLoop);
}

void Engine::GameManager::SetVolume(int groupID, float volume)
{
    _pSoundMgr->SetVolume(groupID, volume);
}

void Engine::GameManager::SetFadeVolume(int groupID, float volume)
{
    _pSoundMgr->SetFadeVolume(groupID, volume);
}

void Engine::GameManager::SetPitch(int groupID, float pitch)
{
    _pSoundMgr->SetPitch(groupID, pitch);
}

void Engine::GameManager::SetMasterVolume(float volume)
{
    _pSoundMgr->SetMasterVolume(volume);
}

void Engine::GameManager::StopSound(int groupID)
{
    _pSoundMgr->StopSound(groupID);
}

void Engine::GameManager::SetSoundTarget(GameObject* pTarget)
{
    _pSoundTarget = pTarget;
}

std::vector<std::string> Engine::GameManager::GetSoundKey()
{
    return _pSoundMgr->GetSoundKey();
}

void Engine::GameManager::ClearObjectList(int layerGroup, const char* listTag)
{
    _vecLayer[layerGroup]->ClearObjectList(listTag);
    _pRenderer->ResetRenderGroup();
}

void Engine::GameManager::ClearLayer(int layerGroup)
{
    _vecLayer[layerGroup]->ClearAllObjectList();
    _pRenderer->ResetRenderGroup();
}

bool Engine::GameManager::AddObjectInLayer(int layerGroup, const char* listTag, GameObject* pObject)
{
    if (nullptr == pObject || _vecLayer.empty())
        return false;

    _vecLayer[layerGroup]->AddObject(listTag, pObject);

    return true;
}

bool Engine::GameManager::IsKeyDown(_byte keycord)
{
    return _pInputMgr->IsKeyDown(keycord);
}

bool Engine::GameManager::IsKeyDown(Input::MouseState mouseState)
{
    return _pInputMgr->IsKeyDown(mouseState);
}

bool Engine::GameManager::IsKeyUp(_byte keycord)
{
    return _pInputMgr->IsKeyUp(keycord);
}

bool Engine::GameManager::IsKeyUp(Input::MouseState mouseState)
{
    return _pInputMgr->IsKeyUp(mouseState);
}

bool Engine::GameManager::IsKeyPress(_byte keycord)
{
    return _pInputMgr->IsKeyPress(keycord);
}

bool Engine::GameManager::IsKeyPress(Input::MouseState mouseState)
{
    return _pInputMgr->IsKeyPress(mouseState);
}

bool Engine::GameManager::IsMouseWheel(short keycord)
{
    return _pInputMgr->IsMouseWheel(keycord);
}

