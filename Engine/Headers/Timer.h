#pragma once
#include "Component.h"

namespace Engine
{
    class Timer : public Component
    {
    private:
        typedef struct TimerInfo
        {
            float elapsed = 0.f;
            bool isActive = false;
        }TIMER;
    private:
        explicit Timer(const char* name) { _name = name; }
        virtual ~Timer() = default;

    public:
        virtual int Update(const float& deltaTime);
        void AddTimer(int groupSize);
        bool IsOverTime(int group, float time);
        bool IsBetweenTime(int group, float first, float last);
        void SetActive(int group, bool isActive) { _vecTimer[group].isActive = isActive; }
        void SetElapsed(int group, float elapsed) { _vecTimer[group].elapsed = elapsed; }
        TIMER& operator[](int index) { return _vecTimer[index]; }

    private:
        // CComponent��(��) ���� ��ӵ�
        void Free() override;

    public:
        static Timer* Create(const char* name);

    private:
        std::vector<TimerInfo> _vecTimer;
    };
}