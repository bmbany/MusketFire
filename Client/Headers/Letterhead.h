#pragma once
#include "UI.h"
#include "Struct.h"

namespace Engine
{
	class BitFlag;
}

class TextLoadSystem;
class Letter;
class Letterhead : public UI
{
public:
	enum Flag : _ullong
	{
		FlagStamp = 1 << 0,
		FlagButton = 1 << 1,
		FlagRestart = 1 << 2
	};
private:
	explicit Letterhead() = default;
	virtual ~Letterhead() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void DrawTitleText(Gdiplus::Graphics* pGraphics);
	void DrawTextLines(Gdiplus::Graphics* pGraphics, int inputLineStart);

private:
	bool Initialize(const Vector3& position, int id, Letter* Owner);
	bool InitializeFont();
	bool InitializeLinePosition();
	void SetDrawLetter(Gdiplus::Graphics* pGraphic, const Vector3& _Scale, Gdiplus::Bitmap* pBitmap, const Vector3& Position);
	void DrawString(Gdiplus::Graphics* pGraphics, const WCHAR* text, Gdiplus::RectF drawRect, Gdiplus::Font* _pFont);
	void SetLetterColorKey(Gdiplus::Color lowColor, Gdiplus::Color highColor);
	void Free() override;

public:
	static Letterhead* Create(const Vector3& position, int id, Letter* Owner);

private:
	Engine::Collider* _pCollider = nullptr;
	TextLoadSystem* _pTextLoadSystem = nullptr;
	Letter* _pLetter = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;

private:
	Vector3 _lineOffset{};
	Vector3 _contentsLinePosition[25]{ {} };

private:
	Gdiplus::PrivateFontCollection* _pFontCollection = nullptr;
	Gdiplus::RectF _titleRect{};
	Gdiplus::RectF _lineRect{};
	Gdiplus::RectF _drawRect{};
	Gdiplus::StringFormat* _pStringFormat = nullptr;
	Gdiplus::FontFamily* _pFontRegulerFamily = nullptr;
	Gdiplus::FontFamily* _pFontBoldFamily = nullptr;
	Gdiplus::FontFamily* FontFamilies = nullptr;
	Gdiplus::Font* _pMainFont = nullptr;
	Gdiplus::Font* _pSubFont = nullptr;
	Gdiplus::SolidBrush* _pBrush = nullptr;
	Gdiplus::ImageAttributes _imageAttributes;

private:
	const int _maxLine = 25;
	const LetterStruct* _pLetterStruct = nullptr;
	Vector3 _stampsScale{};
	Vector3 _buttonScale{};
	int _loadLetterID = 0;
};