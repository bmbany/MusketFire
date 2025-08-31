#pragma once
#include "Base.h"

namespace Engine
{
	class GameObject;
	class Renderer : public Base
	{
		friend class GameManager;
	public:
		struct RenderGroupInfo
		{
			HDC hdc;
			HWND hWnd;
			int size;
			int width;
			int height;
		};
	private:
		using RenderGroup = std::vector<std::list<GameObject*>>;
		using SortInfo = std::vector<bool(*)(GameObject*, GameObject*)>;

	private:
		explicit Renderer() = default;
		virtual ~Renderer() = default;

	public:
		void Render_GameObject(Gdiplus::Graphics* pGraphics);
		void SetUp_Renderer(const RenderGroupInfo& info);
		void SetSortGroup(int sortGroup, bool(*sortFunc)(GameObject*, GameObject*));
		bool AddRenderGroup(int group, GameObject* pObject);
		void ResetRenderGroup();
		HDC GetBackDC() { return _backMemDC; }

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	public:
		static Renderer* Create();

	private:
		RenderGroupInfo _info;
		RenderGroup		_renderGroup;
		SortInfo		_sortInfo;
		HBITMAP			_backBitmap = nullptr;
		HDC				_backMemDC = nullptr;
		bool			_isSetUp = false;
		bool			_isMovie = false;
	};
}

