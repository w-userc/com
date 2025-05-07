#ifndef UDF_TENSOR_CAPACITY_NODE_H
#define UDF_TENSOR_CAPACITY_NODE_H

#include <cdk/ast/expression_node.h>

namespace udf {

  class tensor_capacity_node: public cdk::expression_node {
    cdk::expression_node *_tensor;

  public:
    tensor_capacity_node(int lineno, cdk::expression_node *tensor) :
        cdk::expression_node(lineno), _tensor(tensor) {
    }

  public:
    cdk::expression_node* tensor() {
      return _tensor;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_tensor_capacity_node(this, level);
    }

  };

} // udf

#endif 