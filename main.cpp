#include "include/Optional/Optional.hpp"
#include "include/Result/Result.hpp"
#include "include/Error/Error.hpp"

int main() {
    throwless::StringError e{};
    auto t = throwless::Result<void, throwless::StringError>::Err(e);
}