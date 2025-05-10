#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing return_node nodes.
   */
  class return_node : public cdk::basic_node {
    cdk::expression_node *_value; // Optional return value, nullptr if none

  public:
    // Constructor for return without value
    return_node(int lineno) :
        cdk::basic_node(lineno), _value(nullptr) {
    }

    // Constructor for return with value
    return_node(int lineno, cdk::expression_node *value) :
        cdk::basic_node(lineno), _value(value) {
    }

  public:
    cdk::expression_node* value() {
      return _value;
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_return_node(this, level); }
  };

} // udf
