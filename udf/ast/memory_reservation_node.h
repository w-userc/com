#pragma once

#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing memory reservation nodes (e.g., objects(int_expr)).
   * This expression returns a pointer. The type of the pointer (ptr<auto> or ptr<specific_type>)
   * would be determined by context or language rules.
   * For now, it's an expression, and its type will be set by the type checker.
   */
  class memory_reservation_node : public cdk::expression_node {
    cdk::expression_node *_count; // The integer expression for the number of objects

  public:
    memory_reservation_node(int lineno, cdk::expression_node *count) :
        cdk::expression_node(lineno), _count(count) {
    }

    cdk::expression_node* count_expression() {
      return _count;
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_memory_reservation_node(this, level); }
  };

} // udf
