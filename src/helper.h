#pragma once

#include <type_traits>

namespace helper {

template <class T>
bool constexpr kIsStandardLayout = std::is_standard_layout_v<T>;

template <class T>
bool constexpr kIsSmall = sizeof(T) <= 16;

template <class T, bool ByValue = kIsStandardLayout<T>&& kIsSmall<T>>
struct ReturnType;

template <class T>
struct ReturnType<T, true> {
    using Type = T;
};

template <class T>
struct ReturnType<T, false> {
    using Type = const T&;
};

template <class T>
using ReturnAs = typename ReturnType<T>::Type;

}  // namespace helper
