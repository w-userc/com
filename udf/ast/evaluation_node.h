#pragma once

#include <cdk/ast/expression_node.h>
// basic_ast_visitor forward declaration will be handled by .auto/visitor_decls.h
// included in basic_ast_visitor.h

// Ensure basic_ast_visitor is declared before use in accept
class basic_ast_visitor;

namespace udf {

  /**
   * Class for describing evaluation nodes.
   */
  class evaluation_node : public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    evaluation_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

    cdk::expression_node *argument() { return _argument; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_evaluation_node(this, level); }

  };

} // udf
