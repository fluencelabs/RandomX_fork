#include "vector_freestanding.hpp"
#include "macroop_flu.hpp"

template<typename T>
Vector<T>::Vector()
    : m_size(0), m_capacity(0) {
}

template<typename T>
Vector<T>::Vector(size_type count)
    : m_size(0), m_capacity(0) {
    // reserve(count);
    // for (size_type i = 0; i < count; ++i) {
    //     emplace_back();
    // }
}

template<typename T>
Vector<T>::~Vector() {
    // Вызываем деструкторы элементов
    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~T();
    }
}

template<typename T>
typename Vector<T>::reference Vector<T>::operator[](size_type pos) {
    return m_data[pos];
}

template<typename T>
typename Vector<T>::const_reference Vector<T>::operator[](size_type pos) const {
    return m_data[pos];
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    // if (m_size >= m_capacity) {
    //     reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    // }
    // new (&m_data[m_size]) T(value);
    m_data[m_size] = value;
    ++m_size;
}

template<typename T>
void Vector<T>::clear() {
    m_size = 0;
}

template<typename T>
typename Vector<T>::size_type Vector<T>::size() const {
    return m_size;
}


template<typename T>
typename Vector<T>::reference Vector<T>::back() const {
    return m_data[m_size];
}

template Vector<unsigned long long>::Vector();
template Vector<unsigned long long>::~Vector();
template void Vector<unsigned long long>::clear();
template void Vector<unsigned long long>::push_back(unsigned long long const&);
template Vector<unsigned long long>::size_type Vector<unsigned long long>::size() const;
template Vector<unsigned long long>::reference Vector<unsigned long long>::operator[](size_type);

template Vector<int>::Vector();
template Vector<int>::~Vector();
template void Vector<int>::push_back(int const&);
template Vector<int>::size_type Vector<int>::size() const;
template Vector<int>::reference Vector<int>::operator[](size_type);

// template Vector<randomx::MacroOp>::Vector();
