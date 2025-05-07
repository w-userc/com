#ifndef UDF_RETURN_NODE_H
#define UDF_RETURN_NODE_H

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  class return_node: public cdk::basic_node {
    cdk::sequence_node *_retvals;

  public:
    return_node(int lineno, cdk::sequence_node *retvals) :
        cdk::basic_node(lineno), _retvals(retvals) {
    }

  public:
    cdk::sequence_node* retvals() {
      return _retvals;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // udf

#endif 