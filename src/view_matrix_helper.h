#pragma once

#include <type_traits>

#include "helper.h"

namespace view_matrix_helper {

template <bool IsConst, class T, class Y>
using IfConst = std::conditional_t<IsConst, T, Y>;

template <class T, bool IsConst>
using ReturnIfConst = IfConst<IsConst, helper::ReturnAs<T>, T&>;

template <class Container, bool IsConst>
using ContainerType = IfConst<IsConst, const Container, Container>;

}  // namespace view_matrix_helper
