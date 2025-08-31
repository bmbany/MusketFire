#pragma once
#include "Component.h"

namespace Engine
{
    class Transform;
    class BitmapRenderer : public Component
    {
    private:
        struct BitmapInfo
        {
            HDC hdc;
            BITMAP bitamp;
        };
    private:
        explicit BitmapRenderer(const char* name) { _name = name; }
        virtual ~BitmapRenderer() = default;

    public:
        void Initialize(Gdiplus::Bitmap* pBitmap, HDC originHDC, HDC targetHDC, Transform* pTarget, bool isTransparent = false);
        void Initialize(HDC targetHDC, Transform* pTarget, bool isTransparent = false);
        void AddBitmap(Gdiplus::Bitmap* pBitmap, HDC originHDC);
        void SetDrawInformation(const Vector3& cameraPosition = Vector3(0.f, 0.f, 0.f));
        void SetDrawInformation(int index, const Vector3& cameraPosition = Vector3(0.f, 0.f, 0.f));
        void Draw();
        void Draw(int index);
    private:
        // Component을(를) 통해 상속됨
        void Free() override;

    public:
        static BitmapRenderer* Create(const char* name);

    private:
        std::vector<BitmapInfo> _bitmapInfo;
        Transform*      _pTransform = nullptr;
        HDC             _hDC = nullptr;
        HDC             _targetHDC = nullptr;
        HBITMAP         _hBitmap = nullptr;
        BITMAP          _bitmap;
        Gdiplus::Rect   _drawRect;
        bool            _isTransparent = false;
    };
}