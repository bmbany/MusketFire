#include "Bubble.h"
#include "BitFlag.h"
#include "TextLoadSystem.h"
#include "Client_Define.h"
#include "Timer.h"

#include <random>
#include <ctime>
#include <comdef.h>

int Bubble::Update(const float& deltaTime)
{

	return 0;
}

int Bubble::LateUpdate(const float& deltaTime)
{
	if ((*_pTimer)[Enemy].elapsed > 1.f)
	{
		if (_pBitFlag->CheckFlag(EnemySpeak))
		{
			_pBitFlag->OffFlag(EnemySpeak | RenderingEnemy);
			_oldEnemyMassageID = 0;
			(*_pTimer)[Enemy].isActive = false;
		}
	}

	if ((*_pTimer)[Alies].elapsed > 1.f)
	{
		if (_pBitFlag->CheckFlag(AliesSpeak))
		{
			_pBitFlag->OffFlag(AliesSpeak | RenderingAlies);
			_oldAliesMassageID = 0;
			(*_pTimer)[Alies].isActive = false;
		}
	}

	if ((*_pTimer)[SoundEnemy].elapsed > 2.5f)
	{
		_pBitFlag->OffFlag(PlaySoundEnemy);
		(*_pTimer)[SoundEnemy].isActive = false;
	}

	if ((*_pTimer)[SoundAlies].elapsed > 2.5f)
	{
		_pBitFlag->OffFlag(PlaySoundAlies);
		(*_pTimer)[SoundAlies].isActive = false;
	}

	return 0;
}

void Bubble::Render(Gdiplus::Graphics* pGraphics)
{
	if (_pBitFlag->CheckFlag(AliesSpeak))
	{
		(*_pTimer)[Alies].isActive = true;
		SetDrawBubble(pGraphics, (*_vecTextures[0])[Alies], _bubbleAliesPosition);
		_pBitFlag->OnFlag(RenderingAlies);
	}

	if (_pBitFlag->CheckFlag(EnemySpeak))
	{
		(*_pTimer)[Enemy].isActive = true;
		SetDrawBubble(pGraphics, (*_vecTextures[0])[Enemy], _bubbleEnemyPosition);
		_pBitFlag->OnFlag(RenderingEnemy);
	}

	DrawMessage(pGraphics, _currentIFF_Code);
}

void Bubble::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

bool Bubble::Initialize(const Vector3& position, int id)
{
	_thisBubbleID = id;

	srand((unsigned int)time(nullptr));

	SetName("Bubble");

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

	_pTransform->SetScale(Vector3(0.8f, 0.8f, 0.f));
	_pTransform->SetPosition(position);

	_vecTextures.push_back(Engine::FindTexture(L"UI_Bubble"));

	SetBubbleColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pTextAliesSystem = TextLoadSystem::Create();
	_pTextEnemySystem = TextLoadSystem::Create();
	_pSoundAliesSystem = TextLoadSystem::Create();
	_pSoundEnemySystem = TextLoadSystem::Create();

	_pTextAliesSystem->LoadLetter(L"../Resources/Data/Message/Alies.txt");
	_pTextEnemySystem->LoadLetter(L"../Resources/Data/Message/Enemy.txt");
	_pSoundAliesSystem->LoadLetter(L"../Resources/Data/Message/AliesSound.txt");
	_pSoundEnemySystem->LoadLetter(L"../Resources/Data/Message/EnemySound.txt");

	_pTimer->AddTimer(End);

	return true;
}

bool Bubble::InitializeFont()
{
	_pFontCollection = new Gdiplus::PrivateFontCollection();

	_pFontCollection->AddFontFile(L"../Resources/Font/EFKratosBold.ttf");

	int fontFamilyCount = _pFontCollection->GetFamilyCount();

	_pFontFamily = new Gdiplus::FontFamily;

	if (fontFamilyCount <= 0)
	{
		std::wcerr << L"Failed to load font file" << std::endl;
		return false;
	}

	int numFound = 0;
	_pFontCollection->GetFamilies(fontFamilyCount, _pFontFamily, &numFound);
	if (numFound <= 0)
	{
		std::wcerr << L"Failed to get font family" << std::endl;
		return false;
	}

	_pStringFormat = new Gdiplus::StringFormat;
	_pStringFormat->SetAlignment(Gdiplus::StringAlignmentCenter);

	_pFont = new Gdiplus::Font(_pFontFamily, 24,
		Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	_pBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0));

	return true;
}

bool Bubble::InitializeLinePosition()
{
	_bubbleAliesPosition = Vector3(176.f, 60.f, 0.f);
	_bubbleEnemyPosition = Vector3(1116.f, 60.f, 0.f);

	_bubbleContentsAliesPosition = Vector3(-140.f, -15.f, 0.f);
	_bubbleContentsEnemyPosition = Vector3(-170.f, -15.f, 0.f);

	_bubbleAliesRect = Gdiplus::RectF(
		_bubbleAliesPosition.x + _bubbleContentsAliesPosition.x,
		_bubbleAliesPosition.y + _bubbleContentsAliesPosition.y,
		300.f,
		50.f
	);

	_bubbleEnemyRect = Gdiplus::RectF(
		_bubbleEnemyPosition.x + _bubbleContentsEnemyPosition.x,
		_bubbleEnemyPosition.y + _bubbleContentsEnemyPosition.y,
		300.f,
		50.f
	);

	return true;
}

void Bubble::SetDrawBubble(Gdiplus::Graphics* pGraphic, Gdiplus::Bitmap* pBitmap, const Vector3& Position)
{
	int offsetWidth = static_cast<int>(_pTransform->GetScale().x * (pBitmap->GetWidth() >> 1));
	int offsetHeight = static_cast<int>(_pTransform->GetScale().y * (pBitmap->GetHeight() >> 1));

	_drawRect.X = static_cast<INT>(Position.x - offsetWidth);
	_drawRect.Y = static_cast<INT>(Position.y - offsetHeight);
	_drawRect.Width = offsetWidth << 1;
	_drawRect.Height = offsetHeight << 1;

	pGraphic->DrawImage(pBitmap, _drawRect, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), Gdiplus::UnitPixel, &_imageAttributes);
}

void Bubble::DrawMessage(Gdiplus::Graphics* pGraphics, int IFF_code)
{
	// Draw Alies Message
	if (_pBitFlag->CheckFlag(AliesSpeak)/* && _currentIFF_Code != Enemy*/)
	{
		_pTextAlies = _pTextAliesSystem->GetLetter(_currentAliesMassageID);
		_pSoundAlies = _pSoundAliesSystem->GetLetter(_currentAliesMassageID);

		INT strLength = (INT)wcslen(_pTextAlies->_letterTitle);

		pGraphics->DrawString(_pTextAlies->_letterTitle, strLength, _pFont,
			_bubbleAliesRect, _pStringFormat, _pBrush);

		_bstr_t convertID(_pSoundAlies->_letterTitle);
		const char* pSoundID = convertID;

		if (_oldAliesMassageID != _currentAliesMassageID && !_pBitFlag->CheckFlag(PlaySoundAlies))
		{
			Engine::MyPlaySound(pSoundID, (int)SoundGroup::PlayerVoice);
			_oldAliesMassageID = _currentAliesMassageID;
			_pBitFlag->OnFlag(PlaySoundAlies);
			(*_pTimer)[SoundAlies].isActive = true;
		}
	}

	// Draw Enemy Message
	if (_pBitFlag->CheckFlag(EnemySpeak)/* && _currentIFF_Code != Alies*/)
	{
		_pTextEnemy = _pTextEnemySystem->GetLetter(_currentEnemyMassageID);
		_pSoundEnemy = _pSoundEnemySystem->GetLetter(_currentEnemyMassageID);

		INT strLength = (INT)wcslen(_pTextEnemy->_letterTitle);

		pGraphics->DrawString(_pTextEnemy->_letterTitle, strLength, _pFont,
			_bubbleEnemyRect, _pStringFormat, _pBrush);

		_bstr_t convertID(_pSoundEnemy->_letterTitle);
		const char* pSoundID = convertID;

		if (_oldEnemyMassageID != _currentEnemyMassageID && !_pBitFlag->CheckFlag(PlaySoundEnemy))
		{
			Engine::MyPlaySound(pSoundID, (int)SoundGroup::EnemyVoice);
			_oldEnemyMassageID = _currentEnemyMassageID;
			_pBitFlag->OnFlag(PlaySoundEnemy);
			(*_pTimer)[SoundEnemy].isActive = true;
		}
	}
}

void Bubble::SetBubbleColorKey(Gdiplus::Color lowColor, Gdiplus::Color highColor)
{
	_imageAttributes.SetColorKey(lowColor, highColor, Gdiplus::ColorAdjustTypeBitmap);
}

void Bubble::BubbleSpeak(int IFF_code, int massageID)
{
	if (IFF_code == Alies)
	{
		_currentIFF_Code = Alies;
		_pBitFlag->OnFlag(AliesSpeak);
		_currentAliesMassageID = massageID;
	}
	else if (IFF_code == Enemy)
	{
		_currentIFF_Code = Enemy;
		_pBitFlag->OnFlag(EnemySpeak);
		_currentEnemyMassageID = massageID;
	}
}

void Bubble::BubbleRangedSpeak(int IFF_Code, int minID, int maxID)
{
	if (IFF_Code == Alies && !_pBitFlag->CheckFlag(RenderingAlies))
	{
		_currentIFF_Code = Alies;
		_pBitFlag->OnFlag(AliesSpeak);
		_currentAliesMassageID = rand() % (maxID - minID + 1) + minID;
	}
	else if (IFF_Code == Enemy && !_pBitFlag->CheckFlag(RenderingEnemy))
	{
		_currentIFF_Code = Enemy;
		_pBitFlag->OnFlag(EnemySpeak);
		_currentEnemyMassageID = rand() % (maxID - minID + 1) + minID;
	}
}

bool Bubble::IsBubbleRendering(int IFF_code)
{
	if (IFF_code == Alies)
	{
		return _pBitFlag->CheckFlag(RenderingAlies);
	}
	else if (IFF_code == Enemy)
	{
		return _pBitFlag->CheckFlag(RenderingEnemy);
	}

	return false;
}

int Bubble::getCurrentAliesMassageID()
{
	return _currentAliesMassageID;
}

int Bubble::getCurrentEnemyMassageID()
{
	return _currentEnemyMassageID;
}

void Bubble::Free()
{
	Engine::SafeRelease(_pTextAliesSystem);
	Engine::SafeRelease(_pTextEnemySystem);
	Engine::SafeRelease(_pSoundEnemySystem);
	Engine::SafeRelease(_pSoundAliesSystem);
}

Bubble* Bubble::Create(const Vector3& position, int id)
{
	Bubble* pInstance = new Bubble;
	if (pInstance->Initialize(position, id))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
