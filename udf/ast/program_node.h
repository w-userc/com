#pragma once

#include <cdk/ast/basic_node.h> // For cdk::basic_node (actually sequence_node)
#include <cdk/ast/sequence_node.h>

// Ensure basic_ast_visitor is declared before use in accept
class basic_ast_visitor;

namespace udf {

  /**
   * Class for describing program nodes (the root of the UDF AST).
   * It typically contains a sequence of declarations (variables and functions).
   */
  class program_node : public cdk::basic_node { // Or cdk::sequence_node if it directly holds them
    cdk::sequence_node *_declarations; // Top-level declarations

  public:
    program_node(int lineno, cdk::sequence_node *declarations) :
        cdk::basic_node(lineno), _declarations(declarations) {
    }

    cdk::sequence_node *declarations() { 
      return _declarations; 
    }

    // The `037` `program_node` had `_statements`. The UDF grammar implies a sequence of `declaração`.
    // I'll align with the new understanding. If `037` `_statements` was a sequence of top-level items,
    // then `_declarations` serves a similar role.

    void accept(basic_ast_visitor *sp, int level) { sp->do_program_node(this, level); }
  };

} // udf
