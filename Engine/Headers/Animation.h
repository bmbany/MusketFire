#pragma once
#include "Component.h"

namespace Engine
{
    class Animation : public Component
    {
    private:
        struct Frame
        {
            Frame(Gdiplus::Bitmap* pOriginBitmap, Gdiplus::Bitmap* pFlipBitmap, float speed)
                : pOriginBitmap(pOriginBitmap), pFlipBitmap(pFlipBitmap), speed(speed) {}

            Gdiplus::Bitmap* pOriginBitmap = nullptr;
            Gdiplus::Bitmap* pFlipBitmap = nullptr;
            float speed = 0.f;
        };
        using AnimationData = std::vector<std::vector<Frame>>;

    private:
        explicit Animation(const char* name) { _name = name; }
        virtual ~Animation() = default;

    public:
        int Update(const float& deltaTime);
        Gdiplus::Bitmap* GetCurrentFrame();
        void AddFrame(int group, Gdiplus::Bitmap* pOriginBitmap, Gdiplus::Bitmap* pFlipBitmap, float speed);
        bool ChangeAnimation(int nextAnimation);
        void FlipImage(bool isFlip) { _isFlip = isFlip; }
        bool IsLastFrame() { return _isLastFrame; }
        bool IsBetweenFrame(int first, int last);
        void SetFrame(int frame);

    private:
        // Component을(를) 통해 상속됨
        void Free() override;

    public:
        bool Initialize(int count);
        static Animation* Create(const char* name);
        
    private:
        AnimationData _animationData;
        float _currFrame = 0.f;
        int _currAnimation = 0;
        int _currIndex = 0;
        bool _isFlip = false;
        bool _isLastFrame = false;
    };
}