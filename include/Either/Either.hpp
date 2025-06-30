#ifndef THROWLESS_EITHER_H
#define THROWLESS_EITHER_H

#include <utility> // for std::move
#include <type_traits> // for is_nothrow_constructible

#include "../Optional/Optional.hpp"
#include "../UsedConcepts/Concepts.hpp"

namespace throwless {

template <typename L, typename R>
requires (Copyable<L> && Copyable<R> && Moveable<L> && Moveable<R>)
class Either {
private: //* substructures :
    enum class Tag { 
        LeftTypeTag, 
        RightTypeTag
    };
    
private: //* fields :
    Tag type_tag;
    
    union {
        L left_value;
        R right_value;
    };

private: //* methods:
    // destructurizes the value that is initialized
    void destroy();

public: //* methods :
    //*   <--- constructors, (~)ro5, destructor --->
    Either(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>);// : type_tag(Tag::LeftTypeTag), left_value(l) {}
    Either(L&& l) noexcept(std::is_nothrow_move_constructible_v<L>);// : type_tag(Tag::LeftTypeTag), left_value(std::move(l)) {}
    
    Either(const R& r) noexcept(std::is_nothrow_move_constructible_v<R>);// : type_tag(Tag::RightTypeTag), right_value(r) {}
    Either(R&& r) noexcept(std::is_nothrow_move_constructible_v<R>);// : type_tag(Tag::RightTypeTag), right_value(std::move(r)) {}

    Either(const Either& other)
        noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>);
    Either& operator=(const Either& other) 
        noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>);

    Either(Either&& other) 
        noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>);
    Either& operator=(Either&& other) 
        noexcept(std::is_nothrow_copy_constructible_v<L> && std::is_nothrow_copy_constructible_v<R>);;

    ~Either() noexcept; //{ destroy(); }

    //*   <--- specialized methods  --->
    bool isLeft() const noexcept;
    bool isRight() const noexcept;

    // provides an option to get a reference to an object
    // in an Optional wrapper. 
    // Warning: unsafety lies in the possibility of a dangling link
    Optional<const L&> unsafeGetLeft() noexcept;

    // provides an option to get a reference to an object
    // in an Optional wrapper. 
    // Warning: unsafety lies in the possibility of a dangling link
    Optional<const R&> unsafeGetRight() noexcept;

    // it is more secure that the Optional wrapper 
    // does not wrap a reference, but a copy of the object.
    Optional<L> safeGetLeft() noexcept;

    // it is more secure that the Optional wrapper 
    // does not wrap a reference, but a copy of the object.
    Optional<R> safeGetRight() noexcept;

    //*   <--- static mnemonic methods that create the Either object from an argument --->
    static Either fromLeft(const L& l) noexcept(std::is_nothrow_copy_constructible_v<L>);// { return Either(l); }
    static Either fromLeft(L&& l) noexcept(std::is_nothrow_copy_constructible_v<L>); //{ return Either(l); }
    static Either fromRight(const R& r) noexcept(std::is_nothrow_copy_constructible_v<R>); //{ return Either(r); }
    static Either fromRight(R&& r) noexcept(std::is_nothrow_copy_constructible_v<R>);// { return Either(r); }

};

}

#endif // THROWLESS_EITHER_H