#pragma once
#include <print>
#include "lwp/lwp.h"

class Console
{
public:
    template <typename... Args>
    static void log(std::format_string<Args...> fmt, Args &&...args)
    {
        std::print(fmt, std::forward<Args>(args)...);
    }

    static void logVector(std::string_view tag, const lwp::Vector2f &vec)
    {
        log("{} x {:.1f} y {:.1f}\n", tag, vec.x, vec.y);
    }

    static void logRectangle(std::string_view tag, const lwp::Rectangle &rect)
    {
        log("{} x {:.1f} y {:.1f} bottom {:.1f}\n", tag, rect.x, rect.y, rect.getY_Bottom());
    }
};