#pragma once
#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  class tensor_index_node : public cdk::lvalue_node {
    cdk::expression_node *_tensor_expr;
    cdk::sequence_node *_coordinates; // sequence of integer expressions
  //
  public:
    tensor_index_node(int lineno, cdk::expression_node *tensor_expr, cdk::sequence_node *coordinates) :
        cdk::lvalue_node(lineno), _tensor_expr(tensor_expr), _coordinates(coordinates) {}

    cdk::expression_node *tensor_expr() { return _tensor_expr; }

    cdk::sequence_node *coordinates() { return _coordinates; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_tensor_index_node(this, level); }
  };
} // udf
