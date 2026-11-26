#pragma once
#include "atObjectRef.h"

struct atRenderObject // would be fat?
{
    atObjectRef objRef;
    lwp::Vector2f offset;
    lwp::Vector2f size;
    bool isFlipX = false;
};

struct atRenderObjectCollection
{
    std::vector<atRenderObject> vec;
    atStringTemplate<32> tag;
};

struct atRenderObjectLayer
{
    atRenderObjectCollection platforms;
    atRenderObjectCollection sprites;
    atRenderObjectCollection animated;
    std::array<atRenderObjectCollection *, 3> collections()
    {
        return {&platforms, &sprites, &animated};
    }
    atRenderObjectLayer()
    {
        platforms.tag = "platforms";
        sprites.tag = "sprites";
        animated.tag = "animated";
    }
};

struct atRenderObjectRef
{
    uint32_t objId{};
    size_t objectIndex{};
    size_t collectionIndex{};
    size_t layerIndex{};
    std::vector<atRenderObjectLayer> *renderLayers = nullptr;
    atRenderObjectRef() = default;
    atRenderObjectRef(std::vector<atRenderObjectLayer> *renderLayers, uint32_t objId) : renderLayers(renderLayers), objId(objId){}

    atRenderObjectCollection *resolveCollection()
    {
        if(resolve())
            return renderLayers->at(layerIndex).collections()[collectionIndex];
        return nullptr;
    }

    atRenderObject *resolve()
    {
        if (layerIndex < renderLayers->size())
        {
            atRenderObjectLayer &layer = renderLayers->at(layerIndex);
            if (collectionIndex < layer.collections().size())
            {
                atRenderObjectCollection *collection = layer.collections()[collectionIndex];
                if (objectIndex < collection->vec.size())
                {
                    if (collection->vec[objectIndex].objRef.id == objId)
                        return &collection->vec[objectIndex];
                }
            }
        }
        return fullSearch();
    }

private:
    atRenderObject *fullSearch()
    {
        for (layerIndex = 0; layerIndex < renderLayers->size(); layerIndex++)
        {
            atRenderObjectLayer &layer = renderLayers->at(layerIndex);
            for (collectionIndex = 0; collectionIndex < layer.collections().size(); collectionIndex++)
            {
                atRenderObjectCollection *collection = layer.collections()[collectionIndex];
                for (objectIndex = 0; objectIndex < collection->vec.size(); objectIndex++)
                {
                    if (collection->vec[objectIndex].objRef.id == objId)
                        return &collection->vec[objectIndex];
                }
            }
        }
        return nullptr;
    }
};