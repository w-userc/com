#pragma once

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace cdk {

  /**
   * Class for describing left-values used as expressions (i.e., right-values).
   */
  class rvalue_node : public cdk::expression_node {
    lvalue_node *_lvalue;

  public:
    rvalue_node(int lineno, lvalue_node *lvalue) noexcept:
        cdk::expression_node(lineno), _lvalue(lvalue) {
    }

    cdk::lvalue_node *lvalue() noexcept { return _lvalue; }

    void accept(basic_ast_visitor *sp, int level) override { sp->do_rvalue_node(this, level); }

  };

} // cdk
