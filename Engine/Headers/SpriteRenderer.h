#pragma once
#include "Component.h"

namespace Engine
{
	class Transform;
	class SpriteRenderer : public Component
	{
	private:
		explicit SpriteRenderer(const char* name) { _name = name; }
		virtual ~SpriteRenderer() = default;

	public:
		void SetTargetTransform(Transform* pTarget);
		void SetDrawInformation(Gdiplus::Bitmap* pBitmap, const Vector3& cameraPosition = Vector3(0.f, 0.f, 0.f));
		void SetRotate(const float& angle);
		void SetColorKey(Gdiplus::Color lowColor, Gdiplus::Color highColor);
		void SetColorMatrix(int row, int column, float value);
		void SetTrasnform(Gdiplus::Graphics* pGraphic);
		void ResetTransform(Gdiplus::Graphics* pGraphic);
		void Draw(Gdiplus::Graphics* pGraphic, Gdiplus::Bitmap* pBitmap);

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	public:
		static SpriteRenderer* Create(const char* name);

	private:
		Transform*					_pTransform = nullptr;
		Gdiplus::ImageAttributes	_imageAttributes;
		Gdiplus::Matrix				_rotateMatrix;
		Gdiplus::Rect				_drawRect;
		Gdiplus::ColorMatrix		_colorMatrix =
		{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
	};
}
