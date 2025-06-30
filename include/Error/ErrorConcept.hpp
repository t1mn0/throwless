#ifndef ERROR_CONCEPT_H
#define ERROR_CONCEPT_H

#include <string>
#include <concepts> // for: std::convertible_to<>;

namespace throwless {

//* <--- The basic concept that the "Error" type structure should correspond to --->
template<typename E>
concept Error = requires(E e) {
    { e.message() } -> std::convertible_to<std::string>;
};

} // end of namespace 'throwless'

#endif