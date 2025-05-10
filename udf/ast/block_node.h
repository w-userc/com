#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  /**
   * Class for describing block nodes.
   * A block contains an optional sequence of declarations
   * and an optional sequence of instructions.
   */
  class block_node : public cdk::basic_node {
    cdk::sequence_node *_declarations;
    cdk::sequence_node *_instructions;

  public:
    block_node(int lineno, cdk::sequence_node *declarations, cdk::sequence_node *instructions) :
        cdk::basic_node(lineno), _declarations(declarations), _instructions(instructions) {
    }

  public:
    cdk::sequence_node *declarations() {
      return _declarations;
    }
    cdk::sequence_node *instructions() {
      return _instructions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }
  };

} // udf
