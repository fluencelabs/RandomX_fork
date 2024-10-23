#include "string_freestanding.hpp"
#include <cstring>

// Конструктор по умолчанию
String::String()
    : m_size(0) {
}

// Конструктор копирования
String::String(const String& other)
    : m_size(other.m_size) {
    if (other.m_size) {
        memcpy(&m_data[0], &other.m_data[0], other.m_size);
    }
}

// Деструктор
String::~String() {
}

// Копирующий оператор присваивания
String& String::operator=(const String& other) {
    if (this != &other) {
        m_size = other.m_size;
        memcpy(&m_data[0], &other.m_data[0], m_size);
    }
    return *this;
}


String& String::assign(const char* str, size_type m_size) {
    if (str) {
        memcpy(&m_data[0], str, m_size);
    } else {
        m_size = 0;
    }
    return *this;
}

bool String::operator==(const String& other) const {
    if (m_size != other.m_size) {
        return false;
    }
    for (size_type i = 0; i < m_size; ++i) {
        if (m_data[i] != other.m_data[i]) {
            return false;
        }
    }
    return true;
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

String::size_type String::size() const {
    return m_size;
}

