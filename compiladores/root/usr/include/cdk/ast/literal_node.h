#pragma once

#include <cdk/ast/expression_node.h>

namespace cdk {

  /**
   * Class for describing syntactic tree leaves for holding literal
   * nodes. This is a template class that will be instantiated
   * by the various classes for holding specific leaves.
   *
   * @param StoredType is the type held by the leaf
   * @see Double, Integer, String
   */
  template<typename StoredType>
  class literal_node : public expression_node {
    StoredType _value;

  protected:
    literal_node(int lineno, const StoredType &value) noexcept:
        expression_node(lineno), _value(value) {
    }

  public:
    const StoredType &value() const noexcept { return _value; }

  };

} // cdk
