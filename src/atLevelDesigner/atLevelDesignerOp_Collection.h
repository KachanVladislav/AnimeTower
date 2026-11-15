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
    std::vector<lwp::Rectangle> platforms;
    std::vector<int> originalIndices;

    atLevelDesignerOp_DeletePlatforms(std::vector<lwp::Rectangle> rects, std::vector<int> indices)
        : platforms(std::move(rects)), originalIndices(std::move(indices))
    {
    }

    void apply(atLevelDesigner_Data &data) override
    {
        std::vector<int> sortedIndices = originalIndices;
        std::sort(sortedIndices.begin(), sortedIndices.end());
        for (int i = static_cast<int>(sortedIndices.size()) - 1; i >= 0; --i)
        {
            data.levelData.horizontalPlatforms_lwp.erase(
                data.levelData.horizontalPlatforms_lwp.begin() + sortedIndices[i]);
        }
        data.renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(data.levelData.horizontalPlatforms_lwp);
    }

    void undo(atLevelDesigner_Data &data) override
    {
        for (size_t i = 0; i < platforms.size(); ++i)
        {
            data.levelData.horizontalPlatforms_lwp.insert(
                data.levelData.horizontalPlatforms_lwp.begin() + originalIndices[i],
                platforms[i]);
        }
        data.renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(data.levelData.horizontalPlatforms_lwp);
    }

    const char *getTypeName() const override { return "DeletePlatforms"; }
};

struct atLevelDesignerOp_CreatePlatform : public atLevelDesignerOp_Interface
{
    lwp::Rectangle platform;
    int createdIndex;

    atLevelDesignerOp_CreatePlatform(lwp::Rectangle rect, int index)
        : platform(rect), createdIndex(index)
    {
    }

    void apply(atLevelDesigner_Data &data) override
    {
        data.levelData.horizontalPlatforms_lwp.push_back(platform);
        data.renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(data.levelData.horizontalPlatforms_lwp);
    }

    void undo(atLevelDesigner_Data &data) override
    {
        data.levelData.horizontalPlatforms_lwp.erase(
            data.levelData.horizontalPlatforms_lwp.begin() + createdIndex);
        data.renderData.horizontalPlatforms_raylib = atConvert_lwpToRaylib::buildRectsVector(data.levelData.horizontalPlatforms_lwp);
    }

    const char *getTypeName() const override { return "CreatePlatform"; }
};
