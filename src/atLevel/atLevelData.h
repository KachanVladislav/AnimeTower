#pragma once
#include <vector>
#include "atObject.h"
#include "atObjectRef.h"
#include "atObjectUniqueIdBuilder.h"
#include "atStringTemplate.h"
#include "Console.h"

#include <unordered_map>

#include "atRenderObject.h"

struct atLevelData
{
    atObjectUniqueIdBuilder objectBuilder;

    std::vector<atObjectsCollectionLayer> layers;
    std::unordered_map<atStringTemplate<32>, atObjectRef> objectRefs;
    atObjectRef playerRef;
    lwp::Vector2f playerDefaultPos;

    size_t currentLayerNum = 0;
    atObjectsCollectionLayer &currentLayer()
    {
        return layers[currentLayerNum];
    }

    std::vector<atRenderObjectLayer> layersRenderData;
};

#include "atSerializer.h"
#include <fstream>
#include <iostream>

struct atLevelDataLoader
{
    static void saveLevelSourceData(const char *fileName, atLevelData &levelData)
    {

        std::ofstream file(fileName + std::string(".v2"));
        atSerializer::writeObjectCollectionLayers(file, levelData.layers);
        atSerializer::writeObjectRefCollection(file, levelData.objectRefs);
        atSerializer::writeRenderObjectCollectionLayers(file, levelData.layersRenderData);
        file.close();
    }

    static void loadLevelSourceData(const char *fileName, atLevelData &levelData)
    {
        std::ifstream file(fileName + std::string(".v2"));
        atSerializer::readObjectCollectionLayers(file, levelData.layers);
        atSerializer::readObjectRefCollection(file, levelData.objectRefs);
        levelData.layersRenderData.resize(levelData.layers.size());
        atSerializer::readRenderObjectCollectionLayers(file, levelData.layersRenderData);
        file.close();

        onLoad_setUniqueIdsToObjects(levelData);
        onLoad_setObjectIdsToRefs(levelData);
        onLoad_resolvePlayerRef(levelData);
        onLoad_resolveRenderObjectRefs(levelData);
    }

private:
    static void onLoad_setUniqueIdsToObjects(atLevelData &levelData)
    {
        for (atObjectsCollectionLayer &layer : levelData.layers)
        {
            for (atObjectsCollection *collection : layer.collections())
            {
                for (atObject &obj : collection->vec)
                {
                    levelData.objectBuilder.setUniqueId(obj);
                }
            }
        }
    }

    static void onLoad_setObjectIdsToRefs(atLevelData &levelData)
    {
        for (auto &objRefPair : levelData.objectRefs)
        {
            atObjectRef &ref = objRefPair.second;
            ref = atObjectRef(&levelData.layers, ref.layerIndex, ref.collectionIndex, ref.objectIndex);
        }
    }

    static void onLoad_resolvePlayerRef(atLevelData &levelData)
    {
        auto playerRefIt = levelData.objectRefs.find("player");
        if (playerRefIt != levelData.objectRefs.end())
        {
            levelData.playerRef = (*playerRefIt).second;
        }
        else
        {
            atObject player = levelData.objectBuilder.player(levelData.playerDefaultPos.x, levelData.playerDefaultPos.y);
            levelData.currentLayer().colRigid.vec.push_back(player);
            levelData.playerRef = atObjectRef(&levelData.layers, player.id);
            levelData.objectRefs["player"] = levelData.playerRef;
        }
    }

    static void onLoad_resolveRenderObjectRefs(atLevelData &levelData)
    {
        for(atRenderObjectLayer& layer : levelData.layersRenderData)
        {
            for(atRenderObjectCollection* collection: layer.collections())
            {
                for(atRenderObject& renderObj : collection->vec)
                {
                    atObjectRef& ref = renderObj.objRef;
                    ref = atObjectRef(&levelData.layers, ref.layerIndex, ref.collectionIndex, ref.objectIndex);
                }
            }
        }
    }
};

static inline void atObjectLayers_moveRenderObject(atRenderObjectRef &ref, size_t newCollectionIndex, size_t newLayerIndex)
{
    Console::log("[atObjectLayers_moveRenderObject] from layer {} collection {}  to layer {} collection {}\n",
                 ref.layerIndex, ref.collectionIndex, newLayerIndex, newCollectionIndex);

    atRenderObjectLayer &newLayer = ref.renderLayers->at(newLayerIndex);
    atRenderObjectCollection *newCollection = newLayer.collections()[newCollectionIndex];
    atRenderObjectCollection *currectCollection = ref.resolveCollection();

    atRenderObject obj = *ref.resolve();
    currectCollection->vec.erase(currectCollection->vec.begin() + ref.objectIndex);
    newCollection->vec.push_back(obj);
}

static inline void atObjectLayers_moveRenderObjectToLayer(atRenderObjectRef &ref, size_t newLayerIndex)
{
    atObjectLayers_moveRenderObject(ref, ref.collectionIndex, newLayerIndex);
}

static inline void atObjectLayers_moveRenderObjectToCollection(atRenderObjectRef &ref, size_t newCollectionIndex)
{
    atObjectLayers_moveRenderObject(ref, newCollectionIndex, ref.layerIndex);
}

static inline void atObjectLayers_moveObject(atObjectRef &ref, size_t newCollectionIndex, size_t newLayerIndex)
{
    Console::log("[atObjectLayers_moveObject] from layer {} collection {}  to layer {} collection {}\n",
                 ref.layerIndex, ref.collectionIndex, newLayerIndex, newCollectionIndex);
    atObjectsCollectionLayer &newLayer = ref.layers->at(newLayerIndex);
    atObjectsCollection *newCollection = newLayer.collections()[newCollectionIndex];
    atObjectsCollection *currectCollection = ref.resolveCollection();

    atObject obj = *ref.resolve();
    currectCollection->vec.erase(currectCollection->vec.begin() + ref.objectIndex);
    newCollection->vec.push_back(obj);

    // find ovject in render and move to other layer TODO
}

static inline void atObjectLayers_moveObjectToLayer(atObjectRef &ref, size_t newLayerIndex)
{
    atObjectLayers_moveObject(ref, ref.collectionIndex, newLayerIndex);
}

static inline void atObjectLayers_moveObjectToCollection(atObjectRef &ref, size_t newCollectionIndex)
{
    atObjectLayers_moveObject(ref, newCollectionIndex, ref.layerIndex);
}