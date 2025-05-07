#ifndef UDF_VARIABLE_DECLARATION_NODE_H
#define UDF_VARIABLE_DECLARATION_NODE_H

#include <cdk/ast/basic_node.h>
#include <cdk/types/basic_type.h>

namespace udf {

  class variable_declaration_node: public cdk::basic_node {
    bool _is_public;
    bool _is_forward;
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    cdk::expression_node *_initializer;

  public:
    variable_declaration_node(int lineno, bool is_public, bool is_forward,
                            std::shared_ptr<cdk::basic_type> type, const std::string &name,
                            cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _is_public(is_public), _is_forward(is_forward),
        _type(type), _name(name), _initializer(initializer) {
    }

  public:
    bool is_public() const {
      return _is_public;
    }

    bool is_forward() const {
      return _is_forward;
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }

    const std::string &name() const {
      return _name;
    }

    cdk::expression_node* initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // udf

#endif 