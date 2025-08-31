#pragma once
#include "Base.h"

namespace Engine
{
	class GraphicManager : public Base
	{
	private:
		explicit GraphicManager() = default;
		virtual ~GraphicManager() = default;
		
	public:
		Gdiplus::Graphics* GetGraphic() { return _pGraphic; }
		void SetUpGrahpic(const HDC& hdc);

	private:
		bool Initialize();
		// CBase을(를) 통해 상속됨
		void Free() override;

	private:
		Gdiplus::GdiplusStartupInput	_gpsi;
		Gdiplus::Graphics*				_pGraphic;
		ULONG_PTR						_gpToken;

	public:
		static GraphicManager* Create();
	};
}
