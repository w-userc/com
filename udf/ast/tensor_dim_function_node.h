#pragma once
#include <cdk/ast/expression_node.h>

namespace udf {

  class tensor_dim_function_node : public cdk::expression_node {
    cdk::expression_node *_tensor_expr;
    cdk::expression_node *_index; // integer expression for dimension index

  public:
    tensor_dim_function_node(int lineno, cdk::expression_node *tensor_expr, cdk::expression_node *index) :
        cdk::expression_node(lineno), _tensor_expr(tensor_expr), _index(index) {}

    cdk::expression_node *tensor_expr() { return _tensor_expr; }

    cdk::expression_node *index() { return _index; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_tensor_dim_function_node(this, level); }
  };
} // udf
