#pragma once

#include <cdk/types/typename_type.h>
#include <cdk/types/basic_type.h>
#include <cstdlib>

namespace cdk {

  /**
   * Primitive (i.e., non-structured non-indirect) types.
   */
  class primitive_type: public basic_type {
  public:
    //primitive_type() :
    //    basic_type(0, TYPE_UNSPEC) {
    //}
    explicit primitive_type(explicit_call_disabled, size_t size, typename_type name) :
        basic_type(size, name) {
    }

    ~primitive_type() = default;


  public:

    static auto create(size_t size, typename_type name) {
      return std::make_shared<primitive_type>(explicit_call_disabled(), size, name);
    }

    static auto cast(std::shared_ptr<basic_type> type) {
      return std::dynamic_pointer_cast<primitive_type>(type);
    }

  public:

    std::string to_string() const {
      if (name() == TYPE_INT) return "integer";
      if (name() == TYPE_DOUBLE) return "double";
      if (name() == TYPE_STRING) return "string";
      if (name() == TYPE_VOID) return "void";
      return "UNKNOWN-TYPE:" + std::to_string(name());
    }

  };

} // cdk
