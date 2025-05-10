#pragma once
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {
  class tensor_reshape_node : public cdk::expression_node {
    cdk::expression_node *_tensor_expr;
    cdk::sequence_node *_dimensions; // sequence of cdk::integer_node (positive literals)
  
  public:
    tensor_reshape_node(int lineno, cdk::expression_node *tensor_expr, cdk::sequence_node *dimensions) :
        cdk::expression_node(lineno), _tensor_expr(tensor_expr), _dimensions(dimensions) {}

    cdk::expression_node *tensor_expr() { return _tensor_expr; }

    cdk::sequence_node *dimensions() { return _dimensions; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_tensor_reshape_node(this, level); } 
  };
} // udf
