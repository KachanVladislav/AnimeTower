#pragma once
#include "lwp/lwp.h"
#include "atStringTemplate.h"
#include <array>

struct atTransform
{
    lwp::Vector2f pos;
};

struct atColliderRect
{
    lwp::Vector2f offset;
    lwp::Vector2f size;
};

struct atRigidBody2D
{
    lwp::Vector2f speed{};
    bool isOnFloor = false;
};

struct atAnimationData
{
    lwp::Vector2f offset{};
    lwp::Vector2f size{};
};

struct atObject
{
    uint32_t id = 0;
    atTransform transform;
    atColliderRect collider;
    atRigidBody2D rigidBody;
    bool isFlipX = false;
    atAnimationData animationData;
    lwp::Vector2f getPos_UpLeft() const { return {transform.pos.x, transform.pos.y + collider.size.y}; }
    lwp::Vector2f getTextureWorldPos_UpLeft() const
    {
        return {transform.pos.x + animationData.offset.x,
                transform.pos.y + animationData.offset.y + animationData.size.y};
    }
    lwp::Rectangle getColliderRect() const
    {
        return {transform.pos.x + collider.offset.x,
                transform.pos.y + collider.offset.y,
                collider.size.x,
                collider.size.y};
    }
};

struct atObjectsCollection
{
    atStringTemplate<32> tag;
    std::vector<atObject> vec;
};

struct atObjectsCollectionLayer
{
    atStringTemplate<32> tag;
    atObjectsCollection colStatic;
    atObjectsCollection colRigid;
    atObjectsCollection colNotCollidable;
    std::array<atObjectsCollection *, 3> collections()
    {
        return {&colStatic, &colRigid, &colNotCollidable};
    }
    atObjectsCollectionLayer()
    {
        tag = "layer";
        colStatic.tag = "static";
        colRigid.tag = "rigid";
        colNotCollidable.tag = "notCollidable";
    }
};