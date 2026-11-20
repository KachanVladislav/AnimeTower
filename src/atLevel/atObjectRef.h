#pragma once
#include "atObject.h"
#include <span>
#include <vector>

struct atObjectRefToVector
{
    uint32_t id = 0;
    std::vector<atObject> *vec_ptr = nullptr;
    size_t cachedIndex = 0;
    void set(uint32_t id_, std::vector<atObject> *vec_ptr_) 
    {
        this->id = id_;
        this->vec_ptr = vec_ptr_;
    }
    static atObjectRefToVector build(uint32_t id_, std::vector<atObject> *vec_ptr_)
    {
        atObjectRefToVector res;
        res.set(id_, vec_ptr_);
        return res;
    }
    atObject *resolve()
    {
        if (!vec_ptr)
            return nullptr;
        std::vector<atObject> &vec = *vec_ptr;

        if (cachedIndex < vec.size() && vec[cachedIndex].id == id)
            return &vec[cachedIndex];

        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (vec[i].id == id)
            {
                cachedIndex = i;
                return &vec[i];
            }
        }

        return nullptr;
    }
};