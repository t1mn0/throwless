#ifndef FPP_EITHER_HPP
#error "Include Either.hpp instead of Either<L>.hpp"
#endif

#include "../../include/Either/Either.hpp"
#include "../../include/Option/Option.hpp"

namespace fpp {

template <typename L>
requires (CopyableOrVoid<L> && MoveableOrVoid<L>)
class Either<L, void> {
private:
    Option<L> maybe_left_val;

    void destroy_value() noexcept {
        maybe_left_val.destroy_value();
    }

public:
    explicit Either() noexcept : maybe_left_val(Option<L>{}) {}

    explicit Either(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>) : maybe_left_val(l) {}
    
    explicit Either(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>) : maybe_left_val(std::move(l)) {}
    
    Either(const Either& oth) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        maybe_left_val = oth.maybe_left_val;
    }
    
    Either& operator=(const Either& oth) 
        noexcept(std::is_nothrow_copy_constructible_v<L>) {
        if (this != &oth) {
            destroy_value();
            maybe_left_val = oth.maybe_left_val;
        }
        return *this;
    }
    
    Either(Either&& oth) noexcept(std::is_nothrow_move_constructible_v<L>) {
        if (is_left()) {
            maybe_left_val = std::move(oth.maybe_left_val);
        }
    }
    
    Either& operator=(Either&& oth) noexcept(std::is_nothrow_move_constructible_v<L>) {
        if (this != &oth) {
            destroy_value();
            if (is_left()) {
                maybe_left_val = std::move(oth.maybe_left_val);
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

    static Either from_option(const Option<L>& opt) noexcept {
        if (opt.has_value()) {
            return Either<L, void>::from_left(opt.value_or_exception());
        }
        return Either<L, void>::from_right();
    }

    static Either from_option(Option<L>&& opt) noexcept {
        if (opt.has_value()) {
            return Either<L, void>::from_left(std::move(opt).value_or_exception());
        }
        return Either<L, void>::from_right();
    }

    bool is_left() const noexcept { return maybe_left_val.has_value(); }
    bool is_right() const noexcept { return !maybe_left_val.has_value(); }

    Option<L> left_value() noexcept {
        if (is_left()) {
            return maybe_left_val;
        }
        return Option<L>();
    }

    L left_value_or(L& val) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        return is_left() ? *reinterpret_cast<L*>(maybe_left_val._value) : val;
    }
    
    L left_value_or(L&& val) noexcept(std::is_nothrow_copy_constructible_v<L>) {
        return is_left() ? *reinterpret_cast<L*>(maybe_left_val._value) : std::move(val);
    }
    
    L left_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<L>) 
        requires std::default_initializable<L> 
    {
        return is_left() ? *reinterpret_cast<L*>(maybe_left_val._value) : L();
    }

    const L& left_value_or_exception() const {
        if (is_left()) {
            return maybe_left_val.value_or_exception();
        }
        throw std::runtime_error("Either does not contain left value");
    }

    //? It may be worth reviewing and considering the void-value
    void right_value_or_exception() const {
        if (!is_right()) {
            throw std::runtime_error("Either does not contain right value {void-err}");
        }
    }

    Either<void, L> transpose_types() const & 
        noexcept(std::is_nothrow_move_constructible_v<L>) 
    {
        if (is_left()) {
            return Either<void, L>::from_right(*reinterpret_cast<const L*>(maybe_left_val._value));
        }
        return Either<void, L>::from_left();
    }

    void swap(Either<L, void>& oth) 
        noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_assignable_v<L>) 
    {
        std::swap(maybe_left_val, oth.maybe_left_val);
    }

    template <typename Func>
    requires (std::invocable<Func, L>)
    auto fmap_left(Func&& fn) const & -> Either<std::invoke_result_t<Func, L>, void> {
        if (is_left()) {
            return Either<std::invoke_result_t<Func, L>, void>::from_left(
                std::forward<Func>(fn)(*reinterpret_cast<const L*>(maybe_left_val._value)));
        }
        return Either<std::invoke_result_t<Func, L>, void>::from_right();
    }

    //casts :
    Option<L> to_option() const & noexcept {
        if (is_left()) {
            return maybe_left_val;
        }
        return Option<L>();
    }

    Option<L> to_option() && noexcept {
        if (is_left()) {
            return std::move(maybe_left_val);
        }
        return Option<L>();
    }

    explicit operator Option<L>() const & noexcept { return to_option(); }
    explicit operator Option<L>() && noexcept { return std::move(*this).to_option(); }
    
}; // class 'Either<L, void>'

} // namespace 'fpp'
