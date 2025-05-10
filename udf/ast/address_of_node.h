#pragma once

#include <cdk/ast/unary_operation_node.h>
#include <cdk/ast/lvalue_node.h>

namespace udf {

  /**
   * Class for describing the address-of operator node (e.g., lval?)
   */
  class address_of_node : public cdk::unary_operation_node {
  public:
    // Argument must be an lvalue
    address_of_node(int lineno, cdk::lvalue_node *arg) :
        cdk::unary_operation_node(lineno, arg) {
    }

    cdk::lvalue_node *lvalue() {
      return dynamic_cast<cdk::lvalue_node*>(argument());
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_address_of_node(this, level);
    }

  };

} // udf
