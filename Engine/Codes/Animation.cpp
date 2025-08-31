#include "Animation.h"

using namespace Engine;

int Engine::Animation::Update(const float& deltaTime)
{
	_isLastFrame = false;
	Frame& currFrame = _animationData[_currAnimation][_currIndex];
	_currFrame += deltaTime * currFrame.speed;

	if (1.f <= _currFrame)
	{
		_currFrame = 0.f;
		_currIndex++;
	}

	if (_currIndex >= _animationData[_currAnimation].size())
	{
		_isLastFrame = true;
		_currIndex = 0;
	}

	return 0;
}

Gdiplus::Bitmap* Engine::Animation::GetCurrentFrame()
{	 
	return _isFlip ? _animationData[_currAnimation][_currIndex].pFlipBitmap :
					 _animationData[_currAnimation][_currIndex].pOriginBitmap;
}

void Engine::Animation::AddFrame(int group, Gdiplus::Bitmap* pOriginBitmap, Gdiplus::Bitmap* pFlipBitmap, float speed)
{
	_animationData[group].push_back(Frame(pOriginBitmap, pFlipBitmap, 1.f / speed));
}

bool Engine::Animation::ChangeAnimation(int nextAnimation)
{
	if (_currAnimation != nextAnimation)
	{
		_currAnimation = nextAnimation;
		_currIndex = 0;
		_isLastFrame = false;

		return true;
	}

	return false;
}

bool Engine::Animation::IsBetweenFrame(int first, int last)
{
	return first <= _currIndex && last >= _currIndex;
}

void Engine::Animation::SetFrame(int frame)
{
	_currIndex = frame;
	_currFrame = 0.f;
}

void Engine::Animation::Free()
{
	for (auto& Data : _animationData)
	{
		Data.clear();
		Data.shrink_to_fit();
	}

	_animationData.clear();
	_animationData.shrink_to_fit();
}

bool Engine::Animation::Initialize(int count)
{
	_animationData.resize(count);

	return true;
}

Animation* Engine::Animation::Create(const char* name)
{
	return new Animation(name);
}
