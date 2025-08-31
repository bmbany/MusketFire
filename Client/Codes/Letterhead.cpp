#include "Letterhead.h"
#include "Letter.h"
#include "Client_Define.h"
#include "TextLoadSystem.h"
#include "BitFlag.h"

#include <string>

int Letterhead::Update(const float& deltaTime)
{
	return 0;
}

int Letterhead::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Letterhead::Render(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Rect rc;
	rc.X = 0;
	rc.Y = 0;
	rc.Width = WINCX;
	rc.Height = WINCY;

	Gdiplus::Color color(220, 0, 0, 0);
	Gdiplus::SolidBrush solideBrush(color);
	pGraphics->FillRectangle(&solideBrush, rc);
	if(_loadLetterID == 11)
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[1]);
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[1]);
	}
	else if (_loadLetterID == 12)
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[2]);
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[2]);
	}
	else
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
	}

	if (_loadLetterID == 1)
	{
		SetDrawLetter(pGraphics, Vector3(1.f, 1.f, 1.f), (*_vecTextures[1])[0],
			_pTransform->GetPosition() + Vector3(0.f, -140.f, -10.f));
	}
	else if (_loadLetterID == 2)
	{
		SetDrawLetter(pGraphics, Vector3(1.f, 1.f, 1.f), (*_vecTextures[1])[1],
			_pTransform->GetPosition() + Vector3(0.f, -140.f, -10.f));
	}
	else if (_loadLetterID == 3)
	{
		GetCollider("RestartButton")->SetActive(true);

		SetLetterColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));
		SetDrawLetter(pGraphics, Vector3(0.8f, 0.8f, 0.8f), (*_vecTextures[2])[0], _pTransform->GetPosition() +
			Vector3(0.f, 185.f, -10.f));

		_drawRect = Gdiplus::RectF(_pTransform->GetPosition().x - 200.f, _pTransform->GetPosition().y + 157.f, 400.f, 200.f);

		if (_pBitFlag->CheckFlag(FlagButton))
		{
			SetDrawLetter(pGraphics, Vector3(0.3f, 0.3f, 0.3f), (*_vecTextures[2])[1], _pTransform->GetPosition() +
				Vector3(150.f, 200.f, -10.f));
		}
	}
	else if (_loadLetterID == 4)
	{
		GetCollider("RestartButton")->SetActive(true);

		SetLetterColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));
		SetDrawLetter(pGraphics, Vector3(0.8f, 0.8f, 0.8f), (*_vecTextures[2])[0], _pTransform->GetPosition() +
			Vector3(0.f, 185.f, -10.f));

		_drawRect = Gdiplus::RectF(_pTransform->GetPosition().x - 200.f, _pTransform->GetPosition().y + 157.f, 400.f, 200.f);

		if (_pBitFlag->CheckFlag(FlagButton))
		{
			SetDrawLetter(pGraphics, Vector3(0.3f, 0.3f, 0.3f), (*_vecTextures[2])[1], _pTransform->GetPosition() +
				Vector3(150.f, 160.f, -10.f));
		}
	}

	DrawTitleText(pGraphics);

	if (_loadLetterID <= 2)
	{
		DrawTextLines(pGraphics, 6);
	}
	else
	{
		DrawTextLines(pGraphics, 0);
	}

	for (auto& Collider : GetColliders())
		Collider->DrawCollider(pGraphics);
}

void Letterhead::AddRenderer()
{
	if (IsActive())
		Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void Letterhead::OnCollision(CollisionInfo info)
{
	if (!IsActive() || _pBitFlag->CheckFlag(FlagRestart))
		return;

	if (*info.other->GetOwner() == "Continue")
		_pBitFlag->OnFlag(FlagRestart);

	if (*info.other == "Mouse" && *info.itSelf == "LetterButton")
	{
		if (Engine::IsKeyPress(Input::DIM_LB))
		{
			if (_pLetter != nullptr)
			{
				_pLetter->SetClose();
			}
			Engine::MyPlaySound("SFX_SFX_LetterClose", (int)SoundGroup::SFX);
			SetDead();
		}
	}
}

void Letterhead::OnCollisionExit(CollisionInfo info)
{
	if (*info.other->GetOwner() == "Continue")
		_pBitFlag->OffFlag(FlagRestart);
}

void Letterhead::DrawTitleText(Gdiplus::Graphics* pGraphics)
{
	INT strLength = (INT)wcslen(_pLetterStruct->_letterTitle);

	pGraphics->DrawString(_pLetterStruct->_letterTitle, strLength, _pMainFont,
		_titleRect, _pStringFormat, _pBrush);
}

void Letterhead::DrawTextLines(Gdiplus::Graphics* pGraphics, int inputLineStart)
{
	int maxContentsLine = (int)_pLetterStruct->_letterContents.size();

	for (const wchar_t* line : _pLetterStruct->_letterContents)
	{
		if (inputLineStart >= _maxLine)
			break;

		INT strLength = (INT)wcslen(line);


		_lineRect = Gdiplus::RectF(_contentsLinePosition[inputLineStart].x,
			_contentsLinePosition[inputLineStart].y, 350.f, 50.f);

		pGraphics->DrawString(line, strLength, _pSubFont, _lineRect, _pStringFormat
			, _pBrush);

		++inputLineStart;
	}
}

bool Letterhead::Initialize(const Vector3& position, int id, Letter* Owner)
{
	_pLetter = Owner;

	SetName("Letterhead");

	_pTransform->SetScale(Vector3(1.f, 1.f, 0.f));
	_pTransform->SetPosition(position);

	if (!InitializeFont())
	{
		std::wcerr << L"Failed to initialize font" << std::endl;
		return false;
	}

	if (!InitializeLinePosition())
	{
		std::wcerr << L"Failed to initialize line position" << std::endl;
		return false;
	}

	_vecTextures.push_back(Engine::FindTexture(L"Item_Letterhead"));
	_vecTextures.push_back(Engine::FindTexture(L"UI_Tutorial"));
	_vecTextures.push_back(Engine::FindTexture(L"UI_RestartButton"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pCollider = AddCollider("LetterButton");
	_pCollider->InitalizeOffset(Vector3(130.f, 265.f, 0.f));
	_pCollider->SetScale(Vector3(160.f, 50.f, 0.f));

	_pCollider = AddCollider("RestartButton");
	_pCollider->InitalizeOffset(Vector3(0.f, 180.f, 0.f));
	_pCollider->SetScale(Vector3(280.f, 80.f, 0.f));
	_pCollider->SetActive(false);

	_loadLetterID = id;

	_pTextLoadSystem = TextLoadSystem::Create();
	_pTextLoadSystem->LoadLetter(L"../Resources/Data/Letter.txt");

	_pLetterStruct = _pTextLoadSystem->GetLetter(_loadLetterID);

	std::string name = "Letter";
	name += std::to_string(_loadLetterID);

	return true;
}

bool Letterhead::InitializeFont()
{
	_pFontCollection = new Gdiplus::PrivateFontCollection();

	_pFontCollection->AddFontFile(L"../Resources/Font/EFKratos.ttf");
	_pFontCollection->AddFontFile(L"../Resources/Font/EFKratosBold.ttf");

	int fontFamilyCount = _pFontCollection->GetFamilyCount();

	FontFamilies = new Gdiplus::FontFamily[fontFamilyCount];

	if (fontFamilyCount <= 0)
	{
		std::wcerr << L"Failed to load font file" << std::endl;
		delete[] FontFamilies;
		return false;
	}

	int numFound = 0;
	_pFontCollection->GetFamilies(fontFamilyCount, FontFamilies, &numFound);
	if (numFound <= 0)
	{
		std::wcerr << L"Failed to get font family" << std::endl;
		delete[] FontFamilies;
		return false;
	}

	_pFontRegulerFamily = &FontFamilies[0];
	_pFontBoldFamily = &FontFamilies[1];

	_pStringFormat = new Gdiplus::StringFormat;
	_pStringFormat->SetAlignment(Gdiplus::StringAlignmentCenter);

	_pMainFont = new Gdiplus::Font(_pFontBoldFamily, 25,
		Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	_pSubFont = new Gdiplus::Font(_pFontRegulerFamily, 14,
		Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	_pBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0));

	return true;
}

bool Letterhead::InitializeLinePosition()
{
	_titleRect = Gdiplus::RectF(
		_pTransform->GetPosition().x - 157.f,
		_pTransform->GetPosition().y - 240.f,
		330.f,
		50.f);

	_lineOffset = Vector3(0.f, 23.f, 0.f);
	Vector3 lineStartPosition = Vector3(_pTransform->GetPosition().x - 170.f, _pTransform->GetPosition().y - 200.f, 0.f);

	for (auto& lines : _contentsLinePosition)
	{
		lines = lineStartPosition;
		lineStartPosition += _lineOffset;
	}

	return true;
}

void Letterhead::SetDrawLetter(Gdiplus::Graphics* pGraphic, const Vector3& _scale, Gdiplus::Bitmap* pBitmap, const Vector3& Position)
{
	int offsetWidth = static_cast<int>(_scale.x * (pBitmap->GetWidth() >> 1));
	int offsetHeight = static_cast<int>(_scale.y * (pBitmap->GetHeight() >> 1));

	_drawRect.X = Position.x - offsetWidth;
	_drawRect.Y = Position.y - offsetHeight;
	_drawRect.Width = offsetWidth << 1;
	_drawRect.Height = offsetHeight << 1;

	pGraphic->DrawImage(pBitmap, _drawRect, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), Gdiplus::UnitPixel, &_imageAttributes);
}

void Letterhead::DrawString(Gdiplus::Graphics* pGraphics, const WCHAR* text, Gdiplus::RectF drawRect, Gdiplus::Font* _pFont)
{
	INT strLength = (INT)wcslen(text);

	pGraphics->DrawString(text, strLength, _pFont, drawRect, _pStringFormat, _pBrush);
}

void Letterhead::SetLetterColorKey(Gdiplus::Color lowColor, Gdiplus::Color highColor)
{
	_imageAttributes.SetColorKey(lowColor, highColor, Gdiplus::ColorAdjustTypeBitmap);
}

void Letterhead::Free()
{
	delete _pBrush;
	delete _pSubFont;
	delete _pMainFont;
	delete _pStringFormat;
	delete[] FontFamilies;
	delete _pFontCollection;

	Engine::SafeRelease(_pTextLoadSystem);
}

Letterhead* Letterhead::Create(const Vector3& position, int id, Letter* Owner)
{
	Letterhead* pInstance = new Letterhead;
	if (pInstance->Initialize(position, id, Owner))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
