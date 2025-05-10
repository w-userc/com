#pragma once

#include <cdk/ast/literal_node.h> // Technically not a literal in the same way string/int are
                                  // but it represents a constant value.
                                  // Could also inherit directly from expression_node.
#include <cdk/types/basic_type.h>
#include <cdk/types/primitive_type.h> // For creating a pointer-like type initially

namespace udf {

  /**
   * Class for describing the nullptr literal.
   */
  class nullptr_literal_node : public cdk::expression_node {
  public:
    nullptr_literal_node(int lineno) :
        cdk::expression_node(lineno) {
      // nullptr has a special pointer type, often compatible with any pointer type.
      // CDK might have a specific way to represent this, or it's a primitive type.
      // For now, let's assume its type will be set by type checker.
      // type(cdk::primitive_type::create(0, cdk::TYPE_POINTER)); // A generic pointer
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_nullptr_literal_node(this, level); }
  };

} // udf
