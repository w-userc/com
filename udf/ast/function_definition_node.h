#pragma once

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include "ast/block_node.h" // Definition needs a body (block)
#include <cdk/types/basic_type.h>
// Assuming FunctionQualifier is defined (e.g. in function_declaration_node.h or a common place)
// If not, redefine or include here. For simplicity, assume it's accessible.
#include "ast/function_declaration_node.h" // For FunctionQualifier

namespace udf {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node : public cdk::typed_node { // Stores return type
    FunctionQualifier _qualifier; // public, or none (private)
    std::string _identifier;
    cdk::sequence_node *_parameters; // Sequence of variable_declaration_node
    udf::block_node *_body;

  public:
    function_definition_node(int lineno, FunctionQualifier qualifier,
                             std::shared_ptr<cdk::basic_type> return_type,
                             const std::string &identifier,
                             cdk::sequence_node *parameters,
                             udf::block_node *body) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier),
        _parameters(parameters), _body(body) {
      type(return_type);
      // Functions with bodies cannot be 'forward'
    }

  public:
    FunctionQualifier qualifier() const { return _qualifier; }
    // type() is inherited from cdk::typed_node for return type
    const std::string& identifier() const { return _identifier; }
    cdk::sequence_node* parameters() { return _parameters; }
    udf::block_node* body() { return _body; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_function_definition_noide(this, level); }
  };

} // udf
