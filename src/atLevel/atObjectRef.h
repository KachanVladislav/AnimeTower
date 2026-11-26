#pragma once
#include "atObject.h"
#include <span>
#include <vector>

struct atObjectRef
{
    uint32_t id = 0;
    size_t layerIndex = 0;
    size_t collectionIndex = 0;
    size_t objectIndex = 0;
    std::vector<atObjectsCollectionLayer> *layers;

    atObjectRef() = default;
    atObjectRef(std::vector<atObjectsCollectionLayer> *layers, uint32_t id) : layers(layers), id(id) {}
    atObjectRef(std::vector<atObjectsCollectionLayer> *layers, size_t layerIndex, size_t collectionIndex, size_t objectIndex)
        : layers(layers), layerIndex(layerIndex), collectionIndex(collectionIndex), objectIndex(objectIndex)
    {
        if (layerIndex < layers->size())
        {
            atObjectsCollectionLayer &layer = layers->at(layerIndex);
            if (collectionIndex < layer.collections().size())
            {
                atObjectsCollection &collection = *layer.collections()[collectionIndex];
                if (objectIndex < collection.vec.size())
                {
                    atObject &obj = collection.vec[objectIndex];
                    id = obj.id;
                }
            }
        }
    }

    atObjectsCollectionLayer* resolveLayer()
    {
        if(resolve() == nullptr)
            return nullptr;
        return &layers->at(layerIndex);
    }

    atObjectsCollection *resolveCollection()
    {
        if(resolve() == nullptr)
            return nullptr;
        return layers->at(layerIndex).collections()[collectionIndex];
    }

    atObject *resolve()
    {
        if (layerIndex < layers->size())
        {
            atObjectsCollectionLayer &layer = layers->at(layerIndex);
            if (collectionIndex < layer.collections().size())
            {
                atObjectsCollection &collection = *layer.collections()[collectionIndex];
                if (objectIndex < collection.vec.size())
                {
                    atObject &obj = collection.vec[objectIndex];
                    if (obj.id == id)
                        return &collection.vec[objectIndex];
                }
            }
        }
        return fullSearch();
    }

private:
    atObject *fullSearch()
    {
        for (layerIndex = 0; layerIndex < layers->size(); layerIndex++)
        {
            atObjectsCollectionLayer &layer = layers->at(layerIndex);
            for (collectionIndex = 0; collectionIndex < layer.collections().size(); collectionIndex++)
            {
                atObjectsCollection &collection = *layer.collections()[collectionIndex];
                for (objectIndex = 0; objectIndex < collection.vec.size(); objectIndex++)
                {
                    atObject &obj = collection.vec[objectIndex];
                    if (obj.id == id)
                        return &obj;
                }
            }
        }
        return nullptr;
    }
};
