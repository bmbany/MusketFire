#include "FireStepText.h"
#include "Client_Define.h"
#include "Flintlock.h"
#include "CockPin.h"
#include "TextLoadSystem.h"

#include <iostream>
#include <strsafe.h>
#include <comdef.h>

int FireStepText::Update(const float& deltaTime)
{
	return 0;
}

int FireStepText::LateUpdate(const float& deltaTime)
{
	return 0;
}

void FireStepText::Render(Gdiplus::Graphics* pGraphics)
{
	const WCHAR* text[] = {
		L"상태 : 장전", L"상태 : 장전 완료", L"총구에 화약 넣기 [홀드 LMB > RMB]",
		L"접시에 화약 넣기 [홀드 LMB > RMB]", L"총알 총구에 올리기 [홀드 LMB > RMB]",
		L"막대로 총알 밀어넣기 [홀드 RMB]", L"코킹 [LMB]"
	};

	Vector3 position = _pTransform->GetPosition() + Vector3(0.f, 30.f, 0.f);
	Vector3 OffsetX = Vector3(120.f, 0.f, 0.f);
	Vector3 OffsetY = Vector3(0.f, 30.f, 0.f);

	Gdiplus::Color ActiveColor = Gdiplus::Color(80, 15, 15);
	Gdiplus::Color InActiveColor = Gdiplus::Color(191, 33, 33);
	Gdiplus::Color Black = Gdiplus::Color(0, 0, 0);

	_pBrush->SetColor(Black);
	DrawString(pGraphics, _pTextLoadSystem->GetLetter(8)->_letterTitle, position + Vector3(0.f, -210.f, 0.f));
	DrawString(pGraphics, _pTextLoadSystem->GetLetter(9)->_letterTitle, position + Vector3(0.f, -190.f, 0.f));

	if (_pFlintlock->GetCurrentState() != (int)Flintlock::ReloadAndFireStep::Aim)
	{
		_pBrush->SetColor(Black);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(1)->_letterTitle, position);
	}
	else
	{
		_pBrush->SetColor(Black);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(2)->_letterTitle, position);
	}

	int plintDurablity = _pCockPin->CheckDurablity();

	const int _bufferSize = 256; //보류
	WCHAR buffer[_bufferSize];
	int bufferSizeInBytes = _bufferSize * sizeof(wchar_t);

	const WCHAR* beforeformat = _pTextLoadSystem->GetLetter(10)->_letterTitle;
	_bstr_t convertFormat = beforeformat;
	_bstr_t bstrFormat = convertFormat + " : %d";

	LPCTSTR format = bstrFormat;

	StringCbPrintfW(buffer, bufferSizeInBytes, format, plintDurablity);
	_pBrush->SetColor(Black);
	DrawString(pGraphics, buffer, position + OffsetY);

	if (!_pFlintlock->CheckStateFlag(Flintlock::Flag::FlagMuzzlePowder))
	{
		_pBrush->SetColor(InActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(3)->_letterTitle, position + OffsetY * 2);
	}
	else
	{
		_pBrush->SetColor(ActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(3)->_letterTitle, position + OffsetY * 2);
	}

	if (!_pFlintlock->CheckStateFlag(Flintlock::Flag::FlagFlashHolePowder))
	{
		_pBrush->SetColor(InActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(4)->_letterTitle, position + OffsetY * 3);
	}
	else
	{
		_pBrush->SetColor(ActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(4)->_letterTitle, position + OffsetY * 3);
	}

	if (!_pFlintlock->CheckStateFlag(Flintlock::Flag::FlagUseAmmunition))
	{
		_pBrush->SetColor(InActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(5)->_letterTitle, position + OffsetY * 4);
	}
	else
	{
		_pBrush->SetColor(ActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(5)->_letterTitle, position + OffsetY * 4);
	}

	if (!_pFlintlock->CheckStateFlag(Flintlock::Flag::FlagRamComplete))
	{
		_pBrush->SetColor(InActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(6)->_letterTitle, position + OffsetY * 5);
	}
	else
	{
		_pBrush->SetColor(ActiveColor);
		DrawString(pGraphics, _pTextLoadSystem->GetLetter(6)->_letterTitle, position + OffsetY * 5);
	}

}

void FireStepText::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void FireStepText::DrawString(Gdiplus::Graphics* pGraphics,
	const WCHAR* text, const Vector3& position)
{
	INT strLength = (INT)wcslen(text);

	pGraphics->DrawString(text, strLength, _pFont,
		Gdiplus::PointF(position.x, position.y), _pBrush);
}

void FireStepText::SetTarget()
{
	_pFlintlock = dynamic_cast<Flintlock*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "Flintlock"));
	_pCockPin = dynamic_cast<CockPin*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "CockPin"));
}

bool FireStepText::Initialize(const Vector3& position)
{
	SetName("FireSteopText");

	_pTransform->SetPosition(position);

	_pFontCollection = new Gdiplus::PrivateFontCollection();
	_pFontFamily = new Gdiplus::FontFamily;
	_pFontCollection->AddFontFile(L"../Resources/Font/EFKratosBold.ttf");

	int fontFamilyCount = _pFontCollection->GetFamilyCount();
	if (fontFamilyCount <= 0)
	{
		std::wcerr << L"Failed to load font file" << std::endl;
		return false;
	}

	int numFound = 0;
	_pFontCollection->GetFamilies(1, _pFontFamily, &numFound);

	if (numFound <= 0)
	{
		std::wcerr << L"Failed to get font family" << std::endl;
		return false;
	}

	_pFont = new Gdiplus::Font(_pFontFamily, 16,
		Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	_pBrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255));

	_pTextLoadSystem = TextLoadSystem::Create();
	_pTextLoadSystem->LoadLetter(L"../Resources/Data/Guide.txt");

	SetTarget();

	return true;
}

void FireStepText::Free()
{
	delete _pFontCollection;
	delete _pFont;
	delete _pBrush;
	
	Engine::SafeRelease(_pTextLoadSystem);
}

FireStepText* FireStepText::Create(const Vector3& position)
{
	FireStepText* pInstance = new FireStepText;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
