#ifndef THROWLESS_VOID_H
#define THROWLESS_VOID_H

#include <variant> // for std::monostate;

namespace throwless {

//? ? ? ? ?
// In category theory, Either is a monoidal category, where: 
// - Either<A, B> is an coproduct (A + B)
// - Void is neutral element (A + 0 = A)
using Void = std::monostate;
// std::monostate is an empty (unit) type,
// which is used as a stub to represent "nothing" in contexts 
// where a specific type should be specified, 
// but semantically it is necessary to pass the absence of a value

}

#endif