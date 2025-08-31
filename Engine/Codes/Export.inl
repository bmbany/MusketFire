#pragma once

HWND GetHWND()
{
	return GameManager::GetInstance()->GetHWND();
}

// TimeMgr
void SetSlowTime(float rate, float time)
{
	GameManager::GetInstance()->SetSlowTime(rate, time);
}

// InputMgr
bool IsKeyDown(_byte keycord)
{
	return GameManager::GetInstance()->IsKeyDown(keycord);
}
bool IsKeyDown(Input::MouseState mouseState)
{
	return GameManager::GetInstance()->IsKeyDown(mouseState);
}
bool IsKeyUp(_byte keycord)
{
	return GameManager::GetInstance()->IsKeyUp(keycord);
}
bool IsKeyUp(Input::MouseState mouseState)
{
	return GameManager::GetInstance()->IsKeyUp(mouseState);
}
bool IsKeyPress(_byte keycord)
{
	return GameManager::GetInstance()->IsKeyPress(keycord);
}
bool IsKeyPress(Input::MouseState mouseState)
{
	return GameManager::GetInstance()->IsKeyPress(mouseState);
}
bool IsMouseWheel(short keycord)
{
	return GameManager::GetInstance()->IsMouseWheel(keycord);
}
// SceneMgr
bool ChagneScene(Scene* pScene)
{
	return GameManager::GetInstance()->ChagneScene(pScene);
}

// Layer
void ClearLayer(int layerGroup)
{
	GameManager::GetInstance()->ClearLayer(layerGroup);
}
void ClearObjectList(int layerGroup, const char* listTag)
{
	GameManager::GetInstance()->ClearObjectList(layerGroup, listTag);
}
std::list<GameObject*>* GetObjectList(int layerGroup, const char* listTag)
{
	return GameManager::GetInstance()->GetObjectList(layerGroup, listTag);
}
GameObject* MyGetObject(int layerGroup, const char* listTag, const char* objectTag)
{
	return GameManager::GetInstance()->MyGetObject(layerGroup, listTag, objectTag);
}
bool AddObjectInLayer(int layerGroup, const char* listTag, GameObject* pObject)
{
	return GameManager::GetInstance()->AddObjectInLayer(layerGroup, listTag, pObject);
}

// TextureMgr
bool LoadTexture(const wchar_t* filePath)
{
	return GameManager::GetInstance()->LoadTexture(filePath);
}
Texture* FindTexture(const wchar_t* textureTag)
{
	return GameManager::GetInstance()->FindTexture(textureTag);
}

// SoundMgr
void MyPlaySound(const char* soundTag, int groupID, bool isLoop = false)
{
	GameManager::GetInstance()->MyPlaySound(soundTag, groupID, isLoop);
}
void DistancePlaySound(const Vector3& position, const char* soundTag, int groupID, bool isloop = false)
{
	GameManager::GetInstance()->MyPlaySound(soundTag, groupID, isloop, position);
}
inline void SetFadeVolume(int groupID, float volume)
{
	GameManager::GetInstance()->SetFadeVolume(groupID, volume);
}
inline void SetVolume(int groupID, float volume)
{
	GameManager::GetInstance()->SetVolume(groupID, volume);
}
void StopSound(int groupID)
{
	GameManager::GetInstance()->StopSound(groupID);
}
void SetSoundTarget(GameObject* pTarget)
{
	GameManager::GetInstance()->SetSoundTarget(pTarget);
}

// Renderer
bool AddRenderGroup(int renderGroup, GameObject* pObject)
{
	return GameManager::GetInstance()->AddRenderGroup(renderGroup, pObject);
}
HDC GetOriginHDC()
{
	return GameManager::GetInstance()->GetOriginHDC();
}
HDC GetBackHDC()
{
	return GameManager::GetInstance()->GetBackHDC();
}

// Camera
void CameraShake(float shakeTime, float shakePower)
{
	GameManager::GetInstance()->CameraShake(shakeTime, shakePower);
}
Vector3 CameraPosition()
{
	return GameManager::GetInstance()->CameraPosition();
}