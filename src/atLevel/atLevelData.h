#pragma once
#include <vector>
#include "../lwp/lwp.h"

struct atLevelData
{
    std::vector<lwp::Rectangle> horizontalPlatforms_lwp;
    std::vector<lwp::Rectangle> objectsRects_lwp;

    lwp::Vector2f playerPos;
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

        file << "playerPos " << levelData.playerPos.x << " " << levelData.playerPos.y << "\n";

        saveRectangles(file, "horizontalPlatforms", levelData.horizontalPlatforms_lwp);
        saveRectangles(file, "objectsRects", levelData.objectsRects_lwp);

        file.close();
    }

    static atLevelData loadLevelSourceData(const char *fileName)
    {
        atLevelData result{};

        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file for reading: " << fileName << std::endl;
            return result;
        }

        std::string token;

        file >> token >> result.playerPos.x >> result.playerPos.y;

        loadRectangles(file, result.horizontalPlatforms_lwp);
        loadRectangles(file, result.objectsRects_lwp);

        file.close();
        return result;
    }

private:
    static void saveRectangles(std::ostream &out, const char *name, const std::vector<lwp::Rectangle> &rects)
    {
        out << name << " " << rects.size() << "\n";
        for (const auto &rect : rects)
        {
            out << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << "\n";
        }
    }

    static void loadRectangles(std::istream &in, std::vector<lwp::Rectangle> &rects)
    {
        std::string token;
        size_t count;
        in >> token >> count;
        rects.resize(count);
        for (auto &rect : rects)
        {
            in >> rect.x >> rect.y >> rect.width >> rect.height;
        }
    }
};