#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/symbol_table.h>
#include "targets/basic_ast_visitor.h" // Inherits from basic_ast_visitor
#include "targets/symbol.h"

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
    // Helper functions (already in 037 base)
    void openTag(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "<" + tag + ">" << std::endl;
    }
    void openTag(const cdk::basic_node *node, int lvl) {
      openTag(node->label(), lvl);
    }
    void closeTag(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "</" + tag + ">" << std::endl;
    }
    void closeTag(const cdk::basic_node *node, int lvl) {
      closeTag(node->label(), lvl);
    }
    template <typename T>
    void process_literal(cdk::literal_node<T> *const node, int lvl) {
      os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->value() << "</" << node->label() << ">" << std::endl;
    }


  public:
    // Visitor methods for all nodes are declared here:
    // These are generated into ".auto/visitor_decls.h"
    // and included by the #include directive below.
    // In this concrete class, they are not pure virtual.
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h" // Re-include to get non-pure declarations
#undef __IN_VISITOR_HEADER__
  };

} // udf