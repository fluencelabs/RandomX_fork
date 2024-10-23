#pragma once

#include <cstddef>   // для std::size_t

template<typename T>
class Vector {
public:
    // Типы, аналогичные std::vector
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    // Конструкторы
    Vector();
    explicit Vector(size_type count);
    ~Vector();

    // Запрет копирования
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;

    // Разрешение перемещения
    Vector(Vector&& other);
    Vector& operator=(Vector&& other);

    // Доступ к элементам
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference back() const;

    size_type size() const;

    void push_back(const T& value);

    void clear();

private:
    T m_data[256];
    size_type m_size;
    size_type m_capacity;

    void reallocate(size_type new_cap);
};