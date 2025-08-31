#include "Timer.h"

using namespace Engine;

int Engine::Timer::Update(const float& deltaTime)
{
	for (auto& Timer : _vecTimer)
	{
		if (Timer.isActive)
			Timer.elapsed += deltaTime;
		else
			Timer.elapsed = 0.f;
	}

	return 0;
}

void Engine::Timer::AddTimer(int groupSize)
{
	_vecTimer.resize(groupSize);
}

bool Engine::Timer::IsOverTime(int group, float time)
{
	return time <= _vecTimer[group].elapsed;
}

bool Engine::Timer::IsBetweenTime(int group, float first, float last)
{
	return (first <= _vecTimer[group].elapsed && last >= _vecTimer[group].elapsed);
}

void Engine::Timer::Free()
{
	_vecTimer.clear();
	_vecTimer.shrink_to_fit();
}

Engine::Timer* Engine::Timer::Create(const char* name)
{
	return new Timer(name);
}
