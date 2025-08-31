#include "MiniGame.h"
#include "BitFlag.h"
#include "Timer.h"
#include "Client_Define.h"

int MiniGame::Update(const float& deltaTime)
{
	CompareGaugeWithTargetBar();

	if (Engine::IsKeyPress(Input::DIM_RB))
	{
		_pBitFlag->OnFlag(FlagActive);

		UpdateFill(deltaTime);
	}
	else
	{
		(*_pTimer)[0].isActive = true;
		_pBitFlag->OffFlag(FlagActive);
	}

	UpdateGauge(deltaTime);

	return 0;
}

int MiniGame::LateUpdate(const float& deltaTime)
{
	if ((*_pTimer)[0].elapsed > 0.1f)
	{
		SetDead();
	}

	return 0;
}

void MiniGame::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[4]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[4]);

	RenderTargetBar(pGraphics);

	RenderFill(pGraphics);

	RenderGauge(pGraphics);
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
}

void MiniGame::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void MiniGame::OnCollision(CollisionInfo info)
{
}

Vector3 MiniGame::GetGaugePosition() const
{
	return _gaugePosition;
}

void MiniGame::UpdateGauge(const float& deltaTime)
{
	float oldY = GetGaugePosition().y;
	if (_gaugeCurrentPosition.y <= oldY + _gaugeMaxHeight)
	{
		_gaugeCurrentPosition = Vector3(_pTransform->GetPosition().x,
			oldY + _gaugeMaxHeight, _pTransform->GetPosition().z);
	}
	else if (_pBitFlag->CheckFlag(FlagActive) && _gaugeCurrentPosition.y > oldY + _gaugeMaxHeight)
	{
		_gaugeCurrentPosition = Vector3(_pTransform->GetPosition().x,
			_gaugeCurrentPosition.y - _gaugeSpeed * deltaTime, _pTransform->GetPosition().z);
	}
}

void MiniGame::UpdateFill(const float& deltaTime)
{
	const float fillCorrection = -30.f;
	if (_fillScale >= _topCoordinateY + fillCorrection)
	{
		_fillScale = _topCoordinateY + fillCorrection;
	}
	else if (_fillScale <= _topCoordinateY + fillCorrection)
	{
		_fillScale += _gaugeSpeed * deltaTime;
	}
}

void MiniGame::CompareGaugeWithTargetBar()
{
	float targetMaxPixel = -4.5f * _targetScale.y;
	float targetMinPixel = 4.5f * _targetScale.y;

	if (Engine::IsKeyUp(Input::DIM_RB))
	{
		if (_gaugeCurrentPosition.y < _targetPosition.y + targetMaxPixel)
		{
			_pBitFlag->OnFlag(FlagOverCharge);
			_pBitFlag->OffFlag(FlagSuccess);
			_pBitFlag->OffFlag(FlagFail);

			///\todo delete this codeline before Release
			//std::cout << "OverCharge" << std::endl;
		}
		else if (_gaugeCurrentPosition.y <= _targetPosition.y + targetMinPixel &&
			_gaugeCurrentPosition.y >= _targetPosition.y + targetMaxPixel)
		{
			_pBitFlag->OnFlag(FlagSuccess);
			_pBitFlag->OffFlag(FlagFail);
			_pBitFlag->OffFlag(FlagOverCharge);

			///\todo delete this codeline before Release
			//std::cout << "Success" << std::endl;
		}
		else
		{
			_pBitFlag->OnFlag(FlagFail);
			_pBitFlag->OffFlag(FlagSuccess);
			_pBitFlag->OffFlag(FlagOverCharge);

			///\todo delete this codeline before Release
			//std::cout << "Fail" << std::endl;
		}
	}
}

float MiniGame::RandomHeight()
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(_targetMinHeight, _targetMaxHeight);
	return distrib(gen);
}

void MiniGame::RenderFill(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Color magenta(255, 255, 0, 255); // 마젠타 색상
	Gdiplus::Bitmap* pTransparentBitmap =
		new Gdiplus::Bitmap((*_vecTextures[0])[0]->GetWidth(), (*_vecTextures[0])[2]->GetHeight());

	for (UINT y = 0; y < (*_vecTextures[0])[2]->GetHeight(); ++y) {
		for (UINT x = 0; x < (*_vecTextures[0])[2]->GetWidth(); ++x) {
			Gdiplus::Color color;
			(*_vecTextures[0])[2]->GetPixel(x, y, &color);
			if (color.GetValue() == magenta.GetValue()) {
				color = Gdiplus::Color(0, 0, 0, 0); // 투명한 색으로 변경
			}
			pTransparentBitmap->SetPixel(x, y, color);
		}
	}

	Gdiplus::TextureBrush brush(pTransparentBitmap);

	int offsetWidth = static_cast<int>(_pTransform->GetScale().x * (pTransparentBitmap->GetWidth() >> 1));

	Gdiplus::Rect fillRect;

	fillRect.X = static_cast<INT>(_pTransform->GetPosition().x - offsetWidth);
	fillRect.Y = static_cast<INT>(_pTransform->GetPosition().y + 90.f - _fillScale);
	fillRect.Width = offsetWidth << 1;

	if (_fillScale >= static_cast<INT>(pTransparentBitmap->GetHeight()))
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[2]);
	}
	else
	{
		fillRect.Y = static_cast<INT>(_pTransform->GetPosition().y + 90.f - _fillScale);
		fillRect.Height = static_cast<INT>(_fillScale);

		brush.TranslateTransform(_pTransform->GetPosition().x - pTransparentBitmap->GetWidth() * 0.5f,
			_pTransform->GetPosition().y + pTransparentBitmap->GetHeight() * 0.5f);

		pGraphics->FillRectangle(&brush, fillRect);
	}

	delete pTransparentBitmap;
}

void MiniGame::RenderGauge(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Rect drawRect;

	int offsetGaugeWidth = static_cast<int>(_pTransform->GetScale().x * ((*_vecTextures[0])[3]->GetWidth() >> 1));
	int offsetGaugeHeight = static_cast<int>(_pTransform->GetScale().y * ((*_vecTextures[0])[3]->GetHeight() >> 1));

	drawRect.X = static_cast<INT>(_pTransform->GetPosition().x - offsetGaugeWidth);
	drawRect.Y = static_cast<INT>(_gaugeCurrentPosition.y - offsetGaugeHeight);
	drawRect.Width = offsetGaugeWidth << 1;
	drawRect.Height = offsetGaugeHeight << 1;

	pGraphics->DrawImage((*_vecTextures[0])[3], drawRect, 0, 0,
		(*_vecTextures[0])[3]->GetWidth(), (*_vecTextures[0])[3]->GetHeight(), Gdiplus::UnitPixel);
}

void MiniGame::RenderTargetBar(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Rect drawRect;
	Gdiplus::ImageAttributes imageAttributes;
	Gdiplus::Color lowColor(255, 0, 255);
	Gdiplus::Color highColor(255, 0, 255);

	imageAttributes.SetColorKey(lowColor, highColor, Gdiplus::ColorAdjustTypeBitmap);

	int offsetTargetWidth = static_cast<int>(_targetScale.x * ((*_vecTextures[0])[1]->GetWidth() >> 1));
	int offsetTargetHeight = static_cast<int>(_targetScale.y * ((*_vecTextures[0])[1]->GetHeight() >> 1));

	drawRect.X = static_cast<INT>(_pTransform->GetPosition().x - offsetTargetWidth);
	drawRect.Y = static_cast<INT>(_targetPosition.y - offsetTargetHeight);
	drawRect.Width = offsetTargetWidth << 1;
	drawRect.Height = offsetTargetHeight << 1;

	pGraphics->DrawImage((*_vecTextures[0])[1], drawRect, 0, 0,
		(*_vecTextures[0])[1]->GetWidth(), (*_vecTextures[0])[1]->GetHeight(), Gdiplus::UnitPixel, &imageAttributes);
}

bool MiniGame::IsSuccess()
{
	return _pBitFlag->CheckFlag(FlagSuccess);
}

bool MiniGame::IsFail()
{
	return _pBitFlag->CheckFlag(FlagFail);
}

bool MiniGame::IsOverCharge()
{
	return _pBitFlag->CheckFlag(FlagOverCharge);
}

bool MiniGame::Initialize(const float& targetScale)
{
	const int caseFlashHole = 3;
	Vector3 UI_Offset{};

	if (std::nearbyintf(targetScale) == caseFlashHole)
	{
		UI_Offset = Vector3(380.f, 500.f, -5.f);
	}
	else
	{
		UI_Offset = Vector3(900.f, 600.f, -5.f);
	}

	SetName("MiniGame");
	_pTransform->SetScale(Vector3(1.f, 1.f, 0.f));
	_pTransform->SetPosition(UI_Offset);

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pTimer->AddTimer(1);

	_vecTextures.push_back(Engine::FindTexture(L"UI_Gauge_Frame"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 100, 255));

	float targetHeight = RandomHeight();
	Vector3 targetPositionOffset = Vector3(0.f, targetHeight, 0.f);
	_targetPosition = _pTransform->GetPosition() + targetPositionOffset;

	Vector3 gaugePositionOffset = Vector3(0.f, 90.f, 0.f);
	_gaugePosition = _pTransform->GetPosition() + gaugePositionOffset;
	_gaugeCurrentPosition = _gaugePosition;

	_topCoordinateY = _pTransform->GetPosition().y + 90.f;
	_targetScale = Vector3(1.f, targetScale, 0.f);

	return true;
}

void MiniGame::Free()
{
}

MiniGame* MiniGame::Create(const float& targetScale)
{
	MiniGame* pInstance = new MiniGame;
	if (pInstance->Initialize(targetScale))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
