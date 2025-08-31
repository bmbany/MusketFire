#pragma once
#include "Item.h"
#include "Struct.h"

class Letter : public Item
{
public:
	enum LetterFlag : _ullong
	{
		FlagOpen = 1 << 0,
	};
private:
	explicit Letter() = default;
	virtual ~Letter() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void SetClose();

private:
	bool Initialize(const Vector3& position, int id);
	void Free() override;

public:
	static Letter* Create(const Vector3& position, int id);

private:
	Engine::Collider* _pCollider = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	Gdiplus::PrivateFontCollection* _pFontCollection = nullptr;
	Gdiplus::StringFormat* _pStringFormat = nullptr;
	Gdiplus::FontFamily* _pFontFamily = nullptr;
	Gdiplus::RectF _letterTitleRect{};
	Gdiplus::Font* _pFont = nullptr;
	Gdiplus::SolidBrush* _pBrush = nullptr;

private:
	int _thisLetterID = 0;
	const LetterStruct* _pLetterStruct = nullptr;
};