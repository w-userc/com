#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h> // For multiple expressions

namespace udf {

  /**
   * Class for describing writeln nodes.
   */
  class writeln_node : public cdk::basic_node {
    cdk::sequence_node *_arguments; // Sequence of expressions to print

  public:
    writeln_node(int lineno, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _arguments(arguments) {
    }

  public:
    cdk::sequence_node* arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_writeln_node(this, level); }
  };

} // udf
