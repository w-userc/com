#pragma once
#include <cdk/ast/expression_node.h>

namespace udf {

  class tensor_dims_node : public cdk::expression_node {
    cdk::expression_node *_tensor_expr;

  public:
    tensor_dims_node(int lineno, cdk::expression_node *tensor_expr) :
        cdk::expression_node(lineno), _tensor_expr(tensor_expr) {}

    cdk::expression_node *tensor_expr() { return _tensor_expr; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_tensor_dims_node(this, level); }
  };
} // udf
