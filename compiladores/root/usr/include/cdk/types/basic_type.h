#pragma once

#include <cdk/types/typename_type.h>
#include <cstdlib>
#include <memory>
#include <string>

namespace cdk {

  /**
   * This class represents a general type concept.
   */
  class basic_type {
    size_t _size = 0; // in bytes
    typename_type _name = TYPE_UNSPEC;

  protected:

    struct explicit_call_disabled {};

  protected:

    basic_type() :
        _size(0), _name(TYPE_UNSPEC) {
    }
    basic_type(size_t size, typename_type name) :
        _size(size), _name(name) {
    }

    virtual ~basic_type() noexcept = 0;

  public:

    size_t size() const {
      return _size;
    }
    typename_type name() const {
      return _name;
    }

  public:

    virtual std::string to_string() const = 0;

  };

  inline bool operator==(const std::shared_ptr<basic_type> t1,
                         const std::shared_ptr<basic_type> t2) {
    return t1->size() == t2->size() && t1->name() == t2->name();
  }
  inline bool operator!=(const std::shared_ptr<basic_type> t1,
                         const std::shared_ptr<basic_type> t2) {
    return !(t1 == t2);
  }

} // cdk
