#ifdef __GANGJIN__

#include "TestStage_3.h"
#include "Flintlock.h"
#include "Mouse.h"
#include "Ammunition.h"
#include "Rammer.h"
#include "Soldier.h"
#include "Section.h"
#include "Inventory.h"
#include "UI.h"
#include "Infantry.h"
#include "Infantry.h"
#include "CombatManager.h"
#include "CollisionManager.h"
#include "Client_Define.h"

int TestStage::Update(const float& fDeltaTime)
{
    _elapsed += fDeltaTime;
    _fps++;

    if (1.f <= _elapsed)
    {
        std::cout << _fps << std::endl;
        _elapsed = 0.f;
        _fps = 0;
    }
    
    return 0;
}

int TestStage::LateUpdate(const float& fDeltaTime)
{
    
    return 0;
}

void TestStage::Free()
{

}

bool TestStage::Initialize()
{
    //Engine::AddObjectInLayer((int)LayerGroup::Object, "Enemy", Enemy::Create()); 
    //Engine::AddObjectInLayer((int)LayerGroup::Object, "ScareCrow", ScareCrow::Create()); 
    Engine::AddObjectInLayer((int)LayerGroup::Object, "Infantry", Infantry::Create());
    //Engine::AddObjectInLayer((int)LayerGroup::Object, "Canon", Canon::Create());


    return true;
}

TestStage* TestStage::Create()
{
    TestStage* pInstance = new TestStage;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}

#endif