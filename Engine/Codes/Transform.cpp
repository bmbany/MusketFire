#include "Transform.h"

using namespace Engine;

bool Engine::Transform::Initialize()
{
    _transform[Scale] = { 1.f, 1.f, 0.f };

    return true;
}

void Engine::Transform::Free()
{
}

Transform* Engine::Transform::Create(const char* name)
{
    Transform* pInstance = new Transform(name);
    if (pInstance->Initialize())
        return pInstance;

    SafeRelease(pInstance);
    return nullptr;
}
