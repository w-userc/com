#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <cdk/compiler.h>
#include <cdk/symbol_table.h>
#include "targets/symbol.h" // Assuming symbol.h is in the same directory or accessible

/* do not edit -- include node forward declarations */
// The forward declarations can be generated into .auto/all_nodes.h or similar
// For simplicity, basic_node.h (from CDK) and all_nodes.h (from .auto) should cover this
#define __NODE_DECLARATIONS_ONLY__
#include ".auto/all_nodes.h"  // This will include forward declarations if set up correctly
#undef __NODE_DECLARATIONS_ONLY__
/* do not edit -- end */

namespace cdk {
  // Forward declare CDK nodes that might be directly used in visitor signatures if not covered by all_nodes.h
  // e.g. sequence_node, integer_node, etc.
  // This is usually handled by CDK's infrastructure.
}
namespace udf {
  // Forward declare UDF nodes if not covered by all_nodes.h
  // e.g. evaluation_node, if_node, etc.
}


class basic_ast_visitor {
protected:
  std::shared_ptr<cdk::compiler> _compiler;

private:
  std::shared_ptr<udf::symbol> _new_symbol;

protected:
  basic_ast_visitor(std::shared_ptr<cdk::compiler> compiler) :
      _compiler(compiler) {
  }

  bool debug() {
    return _compiler->debug();
  }

  std::ostream &os() {
    return *_compiler->ostream();
  }

public:
  virtual ~basic_ast_visitor() = default;

public:
  std::shared_ptr<udf::symbol> new_symbol() {
    return _new_symbol;
  }
  void set_new_symbol(std::shared_ptr<udf::symbol> symbol) {
    _new_symbol = symbol;
  }
  void reset_new_symbol() {
    _new_symbol = nullptr;
  }

public:
  // Visitor methods for all nodes are declared here:
  // These are generated into ".auto/visitor_decls.h"
  // and included by the #include directive below.

  // do not edit these lines
#define __IN_VISITOR_HEADER__
#define __PURE_VIRTUAL_DECLARATIONS_ONLY__ // Makes them pure virtual in this base class
#include ".auto/visitor_decls.h"      // automatically generated
#undef __PURE_VIRTUAL_DECLARATIONS_ONLY__
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end
};