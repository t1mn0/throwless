#ifndef FPP_EITHER_HPP
#define FPP_EITHER_HPP

#include <utility> // for std::move;
#include <type_traits> // for std::is_nothrow_constructible, std::is_same_v, std::invokable, etc;

#include "../Option/Option.hpp"
#include "../UsedConcepts/Concepts.hpp"

namespace fpp {

template <typename L, typename R>
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
class Either {
private: //* substructures :
    enum class Tag { 
        LeftTypeTag, 
        RightTypeTag
    };
    
private: //* fields :
    Tag type_tag;
    
    union {
        L left_val;
        R right_val;
    };

private: //*methods :
    void destroy_value() noexcept;

public: //* methods :
    //*   <--- constructors, (~)ro5, destructor --->
    Either() = delete;

    explicit Either(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>);
    explicit Either(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>);
    
    explicit Either(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>);
    explicit Either(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>);

    Either(const Either& oth)
        noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>);
    Either& operator=(const Either& oth) 
        noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>);

    Either(Either&& oth) 
        noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R>);
    Either& operator=(Either&& oth) 
        noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R>);

    ~Either() noexcept;
    
    //*   <--- static mnemonic methods that create the Either object from an passed argument --->
    static Either from_left(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>);
    static Either from_left(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>);
    static Either from_right(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>);
    static Either from_right(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>);

    //*   <--- specialized algorithms & methods  --->
    bool is_left() const noexcept;
    bool is_right() const noexcept;

    Option<L> left_value() noexcept;
    Option<R> right_value() noexcept;

    L left_value_or(L& val) noexcept(std::is_nothrow_copy_constructible_v<L>);  
    R right_value_or(R& val) noexcept(std::is_nothrow_copy_constructible_v<R>);
    L left_value_or(L&& val) noexcept(std::is_nothrow_copy_constructible_v<L>);  
    R right_value_or(R&& val) noexcept(std::is_nothrow_copy_constructible_v<R>);
    L left_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<L>) requires std::default_initializable<L>;
    R right_value_or_default() noexcept(std::is_nothrow_copy_constructible_v<R>) requires std::default_initializable<R>;

    const L& left_value_or_exception() const;
    const R& right_value_or_exception() const;

    Either<R, L> transpose_types() const & 
        noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R>);

    void swap(Either<L,R>& oth) 
        noexcept(std::is_nothrow_move_constructible_v<L> && std::is_nothrow_move_constructible_v<R> && 
            std::is_nothrow_move_assignable_v<L> && std::is_nothrow_move_assignable_v<R>);

    //*   <--- functional methods --->
    template <typename Func>
    requires std::invocable<Func, L>
    auto fmap_left(Func&& fn) const -> Either<std::invoke_result_t<Func,L>, R>;

    template <typename Func>
    requires std::invocable<Func, R>
    auto fmap_right(Func&& fn) const -> Either<L, std::invoke_result_t<Func,R>>;

    // TODO : implement it with type-conversion support without cutting the fmap method by 2 :
    // fmap -> FuncForLeft ? Either<invoke<Func,L>, R> : Either<L, invoke<Func,R>
    #if 0
    template <typename Func>
    Either<L, R> fmap(Func&& f) const;
    #endif

}; // class 'Either'

} // namespace 'fpp'

#include "../../src/Either/Either.tpp"
#include "../../src/Either/Either<L>.hpp"
#include "../../src/Either/Either<R>.hpp"

// specialization of std::swap delegating to the Either<T> method
namespace std {

template <typename L, typename R>
requires (!std::is_same_v<L, R> && fpp::CopyableOrVoid<L> && fpp::CopyableOrVoid<R> && fpp::MoveableOrVoid<L> && fpp::MoveableOrVoid<R>)
void swap(fpp::Either<L, R>& a, fpp::Either<L, R>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

} // namespace 'std'

#endif // FPP_EITHER_HPP