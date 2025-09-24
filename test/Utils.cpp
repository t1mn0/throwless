#include "Utils.hpp"

namespace tmn::test_utils {

int UniqueTestObject::constructor_count = 0;
int UniqueTestObject::destructor_count = 0;
int UniqueTestObject::copy_count = 0;
int UniqueTestObject::move_count = 0;

int UniqueTestDeleter::delete_count = 0;
int UniqueTestDeleter::last_deleted_value = 0;

} // namespace tmn::test_utils;
