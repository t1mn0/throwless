#ifndef FPP_EITHER_H
#error "Include Either.hpp instead of Specializations.hpp"
#endif

#include "../../include/Either/Either.hpp"

namespace fpp {

//* <--- FIRST --->
template <typename L>
requires (CopyableOrVoid<L> && MoveableOrVoid<L>)
class Either<L, void> {
private:
    enum class Tag { 
        LeftTypeTag, 
        RightTypeTag 
    };

    Tag type_tag;
    union {
        L left_val;
    };

private:
    void destroy_value() noexcept {
        if (is_left()) {
            left_val.~L();
        }
    }

public:
    explicit Either(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>) : type_tag(Tag::LeftTypeTag), left_val(l) {}
    
    explicit Either(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>) : type_tag(Tag::LeftTypeTag), left_val(std::move(l)) {}
    
    explicit Either() noexcept : type_tag(Tag::RightTypeTag) {}

    Either(const Either& other) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        type_tag = other.type_tag;
        if (is_left()) {
            new (&left_val) L(other.left_val);
        }
    }
    
    Either& operator=(const Either& other) 
        noexcept(std::is_nothrow_copy_constructible_v<L>) {
        if (this != &other) {
            destroy_value();
            type_tag = other.type_tag;
            if (is_left()) {
                new (&left_val) L(other.left_val);
            }
        }
        return *this;
    }
    
    Either(Either&& other) noexcept(std::is_nothrow_move_constructible_v<L>) {
        type_tag = other.type_tag;
        if (is_left()) {
            new (&left_val) L(std::move(other.left_val));
        }
    }
    
    Either& operator=(Either&& other) noexcept(std::is_nothrow_move_constructible_v<L>) {
        if (this != &other) {
            destroy_value();
            type_tag = other.type_tag;
            if (is_left()) {
                new (&left_val) L(std::move(other.left_val));
            }
        }
        return *this;
    }
    
    ~Either() noexcept {
        destroy_value();
    }
    static Either from_left(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        return Either(l);
    }
    
    static Either from_left(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>) {
        return Either(std::move(l));
    }
    
    static Either from_right() noexcept {
        return Either();
    }

    bool is_left() const noexcept { return type_tag == Tag::LeftTypeTag; }
    bool is_right() const noexcept { return type_tag == Tag::RightTypeTag; }

    Option<L> left_value() noexcept {
        if (is_left()) {
            return Option<L>(left_val);
        }
        return Option<L>();
    }

    L left_value_or(L& val) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        return is_left() ? left_val : val;
    }
    
    L left_value_or(L&& val) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        return is_left() ? left_val : std::move(val);
    }
    
    L left_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<L>) 
        requires std::default_initializable<L> {
        return is_left() ? left_val : L();
    }

    const L& left_value_or_exception() const {
        if (is_left()) {
            return left_val;
        }
        throw std::runtime_error("Either does not contain left value");
    }

    void right_value_or_exception() const {
        if (!is_right()) {
            throw std::runtime_error("Either does not contain right value {void-err}");
        }
    }

    Either<void, L> transpose_types() const & 
        noexcept(std::is_nothrow_move_constructible_v<L>) {
        if (is_left()) {
            return Either<void, L>::from_right(left_val);
        }
        return Either<void, L>::from_left();
    }

    void swap(Either<L, void>& other) 
        noexcept(std::is_nothrow_move_constructible_v<L> && 
                std::is_nothrow_move_assignable_v<L>) {
        if (is_left() && other.is_left()) {
            std::swap(left_val, other.left_val);
        }
        else if (is_left() && other.is_right()) {
            L temp = std::move(left_val);
            destroy_value();
            type_tag = Tag::RightTypeTag;
            new (&other.left_val) L(std::move(temp));
            other.type_tag = Tag::LeftTypeTag;
        }
        else if (is_right() && other.is_left()) {
            L temp = std::move(other.left_val);
            other.destroy_value();
            other.type_tag = Tag::RightTypeTag;
            new (&left_val) L(std::move(temp));
            type_tag = Tag::LeftTypeTag;
        }
    }

    template <typename Func>
    requires (std::invocable<Func, L>)
    auto fmap_left(Func&& fn) const & -> Either<std::invoke_result_t<Func, L>, void> {
        if (is_left()) {
            return Either<std::invoke_result_t<Func, L>, void>::from_left(
                std::forward<Func>(fn)(left_val));
        }
        return Either<std::invoke_result_t<Func, L>, void>::from_right();
    }
}; // class 'Either<L, void>'

//* <--- SECOND --->

template <typename R>
requires (CopyableOrVoid<R> && MoveableOrVoid<R>)
class Either<void, R> {
private:
    enum class Tag { 
        LeftTypeTag, 
        RightTypeTag 
    };
    
    Tag type_tag;
    union {
        R right_val;
    };

    void destroy_value() noexcept {
        if (is_right()) {
            right_val.~R();
        }
    }

public:
    explicit Either() noexcept : type_tag(Tag::LeftTypeTag) {}
    
    explicit Either(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>) 
        : type_tag(Tag::RightTypeTag), right_val(r) {}
    
    explicit Either(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>) 
        : type_tag(Tag::RightTypeTag), right_val(std::move(r)) {}

    Either(const Either& other) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        type_tag = other.type_tag;
        if (is_right()) {
            new (&right_val) R(other.right_val);
        }
    }
    
    Either& operator=(const Either& other) 
        noexcept(std::is_nothrow_copy_constructible_v<R>) {
        if (this != &other) {
            destroy_value();
            type_tag = other.type_tag;
            if (is_right()) {
                new (&right_val) R(other.right_val);
            }
        }
        return *this;
    }
    
    Either(Either&& other) noexcept(std::is_nothrow_move_constructible_v<R>) {
        type_tag = other.type_tag;
        if (is_right()) {
            new (&right_val) R(std::move(other.right_val));
        }
    }
    
    Either& operator=(Either&& other) noexcept(std::is_nothrow_move_constructible_v<R>) {
        if (this != &other) {
            destroy_value();
            type_tag = other.type_tag;
            if (is_right()) {
                new (&right_val) R(std::move(other.right_val));
            }
        }
        return *this;
    }
    
    ~Either() noexcept {
        destroy_value();
    }
    
    static Either from_left() noexcept {
        return Either();
    }
    
    static Either from_right(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        return Either(r);
    }
    
    static Either from_right(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>) {
        return Either(std::move(r));
    }

    bool is_left() const noexcept { return type_tag == Tag::LeftTypeTag; }
    bool is_right() const noexcept { return type_tag == Tag::RightTypeTag; }
    
    Option<R> right_value() noexcept {
        if (is_right()) {
            return Option<R>(right_val);
        }
        return Option<R>();
    }

    R right_value_or(R& val) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        return is_right() ? right_val : val;
    }
    
    R right_value_or(R&& val) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        return is_right() ? right_val : std::move(val);
    }
    
    R right_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<R>) 
        requires std::default_initializable<R> {
        return is_right() ? right_val : R();
    }

    void left_value_or_exception() const {
        if (!is_left()) {
            throw std::runtime_error("Either does not contain left value");
        }
    }

    const R& right_value_or_exception() const {
        if (is_right()) {
            return right_val;
        }
        throw std::runtime_error("Either does not contain right value");
    }

    Either<R, void> transpose_types() const & 
        noexcept(std::is_nothrow_move_constructible_v<R>) {
        if (is_right()) {
            return Either<R, void>::from_left(right_val);
        }
        return Either<R, void>::from_right();
    }

    void swap(Either<void, R>& other) 
        noexcept(std::is_nothrow_move_constructible_v<R> && 
                std::is_nothrow_move_assignable_v<R>) {
        if (is_right() && other.is_right()) {
            std::swap(right_val, other.right_val);
        }
        else if (is_right() && other.is_left()) {
            R temp = std::move(right_val);
            destroy_value();
            type_tag = Tag::LeftTypeTag;
            new (&other.right_val) R(std::move(temp));
            other.type_tag = Tag::RightTypeTag;
        }
        else if (is_left() && other.is_right()) {
            R temp = std::move(other.right_val);
            other.destroy_value();
            other.type_tag = Tag::LeftTypeTag;
            new (&right_val) R(std::move(temp));
            type_tag = Tag::RightTypeTag;
        }
    }

    template <typename Func>
    requires (std::invocable<Func, R>)
    auto fmap_right(Func&& fn) const & -> Either<void, std::invoke_result_t<Func, R>> {
        if (is_right()) {
            return Either<void, std::invoke_result_t<Func, R>>::from_right(
                std::forward<Func>(fn)(right_val));
        }
        return Either<void, std::invoke_result_t<Func, R>>::from_left();
    }

}; // class 'Either<void, R>'

} // namespace 'fpp'