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

struct atLevelDesignerOp_DeletePlatforms : public atLevelDesignerOp_Interface
{
    std::vector<atObject> platforms;
    std::vector<uint32_t> originalIds;

    atLevelDesignerOp_DeletePlatforms(std::vector<atObject> rects, std::vector<uint32_t> ids)
        : platforms(std::move(rects)), originalIds(std::move(ids))
    {
    }

    void apply(atLevelDesigner_Data &data) override
    {
        for (uint32_t id : originalIds)
        {
            for (auto it = data.levelData.objectsStatic.begin(); it != data.levelData.objectsStatic.end(); ++it)
            {
                if (it->id == id)
                {
                    data.levelData.objectsStatic.erase(it);
                    break;
                }
            }
        }
    }

    void undo(atLevelDesigner_Data &data) override
    {
        originalIds.clear();
        for (atObject obj : platforms)
        {
            data.levelData.objectBuilder.setUniqueId(obj);
            originalIds.push_back(obj.id);
            data.levelData.objectsStatic.push_back(obj);
        }
    }

    const char *getTypeName() const override { return "DeletePlatforms"; }
};

struct atLevelDesignerOp_CreatePlatform : public atLevelDesignerOp_Interface
{
    atObject platform;
    uint32_t createdId = 0;

    atLevelDesignerOp_CreatePlatform(atObject rect)
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
