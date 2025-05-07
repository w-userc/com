#ifndef UDF_XML_WRITER_H
#define UDF_XML_WRITER_H

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/integer_node.h>
#include <cdk/ast/string_node.h>
#include <cdk/ast/double_node.h>
#include <cdk/ast/binary_operation_node.h>
#include <cdk/ast/unary_operation_node.h>
#include <cdk/ast/assignment_node.h>
#include <cdk/ast/variable_node.h>
#include <cdk/ast/rvalue_node.h>
#include <cdk/ast/if_node.h>
#include <cdk/ast/if_else_node.h>
#include <cdk/ast/while_node.h>
#include "targets/basic_ast_visitor.h"

//---------------------------------------------------------------------------

namespace udf {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class xml_writer: public basic_ast_visitor {
    cdk::symbol_table<udf::symbol> &_symtab;

  public:
    xml_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<udf::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
    }

  public:
    ~xml_writer() {
      os().flush();
    }

  private:
    void openTag(const std::string &name, int lvl) {
      os() << std::string(lvl, ' ') + "<" + name + ">" << std::endl;
    }
    void openTag(const cdk::basic_node *node, int lvl) {
      openTag(node->label(), lvl);
    }
    void closeTag(const std::string &name, int lvl) {
      os() << std::string(lvl, ' ') + "</" + name + ">" << std::endl;
    }
    void closeTag(const cdk::basic_node *node, int lvl) {
      closeTag(node->label(), lvl);
    }

  protected:
    void processUnaryExpression(cdk::unary_operation_node *const node, int lvl);
    void processBinaryExpression(cdk::binary_operation_node *const node, int lvl);

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
