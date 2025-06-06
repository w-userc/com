#pragma once

#include <cdk/types/basic_type.h>

namespace cdk {

  /**
   * This class represents a reference type concept (such as a C pointer or a C++ reference).
   */
  struct reference_type: public basic_type {
    std::shared_ptr<basic_type> _referenced = nullptr;

  public:
    explicit reference_type(explicit_call_disabled, size_t size,  std::shared_ptr<basic_type> referenced) :
        basic_type(size, TYPE_POINTER), _referenced(referenced) {
    }

    ~reference_type() = default;

    std::shared_ptr<basic_type> referenced() const {
      return _referenced;
    }

  public:
    
    std::string to_string() const {
      return "@" + _referenced->to_string();
    }

  public:

    static auto create(size_t size, std::shared_ptr<basic_type> referenced) {
      return std::make_shared<reference_type>(explicit_call_disabled(), size, referenced);
    }

    static auto cast(std::shared_ptr<basic_type> type) {
      return std::dynamic_pointer_cast<reference_type>(type);
    }

  };

} // cdk
