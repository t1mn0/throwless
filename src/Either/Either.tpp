#ifndef FPP_EITHER_HPP
#error "Include Either.hpp instead of Either.tpp"
#endif

#include "../../include/Either/Either.hpp"

namespace fpp {

//*   <--- constructors, (~)ro5, destructor --->

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::Either(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>)
    : type_tag(Tag::LeftTypeTag), left_val(l) {}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::Either(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>)
    : type_tag(Tag::LeftTypeTag), left_val(std::move(l)) {}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::Either(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>)
    : type_tag(Tag::RightTypeTag), right_val(r) {}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::Either(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>)
    : type_tag(Tag::RightTypeTag), right_val(std::move(r)) {}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::Either(const Either& oth) 
    noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>)
    : type_tag(oth.type_tag) 
{
    if (oth.is_left()) {
        left_val = oth.left_val;
    } 
    else {
        right_val = oth.right_val;
    }
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>& Either<L, R>::operator=(const Either& oth)
    noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>) 
{
    if (this != &oth) {
        destroy_value();
        type_tag = oth.type_tag;
        if (oth.is_left()) {
            left_val = oth.left_val;
        } 
        else {
            right_val = oth.right_val;
        }
    }
    return *this;
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::Either(Either&& oth)
    noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R>)
{
    type_tag = oth.type_tag;
    if (oth.is_left()) {
        new (&left_val) L(std::move(oth.left_val));
        oth.left_val.~L();
    } 
    else if (oth.is_right()){
        new (&right_val) R(std::move(oth.right_val));
        oth.right_val.~R();
    }
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>& Either<L, R>::operator=(Either&& oth)
    noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R>) {
    if (this != &oth) {
        destroy_value();
        type_tag = oth.type_tag;
        if (oth.is_left()) {
            new (&left_val) L(std::move(oth.left_val));
            oth.left_val.~L();
        } 
        else {
            new (&right_val) R(std::move(oth.right_val));
            oth.right_val.~R();
        }
    }

    return *this;
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R>::~Either() noexcept {
    destroy_value();
}

//*   <--- static mnemonic methods that create the Either object from an passed argument --->

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R> Either<L, R>::from_left(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>) {
    return Either(l);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R> Either<L, R>::from_left(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>) {
    return Either(std::move(l));
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R> Either<L, R>::from_right(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>) {
    return Either(r);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<L, R> Either<L, R>::from_right(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>) {
    return Either(std::move(r));
}

//*   <--- specialized algorithms & methods  --->

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
bool Either<L, R>::is_left() const noexcept {
    return type_tag == Tag::LeftTypeTag;
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
bool Either<L, R>::is_right() const noexcept {
    return type_tag == Tag::RightTypeTag;
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Option<L> Either<L, R>::left_value() noexcept {
    if (is_left()) {
        return Option<L>(left_val);
    }
    return Option<L>();
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Option<R> Either<L, R>::right_value() noexcept {
    if (is_right()) {
        return Option<R>(right_val);
    }
    return Option<R>();
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
L Either<L, R>::left_value_or(L& val) noexcept(std::is_nothrow_copy_constructible_v<L>) {
    return is_left() ? left_val : val;
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
R Either<L, R>::right_value_or(R& val) noexcept(std::is_nothrow_copy_constructible_v<R>) {
    return is_right() ? right_val : val;
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
L Either<L, R>::left_value_or(L&& val) noexcept(std::is_nothrow_copy_constructible_v<L>) {
    return is_left() ? left_val : std::move(val);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
R Either<L, R>::right_value_or(R&& val) noexcept(std::is_nothrow_copy_constructible_v<R>) {
    return is_right() ? right_val : std::move(val);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
L Either<L, R>::left_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<L>) 
    requires std::default_initializable<L> 
{
    return is_left() ? left_val : L{};
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
R Either<L, R>::right_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<R>) 
    requires std::default_initializable<R>
{
    return is_right() ? right_val : R{};
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
const L& Either<L, R>::left_value_or_exception() const {
    if (is_left()) {
        return left_val;
    }
    throw std::runtime_error("Either does not contain left value");
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
const R& Either<L, R>::right_value_or_exception() const {
    if (is_right()) {
        return right_val;
    }
    throw std::runtime_error("Either does not contain right value");
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
void Either<L, R>::swap(Either<L,R>& oth) 
    noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R> && 
             std::is_nothrow_move_assignable_v<L> && std::is_nothrow_move_assignable_v<R>)
{
    if (this == &oth) return;
    
    Either temp(std::move(*this));
    *this = std::move(oth);
    oth = std::move(temp);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
Either<R, L> Either<L, R>::transpose_types() const & 
    noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R>) {
    if (is_left()) {
        return Either<R, L>::from_right(left_val);
    }
    return Either<R, L>::from_left(right_val);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
void Either<L, R>::destroy_value() noexcept {
    if (is_left()) {
        left_val.~L();
    } 
    else {
        right_val.~R();
    }
}

//*   <--- functional methods --->
template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
template <typename Func> requires std::invocable<Func, L>
auto Either<L, R>::fmap_left(Func&& fn) const -> Either<std::invoke_result_t<Func,L>, R> {
    if (is_left()) {
        return Either<std::invoke_result_t<Func,L>, R>::from_left(
            std::forward<Func>(fn)(left_val));
    }
    return Either<std::invoke_result_t<Func,L>, R>::from_right(right_val);
}

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
template <typename Func> requires std::invocable<Func, R>
auto Either<L, R>::fmap_right(Func&& fn) const -> Either<L, std::invoke_result_t<Func,R>> {
    if (is_right()) {
        return Either<L, std::invoke_result_t<Func,R>>::from_right(
            std::forward<Func>(fn)(right_val));
    }
    return Either<L, std::invoke_result_t<Func,R>>::from_left(left_val);
}

// TODO :
#if 0
template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
template <typename Func>
Either<L, R> Either<L, R>::fmap(Func&& fn) const {
    if (is_right()) {
        return Either<L, R>::from_right(fn(left_val));
    } 
    else {
        return Either<L, R>::from_left(fn(right_val));
    }
}
#endif

} // namespace 'fpp'