#pragma once
#include "atLevelDesigner_Data.h"
#include "atLevelDesignerOp_Collection.h"
#include "../Console.h"
#include <memory>

struct atLevelDesignerOp_Executer
{
    std::vector<std::unique_ptr<atLevelDesignerOp_Interface>> undoStack;
    std::vector<std::unique_ptr<atLevelDesignerOp_Interface>> redoStack;
    static constexpr size_t MAX_HISTORY = 10;

    void execute(std::unique_ptr<atLevelDesignerOp_Interface> op, atLevelDesigner_Data &data)
    {
        op->apply(data);
        undoStack.push_back(std::move(op));
        if (undoStack.size() > MAX_HISTORY)
        {
            undoStack.erase(undoStack.begin());
        }
        redoStack.clear();
        Console::log("executed: {}\n", undoStack.back()->getTypeName());
    }

    void undo(atLevelDesigner_Data &data)
    {
        if (undoStack.empty())
            return;
        auto op = std::move(undoStack.back());
        undoStack.pop_back();
        op->undo(data);
        redoStack.push_back(std::move(op));
        Console::log("undo: {}\n", redoStack.back()->getTypeName());
    }

    void redo(atLevelDesigner_Data &data)
    {
        if (redoStack.empty())
            return;
        auto op = std::move(redoStack.back());
        redoStack.pop_back();
        op->apply(data);
        undoStack.push_back(std::move(op));
        Console::log("redo: {}\n", undoStack.back()->getTypeName());
    }

    void clear()
    {
        undoStack.clear();
        redoStack.clear();
    }

    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }
};
