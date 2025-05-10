#pragma once

#include <string>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  /**
   * Class for describing function call nodes.
   */
  class function_call_node : public cdk::expression_node {
    std::string _identifier;          // Name of the function
    cdk::sequence_node *_arguments; // Arguments (sequence of expressions)

  public:
    function_call_node(int lineno, const std::string &identifier, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(arguments) {
    }
    
    // Constructor for identifier as an expression (e.g. function pointers if supported later)
    // function_call_node(int lineno, cdk::expression_node *identifier_expr, cdk::sequence_node *arguments) ...

  public:
    const std::string& identifier() const {
      return _identifier;
    }
    cdk::sequence_node* arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) { sp->do_function_call_node(this, level); }
  };

} // udf
