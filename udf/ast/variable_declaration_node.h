#pragma once

#include <string>
#include <cdk/ast/typed_node.h> // To store the variable's type
#include <cdk/ast/expression_node.h>
#include <cdk/types/basic_type.h>

namespace udf {

  enum class DeclarationQualifier {
    NONE = 0,         // Default (e.g. local, or private global)
    PUBLIC = 1,
    FORWARD = 2,      // Implies public for globals
    AUTO_DEDUCED = 4  // Special marker if 'auto' keyword was used for type
                      // The actual type in typed_node might be a placeholder initially.
  };

  /**
   * Class for describing variable declaration nodes.
   */
  class variable_declaration_node : public cdk::typed_node {
    DeclarationQualifier _qualifier;
    std::string _identifier;
    cdk::expression_node *_initializer; // Can be nullptr

  public:
    variable_declaration_node(int lineno, DeclarationQualifier qualifier,
                              std::shared_ptr<cdk::basic_type> var_type,
                              const std::string &identifier,
                              cdk::expression_node *initializer) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _initializer(initializer) {
      type(var_type); // Set the type using typed_node's method
    }

  public:
    DeclarationQualifier qualifier() const { return _qualifier; }
    // type() is inherited from cdk::typed_node
    const std::string& identifier() const { return _identifier; }
    cdk::expression_node* initializer() { return _initializer; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_variable_declaration_node(this, level); }
  };

} // udf
