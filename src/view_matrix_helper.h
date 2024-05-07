#pragma once

#include <type_traits>

#include "helper.h"

namespace view_matrix_helper {

template <bool IsConst, class T, class Y>
using IfElse = std::conditional_t<IsConst, T, Y>;

template <class T, bool IsConst>
using ReturnIfConst = IfElse<IsConst, helper::ReturnAs<T>, T&>;

template <class Container, bool IsConst>
using ContainerType = IfElse<IsConst, const Container, Container>;

}  // namespace view_matrix_helper
