#pragma once
#include "atObject.h"
#include "atDefaults.h"

class atObjectUniqueIdBuilder
{
    uint32_t _id_next = 0;
    uint32_t getNextId() { return _id_next++; }

public:
    void setUniqueId(atObject& obj)
    {
        obj.id = getNextId();
    }
    atObject player(float x, float y)
    {
        atObject res;
        res.id = getNextId();
        res.transform.pos.x = x;
        res.transform.pos.y = y;
        res.collider.size.x = atDefaults::playerW_lwp;
        res.collider.size.y = atDefaults::playerH_lwp;
        return res;
    }
    atObject fromColliderRect(lwp::Rectangle rect)
    {
        atObject res;
        res.id = getNextId();
        res.collider.size.x = rect.width;
        res.collider.size.y = rect.height;
        res.transform.pos.x = rect.x;
        res.transform.pos.y = rect.y;
        return res;
    }
};