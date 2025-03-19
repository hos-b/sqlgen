#ifndef SQLGEN_DYNAMIC_TABLE_HPP_
#define SQLGEN_DYNAMIC_TABLE_HPP_

#include <string>
#include <vector>

namespace sqlgen::dynamic {

struct Table {
    std::string alias;
    std::string name;
    std::string schema = "public";
};

}  // namespace sqlgen::dynamic

#endif
