#pragma once

#include <cdk/ast/expression_node.h> // sizeof returns a value, so it's an expression

namespace udf {

  /**
   * Class for describing sizeof nodes (e.g., sizeof(expr)).
   * Returns an integer.
   */
  class sizeof_node : public cdk::expression_node {
    cdk::expression_node *_argument; // The expression whose size is to be determined

  public:
    sizeof_node(int lineno, cdk::expression_node *argument) :
        cdk::expression_node(lineno), _argument(argument) {
        // The type of sizeof_node itself is integer.
        // This would be set by the type checker.
    }

    cdk::expression_node* argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_sizeof_node(this, level); }
  };

} // udf
