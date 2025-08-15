#pragma once
#include <cstddef>

using ComponentTypeID = std::size_t;

inline ComponentTypeID GetNewComponentTypeID() {
    static ComponentTypeID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentTypeID GetComponentTypeID() noexcept {
    static ComponentTypeID typeID = GetNewComponentTypeID();
    return typeID;
}
