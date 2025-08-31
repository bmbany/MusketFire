#pragma once
#include "Base.h"

namespace Engine
{
    class Texture : public Base
    {
    private:
        explicit Texture() = default;
        virtual ~Texture() = default;

    public:
        Gdiplus::Bitmap* operator[](const int index) { return _isFlip ? _vecFlipBitmap[index] : _vecOriginBitmap[index]; }
        Gdiplus::Bitmap* GetImage(const int index, bool isFlip) { return isFlip ? _vecFlipBitmap[index] : _vecOriginBitmap[index]; }
        int GetImageCount() { return (int)_vecOriginBitmap.size(); }

    public:
        void AddImage(Gdiplus::Bitmap* pBitmap);
        void AddFlipImage(Gdiplus::Bitmap* pBitmap);
        void FlipImage() { _isFlip = !_isFlip; }

    private:
        // Base을(를) 통해 상속됨
        void Free() override;

    private:
        std::vector<Gdiplus::Bitmap*> _vecOriginBitmap;
        std::vector<Gdiplus::Bitmap*> _vecFlipBitmap;
        bool _isFlip = false;

    public:
        static Texture* Create();
    };
}