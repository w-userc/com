#ifndef UDF_TYPE_CHECKER_H
#define UDF_TYPE_CHECKER_H

#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

//---------------------------------------------------------------------------

namespace udf {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class type_checker: public basic_ast_visitor {
    cdk::symbol_table<udf::symbol> &_symtab;
    std::shared_ptr<cdk::basic_type> _function;
    std::vector<std::shared_ptr<cdk::basic_type>> _functionArgs;
    bool _inBlock = false;
    bool _inLoop = false;

  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<udf::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
    }

  public:
    ~type_checker() {
      os().flush();
    }

  protected:
    void processUnaryExpression(cdk::unary_operation_node *const node, int lvl);
    void processBinaryExpression(cdk::binary_operation_node *const node, int lvl);
    void processComparisonExpression(cdk::binary_operation_node *const node, int lvl);
    void processLogicalExpression(cdk::binary_operation_node *const node, int lvl);

  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end

  };

} // udf

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING
//---------------------------------------------------------------------------

#define CHECK_TYPES(COMPILER, TYPE1, TYPE2) \
  { \
    if (TYPE1 == nullptr || TYPE2 == nullptr) { \
      throw std::string("wrong type (null) in type checking"); \
    } \
    if (TYPE1->name() != TYPE2->name()) { \
      throw std::string("wrong type in type checking"); \
    } \
  }

//---------------------------------------------------------------------------

#endif
