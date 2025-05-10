#pragma once

#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing the input expression.
   * The type of the input_node will be determined during semantic analysis
   * based on the context where it's used.
   */
  class input_node : public cdk::expression_node {
  public:
    input_node(int lineno) :
        cdk::expression_node(lineno) {
      // Type will be set later by type_checker
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_input_node(this, level); }
  };

} // udf
