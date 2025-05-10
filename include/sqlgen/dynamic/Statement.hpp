#ifndef SQLGEN_DYNAMIC_STATEMENT_HPP_
#define SQLGEN_DYNAMIC_STATEMENT_HPP_

#include <rfl.hpp>

#include "CreateTable.hpp"
#include "DeleteFrom.hpp"
#include "Drop.hpp"
#include "Insert.hpp"
#include "SelectFrom.hpp"

namespace sqlgen::dynamic {

using Statement =
    rfl::TaggedUnion<"stmt", CreateTable, DeleteFrom, Drop, Insert, SelectFrom>;

}  // namespace sqlgen::dynamic

#endif
