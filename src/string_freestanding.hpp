#pragma once

#include <cstddef> // для std::size_t
#include <cstring> // для std::memcpy (может быть недоступен, можно заменить)

// Если функции из <cstring> недоступны, можно реализовать свои версии

class String {
public:
    // Типы
    using size_type = std::size_t;
    using value_type = char;

    // Конструкторы
    String();
    String(const char* str);
    String(const String& other);
    String(String&& other);

    // Деструктор
    ~String();

    // Операторы присваивания
    String& operator=(const String& other);
    String& operator=(String&& other);

    String& assign(const char* str, size_type str_size);

    // Операторы сравнения
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;

    // Методы
    const char* c_str() const;
    size_type size() const;

private:
    char m_data[256];
    size_type m_size;

    // Вспомогательные функции
    static size_type strlen(const char* str);
    static void strcpy(char* dest, const char* src, size_type count);
};