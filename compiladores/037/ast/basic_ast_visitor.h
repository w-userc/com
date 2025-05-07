#ifndef UDF_BASIC_AST_VISITOR_H
#define UDF_BASIC_AST_VISITOR_H

//---------------------------------------------------------------------------

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

//---------------------------------------------------------------------------

namespace udf {

  class evaluation_node;
  class print_node;
  class read_node;
  class program_node;
  class tensor_capacity_node;
  class tensor_rank_node;
  class tensor_dimensions_node;
  class tensor_indexing_node;
  class tensor_reshape_node;
  class tensor_contraction_node;
  class function_declaration_node;
  class function_call_node;
  class variable_declaration_node;
  class for_node;
  class break_node;
  class continue_node;
  class return_node;
  class write_node;

  //---------------------------------------------------------------------------

  class basic_ast_visitor {
  public:
    virtual ~basic_ast_visitor() {
    }

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