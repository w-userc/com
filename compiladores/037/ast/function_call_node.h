#ifndef UDF_FUNCTION_CALL_NODE_H
#define UDF_FUNCTION_CALL_NODE_H

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  class function_call_node: public cdk::expression_node {
    std::string _name;
    cdk::sequence_node *_arguments;

  public:
    function_call_node(int lineno, const std::string &name, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _name(name), _arguments(arguments) {
    }

  public:
    const std::string &name() const {
      return _name;
    }

    cdk::sequence_node* arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // udf

#endif 