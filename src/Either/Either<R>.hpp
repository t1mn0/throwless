#ifndef FPP_EITHER_HPP
#error "Include Either.hpp instead of Either<R>.hpp"
#endif

#include "../../include/Either/Either.hpp"
#include "../../include/Option/Option.hpp"

namespace fpp {

template <typename R>
requires (CopyableOrVoid<R> && MoveableOrVoid<R>)
class Either<void, R> {
private:
    Option<R> maybe_right_val;

    void destroy_value() noexcept {
        maybe_right_val.destroy_value();
    }

public:
    explicit Either() noexcept : maybe_right_val(Option<R>{}) {}
    
    explicit Either(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>) : maybe_right_val(r) {}
    
    explicit Either(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>) : maybe_right_val(std::move(r)) {}

    Either(const Either& oth) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        maybe_right_val = oth.maybe_right_val;
    }
    
    Either& operator=(const Either& oth) 
        noexcept(std::is_nothrow_copy_constructible_v<R>) {
        if (this != &oth) {
            destroy_value();
            if (is_right()) {
                maybe_right_val = oth.maybe_right_val;
            }
        }
        return *this;
    }
    
    Either(Either&& oth) noexcept(std::is_nothrow_move_constructible_v<R>) {
        maybe_right_val = oth.maybe_right_val;
    }
    
    Either& operator=(Either&& oth) noexcept(std::is_nothrow_move_constructible_v<R>) {
        if (this != &oth) {
            destroy_value();
            if (is_right()) {
                maybe_right_val = oth.maybe_right_val;
            }
        }
        return *this;
    }
    
    ~Either() noexcept {
        destroy_value();
    }
    
    static Either from_right(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        return Either(r);
    }
    
    static Either from_right(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>) {
        return Either(std::move(r));
    }

    static Either from_left() noexcept {
        return Either();
    }

    static Either from_option(const Option<R>& opt) noexcept {
        if (opt.has_value()) {
            return Either<void, R>::from_right(opt.value_or_exception());
        }
        return Either<void, R>::from_left();
    }

    static Either from_option(Option<R>&& opt) noexcept {
        if (opt.has_value()) {
            return Either<void, R>::from_right(std::move(opt).value_or_exception());
        }
        return Either<void, R>::from_left();
    }

    bool is_left() const noexcept { return !maybe_right_val.has_value(); }
    bool is_right() const noexcept { return maybe_right_val.has_value(); }

    Option<R> right_value() noexcept {
        if (is_right()) {
            return maybe_right_val;
        }
        return Option<R>();
    }

    R right_value_or(R& val) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        return is_right() ? *reinterpret_cast<R*>(maybe_right_val._value) : val;
    }
    
    R right_value_or(R&& val) noexcept(std::is_nothrow_copy_constructible_v<R>) {
        return is_right() ? *reinterpret_cast<R*>(maybe_right_val._value) : std::move(val);
    }
    
    R right_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<R>) 
        requires std::default_initializable<R>
    {
        return is_right() ? *reinterpret_cast<R*>(maybe_right_val._value) : R();
    }

    const R& right_value_or_exception() const {
        if (is_right()) {
            return maybe_right_val.value_or_exception();
        }
        throw std::runtime_error("Either does not contain right value");
    }

    //? It may be worth reviewing and considering the void-value
    void left_value_or_exception() const {
        if (!is_left()) {
            throw std::runtime_error("Either does not contain left value");
        }
    }

    Either<R, void> transpose_types() const & 
        noexcept(std::is_nothrow_move_constructible_v<R>) 
    {
        if (is_right()) {
            return Either<R, void>::from_left(*reinterpret_cast<const R*>(maybe_right_val._value));
        }
        return Either<R, void>::from_right();
    }

    void swap(Either<void, R>& oth) 
        noexcept(std::is_nothrow_move_constructible_v<R> && std::is_nothrow_move_assignable_v<R>) 
    {
        std::swap(maybe_right_val, oth.maybe_right_val);
    }

    template <typename Func>
    requires (std::invocable<Func, R>)
    auto fmap_right(Func&& fn) const & -> Either<void, std::invoke_result_t<Func, R>> {
        if (is_right()) {
            return Either<void, std::invoke_result_t<Func, R>>::from_right(
                std::forward<Func>(fn)(*reinterpret_cast<const R*>(maybe_right_val._value)));
        }
        return Either<void, std::invoke_result_t<Func, R>>::from_left();
    }

    // casts :
    Option<R> to_option() const & noexcept {
        if (is_right()) {
            return maybe_right_val;
        }
        return Option<R>();
    }

    Option<R> to_option() && noexcept {
        if (is_left()) {
            return std::move(maybe_right_val);
        }
        return Option<R>();
    }

    explicit operator Option<R>() const & noexcept { return to_option(); }
    explicit operator Option<R>() && noexcept { return std::move(*this).to_option(); }
    
}; // class 'Either<void, R>'

} // namespace 'fpp'