#pragma once

// lightweight physics 2d
namespace lwp
{
    struct Vector2f
    {
        float x{};
        float y{};
    };

    struct Rectangle
    {
        float x{};      // bottom-left X
        float y{};      // bottom-left Y
        float width{};
        float height{};
        Vector2f getPos_TopLeft() const { return {x, y + height}; }
        float getY_Bottom() const { return y; }
        float getY_Top() const { return y + height; }
        float getX_Right() const { return x + width; }
        float getX_Left() const { return x; }
        void setY_Bottom(float val)  { y = val; }
        void setY_Top(float val)  { y = val - height; }
        void setX_Right(float val)  { x = val - width; }
        void setX_Left(float val)  { x = val; }
        Vector2f getCenter() const
        {
            return Vector2f{
                x + width / 2.f,
                y + height / 2.f,
            };
        }
        void move(const Vector2f &delta)
        {
            x += delta.x;
            y += delta.y;
        }
        static Rectangle build_UpLeft(Vector2f pos, Vector2f size)
        {
            Rectangle res;
            res.x = pos.x;
            res.y = pos.y;
            res.width = size.x;
            res.height = size.y;
            return res;
        }
        static Rectangle build_BottomCenter(Vector2f posBottomCenter, Vector2f size)
        {
            Rectangle res;
            res.x = posBottomCenter.x - size.x / 2;
            res.y = posBottomCenter.y;
            res.width = size.x;
            res.height = size.y;
            return res;
        }
    };

    struct RigidBody2D
    {
        Rectangle box{};
        Vector2f speed{};
        bool onFloor = false;
        void applySpeedToBoxPos(float dT)
        {
            box.move({speed.x * dT, speed.y * dT});
        }
        void setOnFloor(float floorY)
        {
            box.setY_Bottom(floorY);
            speed.y = 0;
            onFloor = true;
        }
        bool isOnFloor() { return onFloor; }
    };
}