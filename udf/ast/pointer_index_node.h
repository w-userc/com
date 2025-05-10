#pragma once

#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing pointer indexing nodes (e.g., ptr_expr[idx_expr]).
   * This is an l-value.
   */
  class pointer_index_node : public cdk::lvalue_node {
    cdk::expression_node *_base_ptr; // Expression resulting in a pointer
    cdk::expression_node *_index;    // Expression resulting in an integer index

  public:
    pointer_index_node(int lineno, cdk::expression_node *base_ptr, cdk::expression_node *index) :
        cdk::lvalue_node(lineno), _base_ptr(base_ptr), _index(index) {
    }

  public:
    cdk::expression_node* base_ptr() { return _base_ptr; }
    cdk::expression_node* index() { return _index; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_pointer_index_node(this, level); }
  };

} // udf
