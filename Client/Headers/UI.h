#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitmapRenderer;
}
class UI : public Engine::GameObject
{
public:
	typedef struct UI_Info1
	{
		Vector3 position;
		Vector3 scale;
		Engine::Transform* pTarget = nullptr;
		const char* name = nullptr;
		const wchar_t* textureTag = nullptr;
		int index = 0;
	}GDIPlus_Info;

	typedef struct UI_Info2
	{
		Vector3 position;
		Vector3 scale;
		Engine::Transform* pTarget = nullptr;
		const char* name = nullptr;
		const wchar_t* textureTag = nullptr;
		HDC originHDC;
		HDC backHDC;
		int index = 0;
		bool isTransparent = false;
	}GDI_Info;

protected:
	explicit UI() = default;
	virtual ~UI() = default;

public:
	Vector3 GetScale();
	void SetScale(Vector3 scale);
	void AddScale(Vector3 scale);

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize(const GDIPlus_Info& info);
	bool Initialize(const GDI_Info& info);

public:
	static UI* Create(GDIPlus_Info& info);
	static UI* Create(GDI_Info& info);

private:
	Engine::Transform*		_pTarget = nullptr;
	Engine::BitmapRenderer* _pBitmapRenderer = nullptr;

	GDIPlus_Info _uiGdiplusInfo;
	GDI_Info _uiGdiInfo;

	Vector3 _position;
	Vector3 _scale;
	bool _isGDI = false;
};

