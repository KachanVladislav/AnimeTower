#pragma once
#include <vector>
#include "atObject.h"
#include "atObjectRef.h"
#include "atObjectUniqueIdBuilder.h"

struct atLevelData
{
    atObjectUniqueIdBuilder objectBuilder;

    std::vector<atObject> objectsStatic;
    std::vector<atObject> objectsRigid;
    std::vector<atObject> objectsNotCollidable;

    atObjectRefToVector playerRef;
    lwp::Vector2f playerDefaultPos;
};

#include <fstream>
#include <iostream>
struct atLevelDataLoader
{
    static void saveLevelSourceData(const char *fileName, const atLevelData &levelData)
    {
        std::ofstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file for writing: " << fileName << std::endl;
            return;
        }

        file << "playerPos " << levelData.playerDefaultPos.x << " " << levelData.playerDefaultPos.y << "\n";

        saveRectangles(file, "objectsStatic", levelData.objectsStatic);
        saveRectangles(file, "objectsNotCollidable", levelData.objectsNotCollidable);

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
        atObject player = result.objectBuilder.player(result.playerDefaultPos.x, result.playerDefaultPos.y);
        result.playerRef.set(player.id, &result.objectsRigid);
        result.objectsRigid.push_back(player);
        loadRectangles(file, result.objectBuilder, result.objectsStatic);
        loadRectangles(file, result.objectBuilder, result.objectsNotCollidable);

        file.close();
    }

private:
    static void saveRectangles(std::ostream &out, const char *name, const std::vector<atObject> &objects)
    {
        out << name << " " << objects.size() << "\n";
        for (const auto &obj : objects)
        {
            const auto &rect = obj.getColliderRect();
            out << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << "\n";
        }
    }

    static void loadRectangles(std::istream &in, atObjectUniqueIdBuilder &builder, std::vector<atObject> &data)
    {
        std::string token;
        size_t count;
        in >> token >> count;
        data.clear();
        for (size_t i = 0; i < count; i++)
        {
            lwp::Rectangle rect;
            in >> rect.x >> rect.y >> rect.width >> rect.height;
            data.push_back(builder.fromColliderRect(rect));
        }
    }
};