#pragma once

#include "GameManager.h"

namespace Engine
{
	//
	inline HWND GetHWND();

	// TimeMgr
	inline void SetSlowTime(float rate, float time);

	// InputMgr
	inline bool IsKeyDown(_byte keycord);
	inline bool IsKeyDown(Input::MouseState mouseState);
	inline bool IsKeyUp(_byte keycord);
	inline bool IsKeyUp(Input::MouseState mouseState);
	inline bool IsKeyPress(_byte keycord);
	inline bool IsKeyPress(Input::MouseState mouseState);
	inline bool IsMouseWheel(short keycord);

	// SceneMgr
	inline bool ChagneScene(Scene* pScene);
		
	// Layer
	inline void ClearLayer(int layerGroup);
	inline void ClearObjectList(int layerGroup, const char* listTag);
	inline std::list<GameObject*>* GetObjectList(int layerGroup, const char* listTag);
	inline GameObject* MyGetObject(int layerGroup, const char* listTag, const char* objectTag);
	inline bool AddObjectInLayer(int layerGroup, const char* listTag, GameObject* pObject);

	// TextureMgr
	inline bool LoadTexture(const wchar_t* filePath);
	inline Texture* FindTexture(const wchar_t* textureTag);

	// SoundMgr
	inline void MyPlaySound(const char* soundTag, int groupID, bool isLoop);
	inline void DistancePlaySound(const Vector3& position, const char* soundTag, int groupID, bool isLoop);
	inline void SetFadeVolume(int groupID, float volume);
	inline void SetVolume(int groupID, float volume);
	inline void StopSound(int groupID);
	inline void SetSoundTarget(GameObject* pTarget);
	// Renderer
	inline bool AddRenderGroup(int renderGroup, GameObject* pObject);
	inline HDC GetOriginHDC();
	inline HDC GetBackHDC();

	// Camera
	inline void CameraShake(float shakeTime, float shakePower);
	inline Vector3 CameraPosition();
#include "Export.inl"
}