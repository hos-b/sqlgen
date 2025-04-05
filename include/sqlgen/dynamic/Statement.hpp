#ifndef SQLGEN_DYNAMIC_STATEMENT_HPP_
#define SQLGEN_DYNAMIC_STATEMENT_HPP_

#include <rfl.hpp>

#include "CreateTable.hpp"
#include "Insert.hpp"

namespace sqlgen::dynamic {

using Statement = rfl::TaggedUnion<"stmt", CreateTable, Insert>;

}  // namespace sqlgen::dynamic

#endif
