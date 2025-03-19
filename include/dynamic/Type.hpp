#ifndef SQLGEN_DYNAMIC_TABLE_TYPE_HPP_
#define SQLGEN_DYNAMIC_TABLE_TYPE_HPP_

#include <variant>

#include "types.hpp"

namespace sqlgen::dynamic {

using Type =
    std::variant<types::Unknown, types::Boolean, types::Float32, types::Float64,
                 types::Int8, types::Int16, types::Int32, types::Int64,
                 types::Text, types::Timestamp, types::TimestampWithTZ,
                 types::VarChar>;

}  // namespace sqlgen::dynamic

#endif
