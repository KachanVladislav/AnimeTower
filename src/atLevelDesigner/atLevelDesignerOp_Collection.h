#pragma once
#include "atLevelDesignerMode_Interface.h"
#include "../atLevel/atLevelData.h"
#include "../atRender/atRenderLevel.h"
#include "../atConvert_lwpToRaylib.h"
#include <algorithm>
#include <vector>

struct atLevelDesignerOp_Interface
{
    virtual ~atLevelDesignerOp_Interface() = default;
    virtual void apply(atLevelDesigner_Data &data) = 0;
    virtual void undo(atLevelDesigner_Data &data) = 0;
    virtual const char *getTypeName() const = 0;
};

struct atLevelDesignerOp_DeleteObjects : public atLevelDesignerOp_Interface
{
    std::vector<std::pair<atObjectRefToVector, atObject>> objectsCollection;

    atLevelDesignerOp_DeleteObjects(std::vector<atObjectRefToVector> &objectsRefs)
    {
        objectsCollection.reserve(objectsRefs.size());
        for (auto &objRef : objectsRefs)
        {
            atObject *obj = objRef.resolve();
            if (obj)
            {
                objectsCollection.push_back({objRef, *obj});
            }
        }
    }

    void apply(atLevelDesigner_Data &) override
    {
        for (auto &objPair : objectsCollection)
        {
            auto &vec = *objPair.first.vec_ptr;
            uint32_t id = objPair.first.id;
            for (auto it = vec.begin(); it != vec.end(); ++it)
            {
                if (it->id == id)
                {
                    vec.erase(it);
                    break;
                }
            }
        }
    }

    void undo(atLevelDesigner_Data &) override
    {
        for (auto &objPair : objectsCollection)
        {
            auto &vec = *objPair.first.vec_ptr;
            vec.push_back(objPair.second);
        }
    }

    const char *getTypeName() const override { return "DeletePlatforms"; }
};

struct atLevelDesignerOp_CreateObject : public atLevelDesignerOp_Interface
{
    atObject platform;
    uint32_t createdId = 0;

    atLevelDesignerOp_CreateObject(atObject rect)
        : platform(rect)
    {
    }

    void apply(atLevelDesigner_Data &data) override
    {
        data.levelData.objectBuilder.setUniqueId(platform);
        createdId = platform.id;
        data.levelData.objectsStatic.push_back(platform);
    }

    void undo(atLevelDesigner_Data &data) override
    {
        for (auto it = data.levelData.objectsStatic.begin(); it != data.levelData.objectsStatic.end(); ++it)
        {
            if (it->id == createdId)
            {
                data.levelData.objectsStatic.erase(it);
                break;
            }
        }
    }

    const char *getTypeName() const override { return "CreatePlatform"; }
};
