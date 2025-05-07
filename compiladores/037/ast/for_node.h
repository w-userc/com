#ifndef UDF_FOR_NODE_H
#define UDF_FOR_NODE_H

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  class for_node: public cdk::basic_node {
    cdk::sequence_node *_initializers;
    cdk::sequence_node *_conditions;
    cdk::sequence_node *_increments;
    cdk::basic_node *_block;

  public:
    for_node(int lineno, cdk::sequence_node *initializers, cdk::sequence_node *conditions,
            cdk::sequence_node *increments, cdk::basic_node *block) :
        cdk::basic_node(lineno), _initializers(initializers), _conditions(conditions),
        _increments(increments), _block(block) {
    }

  public:
    cdk::sequence_node* initializers() {
      return _initializers;
    }

    cdk::sequence_node* conditions() {
      return _conditions;
    }

    cdk::sequence_node* increments() {
      return _increments;
    }

    cdk::basic_node* block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // udf

#endif 