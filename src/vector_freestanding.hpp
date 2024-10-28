#pragma once

#include <cstddef> // для std::size_t
#include <malloc.h>

const constexpr size_t VectorDefaultCapacity = 10;

template <typename T> class Vector {
public:
  // Типы, аналогичные std::vector
  using value_type = T;
  using size_type = std::size_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  // Конструкторы
  Vector();
  explicit Vector(size_type count);
  ~Vector();

  // Запрет копирования
  Vector(const Vector &) = delete;
  Vector &operator=(const Vector &) = delete;

  // Разрешение перемещения
  Vector(Vector &&other);
  Vector &operator=(Vector &&other);

  // Доступ к элементам
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference back() const;

  size_type size() const;

  void push_back(const T &value);

  void clear();

private:
  T *m_data;
  size_type m_size;
  size_type m_capacity;

  void reallocate(size_t new_capacity) {
    // T* new_data = new T[new_capacity];
    T *new_data = reinterpret_cast<T *>(malloc(sizeof(T) * new_capacity));
    for (size_t i = 0; i < m_size; ++i) {
      new_data[i] = m_data[i];
    }
    if (m_capacity) {
      delete[] m_data;
    }
    m_data = new_data;
    m_capacity = new_capacity;
  }
};