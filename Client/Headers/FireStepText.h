#pragma once
#include "UI.h"
#include "Struct.h"

class TextLoadSystem;
class CockPin;
class Flintlock;
class FireStepText : public UI
{
private:
	explicit FireStepText() = default;
	virtual ~FireStepText() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void DrawString(Gdiplus::Graphics* pGraphics, const WCHAR* text,
		const Vector3& position);

	void SetTarget();

private:
	bool Initialize(const Vector3& position);
	void Free() override;

public:
	static FireStepText* Create(const Vector3& position);

private:
	Flintlock* _pFlintlock = nullptr;
	CockPin* _pCockPin = nullptr;

	Gdiplus::PrivateFontCollection* _pFontCollection = nullptr;
	Gdiplus::FontFamily* _pFontFamily = nullptr;
	Gdiplus::Font* _pFont = nullptr;
	Gdiplus::SolidBrush* _pBrush = nullptr;
	TextLoadSystem* _pTextLoadSystem = nullptr;
	const LetterStruct* _pLetterStruct = nullptr;
	int _loadLetterID = 0;
};
