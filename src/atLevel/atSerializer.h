#pragma once
#include "atObject.h"
#include "atObjectRef.h"
#include "atRenderObject.h"
#include <sstream>

struct atSerializer
{
    //--- RenderObject
    static void writeRenderObjectCollectionLayers(std::ostream &stream, const std::vector<atRenderObjectLayer> &objCollectionLayers)
    {
        stream << "layers_1.0" << " " << objCollectionLayers.size() << "\n";
        for (const auto &layer : objCollectionLayers)
        {
            writeRenderObjectCollectionLayer(stream, layer);
        }
    }

    static void readRenderObjectCollectionLayers(std::istream &stream, std::vector<atRenderObjectLayer> &objCollectionLayers)
    {
        std::string tmp;
        size_t size{};
        stream >> tmp >> size;
        for(size_t i = 0; i < size; i++)
        {
            if(i < objCollectionLayers.size())
                readRenderObjectCollectionLayer(stream, objCollectionLayers[i]);
            else
            {
                atRenderObjectLayer tmpLayer;
                readRenderObjectCollectionLayer(stream, tmpLayer);
                Console::log("[FAIL] [readRenderObjectCollectionLayers] render layers size less than saved");
            }
        }
    }

    static void writeRenderObjectCollectionLayer(std::ostream &stream, const atRenderObjectLayer &objCollectionLayer)
    {
        writeRenderObjectCollection(stream, objCollectionLayer.sprites);
        writeRenderObjectCollection(stream, objCollectionLayer.platforms);
        writeRenderObjectCollection(stream, objCollectionLayer.animated);
    }

    static void readRenderObjectCollectionLayer(std::istream &stream, atRenderObjectLayer &objCollectionLayer)
    {
        readRenderObjectCollection(stream, objCollectionLayer.sprites);
        readRenderObjectCollection(stream, objCollectionLayer.platforms);
        readRenderObjectCollection(stream, objCollectionLayer.animated);
    }

    static void writeRenderObjectCollection(std::ostream &stream, const atRenderObjectCollection &objCollection)
    {
        stream << objCollection.tag.data << " " << objCollection.vec.size() << "\n";
        for (const auto &obj : objCollection.vec)
        {
            writeRenderObject(stream, obj);
        }
    }

    static void readRenderObjectCollection(std::istream &stream, atRenderObjectCollection &objCollection)
    {
        std::string tmp;
        size_t size = 0;
        stream >> tmp >> size;
        objCollection.vec.resize(size);
        for (auto &obj : objCollection.vec)
        {
            readRenderObject(stream, obj);
        }
    }

    static void writeRenderObject(std::ostream &stream, const atRenderObject &obj)
    {
        const atObjectRef &ref = obj.objRef;
        stream << ref.layerIndex << " " << ref.collectionIndex << " " << ref.objectIndex << "\n";
        writeVector2f(stream, obj.offset);
        writeVector2f(stream, obj.size);
    }

    static void readRenderObject(std::istream &stream, atRenderObject &obj)
    {
        atObjectRef &ref = obj.objRef;
        stream >> ref.layerIndex >> ref.collectionIndex >> ref.objectIndex;
        readVector2f(stream, obj.offset);
        readVector2f(stream, obj.size);
    }

    //--- ObjectRef
    static void writeObjectRefCollection(std::ostream &stream, std::unordered_map<atStringTemplate<32>, atObjectRef> &objectRefs)
    {
        std::vector<std::pair<atStringTemplate<32>, atObjectRef>> resolvedRefs;
        resolvedRefs.reserve(objectRefs.size());
        for (auto &objRef : objectRefs)
        {
            if (objRef.second.resolve())
                resolvedRefs.push_back(objRef);
        }
        stream << "refs" << " " << resolvedRefs.size() << "\n";
        for (auto &objRef : resolvedRefs)
        {
            atObjectRef &ref = objRef.second;
            stream << objRef.first.data << " " << ref.layerIndex << " " << ref.collectionIndex << " " << ref.objectIndex << "\n";
        }
    }

    static void readObjectRefCollection(std::istream &stream, std::unordered_map<atStringTemplate<32>, atObjectRef> &objectRefs)
    {
        std::string tmp;
        size_t size{};
        stream >> tmp >> size;
        objectRefs.reserve(size);
        for (size_t i = 0; i < size; i++)
        {
            std::string name;
            atObjectRef ref;
            stream >> name >> ref.layerIndex >> ref.collectionIndex >> ref.objectIndex;
            objectRefs[name.c_str()] = ref;
        }
    }

    //--- ObjectCollectionLayer and lower
    static void writeObjectCollectionLayers(std::ostream &stream, const std::vector<atObjectsCollectionLayer> &objCollectionLayers)
    {
        stream << "layers_1.0" << " " << objCollectionLayers.size() << "\n";
        for (const auto &layer : objCollectionLayers)
        {
            writeObjectCollectionLayer(stream, layer);
        }
    }

    static void readObjectCollectionLayers(std::istream &stream, std::vector<atObjectsCollectionLayer> &objCollectionLayers)
    {
        std::string tmp;
        size_t size{};
        stream >> tmp >> size;
        objCollectionLayers.resize(size);
        for (auto &layer : objCollectionLayers)
        {
            readObjectCollectionLayer(stream, layer);
        }
    }

    static void writeObjectCollectionLayer(std::ostream &stream, const atObjectsCollectionLayer &objCollectionLayer)
    {
        stream << objCollectionLayer.tag.data << "\n";
        writeObjectCollection(stream, objCollectionLayer.colStatic);
        writeObjectCollection(stream, objCollectionLayer.colRigid);
        writeObjectCollection(stream, objCollectionLayer.colNotCollidable);
    }

    static void readObjectCollectionLayer(std::istream &stream, atObjectsCollectionLayer &objCollectionLayer)
    {
        std::string tmp;
        stream >> tmp;
        objCollectionLayer.tag = tmp.c_str();
        readObjectCollection(stream, objCollectionLayer.colStatic);
        readObjectCollection(stream, objCollectionLayer.colRigid);
        readObjectCollection(stream, objCollectionLayer.colNotCollidable);
    }

    static void writeObjectCollection(std::ostream &stream, const atObjectsCollection &objCollection)
    {
        stream << objCollection.tag.data << " " << objCollection.vec.size() << "\n";
        for (const auto &obj : objCollection.vec)
        {
            writeObject(stream, obj);
        }
    }

    static void readObjectCollection(std::istream &stream, atObjectsCollection &objCollection)
    {
        std::string tmp;
        size_t size = 0;
        stream >> tmp >> size;
        objCollection.vec.resize(size);
        for (auto &obj : objCollection.vec)
        {
            readObject(stream, obj);
        }
    }

    static void writeObject(std::ostream &stream, const atObject &obj)
    {
        writeVector2f(stream, obj.transform.pos);
        writeVector2f(stream, obj.collider.offset);
        writeVector2f(stream, obj.collider.size);
    }

    static void readObject(std::istream &stream, atObject &obj)
    {
        readVector2f(stream, obj.transform.pos);
        readVector2f(stream, obj.collider.offset);
        readVector2f(stream, obj.collider.size);
    }

    static void writeVector2f(std::ostream &stream, const lwp::Vector2f &vec)
    {
        stream << vec.x << " " << vec.y << " ";
    }

    static void readVector2f(std::istream &stream, lwp::Vector2f &vec)
    {
        stream >> vec.x >> vec.y;
    }
};