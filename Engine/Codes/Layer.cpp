#include "Layer.h"
#include "GameObject.h"

using namespace Engine;

void Layer::FixUpdate()
{
    for (auto& Pair : _objectData)
    {
        for (auto& Object : Pair.second)
        {
            Object->FixedUpdate();
        }
    }
}

int Layer::Update(const float& deltaTime)
{
    int isEvent = 0;

    for (auto& Pair : _objectData)
    {
        for (auto& Object : Pair.second)
        {
            if (Object->IsDead())
                continue;

            Object->UpdateComponent(deltaTime);
            isEvent = Object->Update(deltaTime);
            if (-5 == isEvent)
                return isEvent;
        }
    }

    return isEvent;
}

int Layer::LateUpdate(const float& deltaTime)
{
    int isEvent = 0;
   
    for (auto& Pair : _objectData)
    {
        auto iter = Pair.second.begin();
        for (iter; iter != Pair.second.end();)
        {
            if ((*iter)->IsDead())
            {
                SafeRelease(*iter);
                iter = Pair.second.erase(iter);
            }
            else
            {
                (*iter)->LateUpdate(deltaTime);
                ++iter;
            }
        }
    }

    return isEvent;
}

void Engine::Layer::AddRenderer()
{
    for (auto& Pair : _objectData)
    {
        for (auto& Object : Pair.second)
        {
            if (!Object->IsDead())
                Object->AddRenderer();
        }
    }
}

void Engine::Layer::ClearObjectList(const char* listTag)
{
    if (0 == _objectData[listTag].size())
        return;

    for (auto& Object : _objectData[listTag])
    {
        Object->SetDead();       
    }
}

void Engine::Layer::ClearAllObjectList()
{
    for (auto& Pair : _objectData)
    {
        for (auto& Object : Pair.second)
            Object->SetDead();
    }
}

GameObject* Engine::Layer::MyGetObject(const char* listTag, const char* objectTag)
{
    if (0 == _objectData[listTag].size())
        return nullptr;

    for (auto& Object : _objectData[listTag])
    {
        if (Object->IsDead())
            continue;

        if (*Object == objectTag)
            return Object;
    }

    return nullptr;
}

bool Layer::AddObject(const char* listTag, GameObject* pObject)
{
    if (nullptr == pObject)
        return false;

    _objectData[listTag].push_back(pObject);

    return true;
}

void Layer::Free()
{
    for (auto& Pair : _objectData)
    {
        for (auto& Object : Pair.second)
            SafeRelease(Object);

        Pair.second.clear();
    }

    _objectData.clear();
}

Layer* Layer::Create()
{
    return new Layer;
}
