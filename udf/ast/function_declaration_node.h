#pragma once

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/types/basic_type.h>

namespace udf {
  // Re-using DeclarationQualifier from variable_declaration_node for public/forward
  // Alternatively, define a specific one for functions if needed.
  // For now, assuming 'forward' is the main qualifier here.
  // 'public' is also possible for functions not 'forward'.

  enum class FunctionQualifier {
    NONE = 0,
    PUBLIC = 1,
    FORWARD = 2 // 'forward' implies public
  };

  /**
   * Class for describing function declaration nodes (e.g., forward declarations).
   */
  class function_declaration_node : public cdk::typed_node { // Stores return type
    FunctionQualifier _qualifier;
    std::string _identifier;
    cdk::sequence_node *_parameters; // Sequence of variable_declaration_node

  public:
    function_declaration_node(int lineno, FunctionQualifier qualifier,
                              std::shared_ptr<cdk::basic_type> return_type,
                              const std::string &identifier,
                              cdk::sequence_node *parameters) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _parameters(parameters) {
      type(return_type);
    }

  public:
    FunctionQualifier qualifier() const { return _qualifier; }
    // type() is inherited from cdk::typed_node for return type
    const std::string& identifier() const { return _identifier; }
    cdk::sequence_node* parameters() { return _parameters; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_function_declaration_node(this, level); }
  };

} // udf
