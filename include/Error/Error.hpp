#ifndef ERROR_H
#define ERROR_H

template<typename E>
concept Error = requires(E e) {
    { e.message() } -> std::convertible_to<std::string>;
};

struct StringError {
    std::string msg;
    std::string message() const { return msg; }
};

struct CodeError {
    int code;
    std::string message() const { return "Error code: " + std::to_string(code); }
};

#endif