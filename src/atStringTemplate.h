#pragma once
#include <cstring>
#include <cstdint>

template <size_t CAPACITY>
struct atStringTemplate
{
    char data[CAPACITY]{};
    size_t capacity() const { return CAPACITY; }

    constexpr atStringTemplate()
    {
        clear();
    }

    constexpr atStringTemplate(const char *str)
    {
        *this = str;
    }

    atStringTemplate &operator=(const char *str)
    {
        if (str == nullptr)
        {
            clear();
            return *this;
        }
        size_t len = strnlen(str, CAPACITY - 1);
        std::memcpy(data, str, len);
        std::memset(&data[len], 0, CAPACITY - len);
        return *this;
    }

    void append(const char *str)
    {
        size_t len = length();
        if (len >= CAPACITY - 1)
            return; 
        snprintf(&data[len], CAPACITY - len, "%s", str);
    }

    bool operator==(atStringTemplate str2) const
    {
        return strncmp(data, str2.data, CAPACITY) == 0;
    }

    bool operator==(const char *str) const
    {
        if (str == nullptr)
        {
            return data[0] == '\0';
        }
        return strncmp(data, str, CAPACITY) == 0;
    }

    void fromFloat(float value)
    {
        int len = snprintf(data, CAPACITY, "%.2f", value);
        std::memset(&data[len], 0, CAPACITY - len);
    }
    void clear()
    {
        std::memset(&data[0], 0, CAPACITY);
    }
    size_t length()
    {
        return strnlen_s(data, CAPACITY);
    }
};

#include <string_view>
namespace std
{
    template <size_t N>
    struct hash<atStringTemplate<N>>
    {
        size_t operator()(const atStringTemplate<N> &str) const noexcept
        {
            // Самый быстрый способ в C++17 и новее — обернуть в std::string_view
            // Предполагается, что у вашего класса есть методы c_str() и size() (или length())
            return std::hash<std::string_view>{}(std::string_view(str.data, str.capacity()));
        }
    };
}