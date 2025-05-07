#ifndef UDF_COMPILER_H
#define UDF_COMPILER_H

#include <cdk/compiler.h>
#include <cdk/symbol_table.h>
#include "targets/symbol.h"

namespace udf {

  class compiler: public cdk::compiler {
    cdk::symbol_table<udf::symbol> _symtab;

  public:
    compiler() :
        cdk::compiler("UDF") {
    }

  public:
    cdk::symbol_table<udf::symbol> &symtab() {
      return _symtab;
    }

  };

} // udf

#endif 