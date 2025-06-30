#ifndef THROWLESS_USED_CONCEPTS_H
#define THROWLESS_USED_CONCEPTS_H

template <typename T>
concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

template <typename T>
concept Moveable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

#endif