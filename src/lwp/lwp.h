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
        Vector2f bottomLeftPos{}; // bottom-left corner (physics)
        Vector2f size{};          // width, height
        Vector2f getPos_TopLeft() const { return {bottomLeftPos.x, bottomLeftPos.y + size.y}; }
        float getY_Bottom() const { return bottomLeftPos.y; }
        float getY_Top() const { return bottomLeftPos.y + size.y; }
        float getX_Right() const { return bottomLeftPos.x + size.x; }
        float getX_Left() const { return bottomLeftPos.x; }
        void setY_Bottom(float val)  { bottomLeftPos.y = val; }
        void setY_Top(float val)  { bottomLeftPos.y = val - size.y; }
        void setX_Right(float val)  { bottomLeftPos.x = val - size.x; }
        void setX_Left(float val)  { bottomLeftPos.x = val; }
        Vector2f getCenter() const
        {
            return Vector2f{
                bottomLeftPos.x + size.x / 2.f,
                bottomLeftPos.y + size.y / 2.f,
            };
        }
        void move(const Vector2f &delta)
        {
            bottomLeftPos.x += delta.x;
            bottomLeftPos.y += delta.y;
        }
        static Rectangle build_UpLeft(Vector2f pos, Vector2f size)
        {
            Rectangle res;
            res.bottomLeftPos = Vector2f{pos.x, pos.y - size.y};
            res.size = size;
            return res;
        }
        static Rectangle build_BottomCenter(Vector2f posBottomCenter, Vector2f size)
        {
            Rectangle res;
            res.bottomLeftPos.x = posBottomCenter.x - size.x / 2;
            res.bottomLeftPos.y = posBottomCenter.y;
            res.size = size;
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