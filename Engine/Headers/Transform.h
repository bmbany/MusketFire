#pragma once
#include "Component.h"

namespace Engine
{
    class Transform : public Component
    {
    public:
        enum Property { Position, Direction, Scale, Property_End };

    private:
        explicit Transform(const char* name) { _name = name; }
        virtual ~Transform() = default;

    public:
        Vector3 GetTransform(Property property) const { return _transform[property]; }
        Vector3 GetPosition() const { return _transform[Position]; }
        Vector3 GetDirection() const { return _transform[Direction]; }
        Vector3 GetScale() const { return _transform[Scale]; }

        void SetTransform(const Vector3& value, Property property) { _transform[property] = value; }
        void SetPosition(const Vector3& position) { _transform[Position] = position;; }
        void SetDirection(const Vector3& direction) { _transform[Direction] = direction;; }
        void SetScale(const Vector3& scale) { _transform[Scale] = scale; }

        void AddTransform(const Vector3& value, Property eTrs) { _transform[eTrs] += value; }
        void AddPosition(const Vector3& position) { _transform[Position] += position;; }
        void AddDirection(const Vector3& direction) { _transform[Direction] += direction;; }
        void AddScale(const Vector3& scale) { _transform[Scale] += scale; }

    private:
        bool Initialize();
        void Free() override;

    public:
        static Transform* Create(const char* name);

    private:
        Vector3 _transform[Property_End];
    };
}