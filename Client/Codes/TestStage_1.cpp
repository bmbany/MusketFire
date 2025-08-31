//#ifdef __MINJAE__
//#include "TestStage_1.h"
//#include "Mouse.h"
//
//// Musket
//#include "Flintlock.h"
//#include "Ammunition.h"
//#include "Rammer.h"
//#include "CockPin.h"
//#include "FlashHole.h"
//#include "Flint.h"
//#include "GunPowder.h"
//#include "PaperCartidge.h"
//#include "FireStepText.h"
//
//// Section
//#include "Inventory.h"
//#include "Musket.h"
//#include "Combat.h"
//#include "Bubble.h"
//#include "UI.h"
//
//#include "GameManager.h"
//#include "CollisionManager.h"
//#include "Client_Define.h"
//
//int TestStage::Update(const float& fDeltaTime)
//{
//    /*_elapsed += fDeltaTime;
//    _fps++;
//
//    if (1.f <= _elapsed)
//    {
//        std::cout << _fps << std::endl;
//        _elapsed = 0.f;
//        _fps = 0;
//    }*/
//    
//    if (Engine::IsKeyDown(Input::DIM_LB))
//    {
//        Engine::MyPlaySound(_soundKey[_index].c_str(), (int)SoundChannel::PlayerVoice);
//    }
//
//    if (Engine::IsMouseWheel(VK_MWHEELUP))
//    {
//        float value = 0.01f;
//
//        if (Engine::IsKeyPress(DIK_LSHIFT))
//            value = 0.1f;
//
//        _pitch += value;
//        _pGameMgr->SetPitch((int)SoundGroup::Voice, _pitch);
//
//        system("cls");
//        std::cout << "사운드 : " << _soundKey[_index].c_str() << "\nPitch : " << _pitch << std::endl;
//    }
//
//    if (Engine::IsMouseWheel(VK_MWHEELDOWN))
//    {
//        float value = 0.01f;
//
//        if (Engine::IsKeyPress(DIK_LSHIFT))
//            value = 0.1f;
//
//        _pitch -= value;
//        _pGameMgr->SetPitch((int)SoundGroup::Voice, _pitch);
//
//        system("cls");
//        std::cout << "사운드 : " << _soundKey[_index].c_str() << "\nPitch : " << _pitch << std::endl;
//    }
//
//    if (Engine::IsKeyDown(DIK_PGUP))
//    {
//        _index++;
//
//        if (_index >= _soundKey.size())
//        {
//            _index = 0;
//        }
//
//        system("cls");
//        std::cout << "사운드 : " << _soundKey[_index].c_str() << "\nPitch : " << _pitch << std::endl;
//    }
//    if (Engine::IsKeyDown(DIK_PGDN))
//    {
//        _index--;
//
//        if (0 >= _index)
//        {
//            _index = _soundKey.size() - 1;
//        }
//
//        system("cls");
//        std::cout << "사운드 : " << _soundKey[_index].c_str() << "\nPitch : " << _pitch << std::endl;
//    }
//
//    return 0;
//}
//
//int TestStage::LateUpdate(const float& fDeltaTime)
//{
//    _pCollisionMgr->CheckCollision(
//        Engine::GetObjectList((int)LayerGroup::Object, "Musket"),
//        Engine::GetObjectList((int)LayerGroup::Object, "Mouse"));
//
//    _pCollisionMgr->CheckCollision(
//        Engine::GetObjectList((int)LayerGroup::Object, "Musket"),
//        Engine::GetObjectList((int)LayerGroup::Object, "Musket"));
//
//    _pCollisionMgr->CheckCollision(
//        Engine::GetObjectList((int)LayerGroup::UI, "Section"),
//        Engine::GetObjectList((int)LayerGroup::Object, "Mouse"));
//
//    _pCollisionMgr->CheckCollision(
//        Engine::GetObjectList((int)LayerGroup::UI, "Section"),
//        Engine::GetObjectList((int)LayerGroup::Object, "Musket"));
//
//    _pCollisionMgr->CheckCollision(
//        Engine::GetObjectList((int)LayerGroup::UI, "UI"),
//        Engine::GetObjectList((int)LayerGroup::Object, "Mouse"));
//
//    _pCollisionMgr->CheckCollision(
//        Engine::GetObjectList((int)LayerGroup::Object, "Soldier"),
//        Engine::GetObjectList((int)LayerGroup::Object, "Soldier"));
//
//    return 0;
//}
//
//void TestStage::Free()
//{
//    Engine::SafeRelease(_pCollisionMgr);
//}
//
//bool TestStage::Initialize()
//{
//    _pCollisionMgr = Engine::CollisionManager::Create();
//    _pGameMgr = Engine::GameManager::GetInstance();
//    _soundKey = _pGameMgr->GetSoundKey();
//
//    //Engine::AddObjectInLayer((int)LayerGroup::Object, "Resource", CheckResource::Create());
//
//    /*Engine::AddObjectInLayer((int)LayerGroup::Object, "Mouse", Mouse::Create());    
//    Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Bubble::Create(Vector3(500.f, 300.f, 0.f), 1));
//
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Flintlock::Create(Vector3(600.f, 540.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(50.f, 350.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(150.f, 350.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(250.f, 350.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(50.f, 450.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(150.f, 450.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(250.f, 450.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", PaperCartidge::Create(Vector3(150.f, 550.f, 0.f)));
//    Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", GunPowder::Create(Vector3(150.f, 600.f, 0.f)));    
//    Engine::AddObjectInLayer((int)LayerGroup::UI, "Text", FireStepText::Create(Vector3(930.f, 500.f, 0.f)));
//
//    Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Combat::Create());
//    Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Musket::Create());
//    Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Inventory::Create());
//
//    UI::GDI_Info info;
//    info.textureTag = L"UI_Frame";
//    info.index = 1;
//    info.position = { 640.f, 508.f, 0.f };
//    info.scale = { 1.f, 1.f, 0.f };
//    info.name = "Frame";
//    info.originHDC = Engine::GetOriginHDC();
//    info.backHDC = Engine::GetBackHDC();
//    info.isTransparent = true;
//    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", UI::Create(info));    */        
//
//    return true;
//}
//
//TestStage* TestStage::Create()
//{
//    return new TestStage;
//}
//
//#endif