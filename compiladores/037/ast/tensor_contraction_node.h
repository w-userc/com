#ifndef UDF_TENSOR_CONTRACTION_NODE_H
#define UDF_TENSOR_CONTRACTION_NODE_H

#include <cdk/ast/binary_operation_node.h>

namespace udf {

  class tensor_contraction_node: public cdk::binary_operation_node {
  public:
    tensor_contraction_node(int lineno, cdk::expression_node *left, cdk::expression_node *right) :
        cdk::binary_operation_node(lineno, left, right) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_tensor_contraction_node(this, level);
    }

  };

} // udf

#endif 