#pragma once

#include "targets/basic_ast_visitor.h"
#include <vector> // For loop label tracking
#include <sstream>
#include <cdk/emitters/basic_postfix_emitter.h>

namespace udf {

  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<udf::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;

    // Context for loops (break/continue)
    std::vector<std::string> _for_cond_labels;  // Stores labels for loop condition/increment
    std::vector<std::string> _for_end_labels;   // Stores labels for loop end (for break)
    std::string _current_function_end_label; // For return statements

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<udf::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl_id) { // Using int for id, but returning string
      std::ostringstream oss;
      if (lbl_id < 0) // Special labels if needed
        oss << ".L" << std::abs(lbl_id); // Use abs for negative
      else
        oss << "_L" << lbl_id;
      return oss.str();
    }
    
    int newlbl_int() { // Returns int id for mklbl
        return ++_lbl;
    }
    
    // Helper for defining a label using an int id
    void define_lbl(int lbl_id) {
        _pf.LABEL(mklbl(lbl_id));
    }


  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end
  };
} // udf