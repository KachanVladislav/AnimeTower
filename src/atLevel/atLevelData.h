#pragma once
#include <vector>
#include "atObject.h"
#include "atObjectRef.h"
#include "atObjectUniqueIdBuilder.h"
#include "atStringTemplate.h"
#include "Console.h"

#include <unordered_map>

struct atLevelData
{
    atObjectUniqueIdBuilder objectBuilder;

    std::vector<atObjectsCollectionLayer> layers;
    size_t currentLayerNum = 0;
    atObjectsCollectionLayer &currentLayer()
    {
        return layers[currentLayerNum];
    }

    std::unordered_map<atStringTemplate<32>, atObjectRef> objectRefs;
    atObjectRef playerRef;
    lwp::Vector2f playerDefaultPos;
};

#include <fstream>
#include <iostream>
struct atLevelDataLoader
{
    static inline const char *TOKEN_LAYER = "layer";
    static inline const char *TOKEN_REF = "ref";
    static inline const char *TOKEN_OBJECT = "obj";
    static void saveLevelSourceData(const char *fileName, atLevelData &levelData)
    {
        std::ofstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file for writing: " << fileName << std::endl;
            return;
        }

        file << "playerPos " << levelData.playerDefaultPos.x << " " << levelData.playerDefaultPos.y << "\n";

        for (auto &layer : levelData.layers)
        {
            file << TOKEN_LAYER << " " << layer.tag.data << " \n";
            for (auto &objCollection : layer.collections())
            {
                writeObjectCollection(file, objCollection);
            }
        }
        for (auto &refPair : levelData.objectRefs)
        {
            atObjectRef &ref = refPair.second;
            if (ref.resolve())
            {
                file << TOKEN_REF << " "
                     << refPair.first.data << " "
                     << ref.layerIndex << " "
                     << ref.collectionIndex << " "
                     << ref.objectIndex << "\n";
            }
        }
        file.close();
    }

    static void loadLevelSourceData(const char *fileName, atLevelData &result)
    {
        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file for reading: " << fileName << std::endl;
            return;
        }

        std::string token;
        file >> token >> result.playerDefaultPos.x >> result.playerDefaultPos.y;

        auto readObjectCollection = [](std::istream &in, atObjectsCollection *collection, atObjectUniqueIdBuilder &builder) -> std::string
        {
            size_t count = 0;
            in >> count;

            std::string token;
            for (size_t i = 0; i < count; i++)
            {
                if (!(in >> token) || token != TOKEN_OBJECT)
                    return token;

                lwp::Rectangle rect;
                if (in >> rect.x >> rect.y >> rect.width >> rect.height)
                    collection->vec.push_back(builder.fromColliderRect(rect));
                else
                    Console::log("[FAIL] read obj collection");
                in >> rect.x >> rect.y >> rect.width >> rect.height;
                collection->vec.back().animationData.offset.x = rect.x;
                collection->vec.back().animationData.offset.y = rect.y;
                collection->vec.back().animationData.size.x = rect.width;
                collection->vec.back().animationData.size.y = rect.height;
            }
            if (!(in >> token))
                return "";
            return token;
        };

        while (!token.empty())
        {
            if (token == TOKEN_LAYER)
            {
                result.layers.push_back({});
                if (file >> token)
                    result.layers.back().tag = token.c_str();
                if (file >> token)
                    continue;
                else
                    break;
            }

            if (token == TOKEN_REF)
            {
                size_t layerIndex = 0;
                size_t collectionIndex = 0;
                size_t objectIndex = 0;
                std::string refName;
                file >> refName >> layerIndex >> collectionIndex >> objectIndex;
                result.objectRefs[refName.c_str()] = atObjectRef(&result.layers, layerIndex, collectionIndex, objectIndex);
            }

            if (result.layers.empty())
            {
                if (file >> token)
                    continue;
                else
                    break;
            }

            auto &layer = result.layers.back();

            bool matched = false;
            for (auto &collection : layer.collections())
            {
                if (collection->tag == token.c_str())
                {
                    token = readObjectCollection(file, collection, result.objectBuilder);
                    matched = true;
                    break;
                }
            }
            if (!matched)
            {
                if (file >> token)
                    continue;
                else
                    break;
            }
        };
        if (result.layers.empty())
            result.layers.push_back({});
        result.currentLayerNum = 0;

        auto playerRefIt = result.objectRefs.find("player");
        if (playerRefIt != result.objectRefs.end())
        {
            result.playerRef = (*playerRefIt).second;
        }
        else
        {
            atObject player = result.objectBuilder.player(result.playerDefaultPos.x, result.playerDefaultPos.y);
            result.currentLayer().colRigid.vec.push_back(player);
            result.playerRef = atObjectRef(&result.layers, player.id);
            result.objectRefs["player"] = result.playerRef;
        }

        file.close();
    }

private:
    static void writeObjectCollection(std::ostream &out, atObjectsCollection *collection)
    {
        out << collection->tag.data << " " << collection->vec.size() << "\n";
        for (const auto &obj : collection->vec)
        {
            const auto &rect = obj.getColliderRect();
            out << TOKEN_OBJECT << " " << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << " ";
            out << obj.animationData.offset.x << " " << obj.animationData.offset.y << " ";
            out << obj.animationData.size.x << " " << obj.animationData.size.y << "\n";
        }
    }
};

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
}

static inline void atObjectLayers_moveObjectToLayer(atObjectRef &ref, size_t newLayerIndex)
{
    atObjectLayers_moveObject(ref, ref.collectionIndex, newLayerIndex);
}

static inline void atObjectLayers_moveObjectToCollection(atObjectRef &ref, size_t newCollectionIndex)
{
    atObjectLayers_moveObject(ref, newCollectionIndex, ref.layerIndex);
}