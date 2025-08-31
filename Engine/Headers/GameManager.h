#pragma once

#include "Base.h"

namespace Engine
{
	class Scene;
	class Renderer;
	class Layer;
	class GameObject;
	class Texture;
	class Camera;
	class InputManager;
	class TimeManager;
	class TextureManager;
	class SoundManager;
	class GraphicManager;
	class CollisionManager;

	class GameManager : public Base, public SingleTon<GameManager>
	{
		friend class SingleTon;
	public:
		struct GameDefaultSetting
		{
			HINSTANCE hInstance;
			HWND hWnd;
			HDC hdc;
			int renderGroupSize;
			int maxSoundGroup;
			int layerSize;
			int width;
			int height;
		};
	private:
		GameManager();
		~GameManager() = default;
		NOCOPY(GameManager)

	public:
		void FixedUpdateGame(int count);
		int UpdateGame();
		int LateUpdateGame();
		void RenderGame();
		void Initialize(const GameDefaultSetting& info);
		HWND GetHWND();

		// TimeMgr
		void SetSlowTime(float rate, float time);

		// InputMgr
		bool IsKeyDown(_byte keycord);
		bool IsKeyDown(Input::MouseState mouseState);
		bool IsKeyUp(_byte keycord);
		bool IsKeyUp(Input::MouseState mouseState);
		bool IsKeyPress(_byte keycord);
		bool IsKeyPress(Input::MouseState mouseState);
		bool IsMouseWheel(short keycord);

		// SceneMgr
		bool ChagneScene(Scene* pScene);

		// Layer
		void ClearObjectList(int layerGroup, const char* listTag);
		void ClearLayer(int layerGroup);
		std::list<GameObject*>* GetObjectList(int layerGroup, const char* listTag);
		GameObject* MyGetObject(int layerGroup, const char* listTag, const char* objectTag);
		bool AddObjectInLayer(int layerGroup, const char* listTag, GameObject* pObject);

		// TextureMgr
		bool LoadTexture(const wchar_t* filePath);
		Texture* FindTexture(const wchar_t* textureTag);

		// SoundMgr		
		void LoadSound(const char* filePath);
		void MyPlaySound(const char* soundTag, int groupID, bool isloop);
		void MyPlaySound(const char* soundTag, int groupID, bool isloop, const Vector3& position);
		void SetVolume(int groupID, float volume);
		void SetFadeVolume(int groupID, float volume);
		void SetPitch(int groupID, float pitch);
		void SetMasterVolume(float volume);
		void StopSound(int groupID);
		void SetSoundTarget(GameObject* pTarget);
		std::vector<std::string> GetSoundKey();

		// Renderer
		HDC GetOriginHDC();
		HDC GetBackHDC();
		bool AddRenderGroup(int renderGroup, GameObject* pObject);
		void SetSortGroup(int sortGroup, bool(*sortFunc)(GameObject*, GameObject*));
		void SetPlayMovie(bool isPlay);

		// Camera
		void CameraShake(float shakeTime, float shakePower);
		Vector3 CameraPosition();

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		std::vector<Layer*>	_vecLayer;
		InputManager*		_pInputMgr		= nullptr;
		TimeManager*		_pTimeMgr		= nullptr;
		TextureManager*		_pTextureMgr	= nullptr;
		SoundManager*		_pSoundMgr		= nullptr;
		GraphicManager*		_pGrahpicMgr	= nullptr;
		CollisionManager*	_pCollisionMgr	= nullptr;
		Renderer*			_pRenderer		= nullptr;
		Scene*				_pScene			= nullptr;
		HDC					_originHDC		= nullptr;
		HWND				_hWnd			= nullptr;
		GameObject*			_pSoundTarget	= nullptr;
		Camera*				_pCamera		= nullptr;
		float				_elapsed		= 0.f;
		bool				_isSetUp		= false;
		bool				_isSceneChange	= false;
	};
}