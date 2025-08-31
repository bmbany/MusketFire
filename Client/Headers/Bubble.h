#pragma once
#include "UI.h"
#include "Struct.h"

namespace Engine
{
	class Timer;
	class BitFlag;
}

class TextLoadSystem;
class Bubble : public UI
{
public:
	enum BubbleFlag : _ullong
	{
		AliesSpeak = 1 << 0,
		EnemySpeak = 1 << 1,
		RenderingAlies = 1 << 2,
		RenderingEnemy = 1 << 3,
		PlaySoundAlies = 1 << 4,
		PlaySoundEnemy = 1 << 5,
	};

	enum IFF_code { Alies, Enemy, SoundAlies, SoundEnemy, End };
private:
	explicit Bubble() = default;
	virtual ~Bubble() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void BubbleSpeak(int IFF_code, int massageID);
	void BubbleRangedSpeak(int IFF_Code, int minID, int maxID);
	bool IsBubbleRendering(int IFF_code);
	int getCurrentAliesMassageID();
	int getCurrentEnemyMassageID();

private:
	bool Initialize(const Vector3& position, int id);
	bool InitializeFont();
	bool InitializeLinePosition();
	void SetDrawBubble(Gdiplus::Graphics* pGraphic, Gdiplus::Bitmap* pBitmap, const Vector3& Position);
	void DrawMessage(Gdiplus::Graphics* pGraphics, int IFF_code);
	void SetBubbleColorKey(Gdiplus::Color lowColor, Gdiplus::Color highColor);
	void Free() override;

public:
	static Bubble* Create(const Vector3& position, int id);

private:
	TextLoadSystem* _pTextAliesSystem = nullptr;
	TextLoadSystem* _pTextEnemySystem = nullptr;
	TextLoadSystem* _pSoundAliesSystem = nullptr;
	TextLoadSystem* _pSoundEnemySystem = nullptr;
	Vector3 _bubbleContentsAliesPosition{};
	Vector3 _bubbleContentsEnemyPosition{};

private:
	Gdiplus::PrivateFontCollection* _pFontCollection = nullptr;
	Gdiplus::StringFormat* _pStringFormat = nullptr;
	Gdiplus::FontFamily* _pFontFamily = nullptr;
	Gdiplus::ImageAttributes _imageAttributes;
	Gdiplus::Rect _drawRect;
	Gdiplus::RectF _bubbleAliesRect{};
	Gdiplus::RectF _bubbleEnemyRect{};
	Gdiplus::Font* _pFont = nullptr;
	Gdiplus::SolidBrush* _pBrush = nullptr;
	Engine::Timer* _pTimer = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;

private:
	int _currentIFF_Code = 0;
	int _thisBubbleID = 0;
	int _oldAliesMassageID = 0;
	int _oldEnemyMassageID = 0;
	int _currentAliesMassageID = 1;
	int _currentEnemyMassageID = 1;
	Vector3 _bubbleAliesPosition{};
	Vector3 _bubbleEnemyPosition{};
	const LetterStruct* _pTextAlies = nullptr;
	const LetterStruct* _pTextEnemy = nullptr;
	const LetterStruct* _pSoundAlies = nullptr;
	const LetterStruct* _pSoundEnemy = nullptr;
};