#include <string>
#include "targets/type_checker.h"
#include ".auto/all_nodes.h"  // automatically generated
#include <cdk/types/primitive_type.h>
#include <cdk/types/pointer_type.h>
#include <cdk/types/functional_type.h>
// Assuming UDF types like tensor_type would be here if CDK provides them or if we define them.
// For now, the type system is simple (int, string, later double, pointer).

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

// Helper to check if a type is numeric (int or double for UDF)
bool is_numeric(std::shared_ptr<cdk::basic_type> type) {
    if (!type) return false;
    return type->name() == cdk::TYPE_INT || type->name() == cdk::TYPE_DOUBLE;
}

// Helper to check for compatible assignment or operation (e.g. int to real is ok)
void check_compatible_types(std::shared_ptr<cdk::basic_type> left, std::shared_ptr<cdk::basic_type> right, const std::string& context) {
    if (!left || !right) throw std::string("Type error: null type in " + context);
    if (left->name() == right->name()) return; // Same types are compatible

    // int can be converted to real
    if (left->name() == cdk::TYPE_DOUBLE && right->name() == cdk::TYPE_INT) return;

    // Pointers: ptr<auto> is compatible with any ptr<T>, and ptr<T> with ptr<auto>
    if (left->name() == cdk::TYPE_POINTER && right->name() == cdk::TYPE_POINTER) {
        auto lt = std::dynamic_pointer_cast<cdk::pointer_type>(left);
        auto rt = std::dynamic_pointer_cast<cdk::pointer_type>(right);
        if (lt && rt) {
            if (lt->referenced() && lt->referenced()->name() == cdk::TYPE_UNSPEC) return; // ptr<auto> on left
            if (rt->referenced() && rt->referenced()->name() == cdk::TYPE_UNSPEC) return; // ptr<auto> on right
            // Specific pointer types must match (or one is ptr<auto>)
            if (lt->referenced() && rt->referenced() && lt->referenced()->name() == rt->referenced()->name()) return;
        }
    }
    // nullptr (TYPE_POINTER with unspecified subtype by convention) is compatible with any pointer
    if (left->name() == cdk::TYPE_POINTER && right->name() == cdk::TYPE_POINTER) {
        auto rt = std::dynamic_pointer_cast<cdk::pointer_type>(right);
        if (rt && rt->referenced() && rt->referenced()->name() == cdk::TYPE_UNSPEC) { // Assuming nullptr is ptr<unspec>
             // This might need refinement: if rt represents nullptr.
             // A better way: nullptr_literal_node has a specific marker or its own type.
            return;
        }
    }
     if (right->name() == cdk::TYPE_POINTER && left->name() == cdk::TYPE_POINTER) { // Symmetrical for nullptr
        auto lt = std::dynamic_pointer_cast<cdk::pointer_type>(left);
        if (lt && lt->referenced() && lt->referenced()->name() == cdk::TYPE_UNSPEC) {
            return;
        }
    }


    throw std::string("Type mismatch in " + context + ": cannot convert from " + 
                      /*type_to_string(right)*/ " to " + /*type_to_string(left)*/ " (type names for now)");
}


//---------------------------------------------------------------------------
// CDK and 037-UDF node implementations (from your provided file)
//---------------------------------------------------------------------------
void udf::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i)) node->node(i)->accept(this, lvl + 1); // Process children
  }
}
void udf::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) { /* EMPTY */ }
void udf::type_checker::do_data_node(cdk::data_node *const node, int lvl) { /* EMPTY */ }

void udf::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}
void udf::type_checker::do_double_node(cdk::double_node *const node, int lvl) { // From 037, but UDF has 'real'
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}
void udf::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING)); // Size of pointer
}

void udf::type_checker::do_not_node(cdk::not_node *const node, int lvl) { // UDF has '~' for logical not
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 1);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) // Logical ops on integers
    throw std::string("integer expression expected in NOT operator");
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::processUnaryNumericExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 1);
  if (!is_numeric(node->argument()->type()))
    throw std::string("numeric expression expected in unary numeric operator");
  node->type(node->argument()->type()); // Result type is same as argument
}

void udf::type_checker::do_unary_minus_node(cdk::unary_minus_node *const node, int lvl) {
  processUnaryNumericExpression(node, lvl);
}
void udf::type_checker::do_unary_plus_node(cdk::unary_plus_node *const node, int lvl) {
  processUnaryNumericExpression(node, lvl);
}

void udf::type_checker::processBinaryNumericExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 1);
  node->right()->accept(this, lvl + 1);

  if (!is_numeric(node->left()->type()) || !is_numeric(node->right()->type()))
    throw std::string("numeric expressions expected in binary numeric operator");

  // Type promotion: if either is double, result is double
  if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE))
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  else
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::processBinaryIntegerExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 1);
  node->right()->accept(this, lvl + 1);
  if (!node->left()->is_typed(cdk::TYPE_INT) || !node->right()->is_typed(cdk::TYPE_INT))
    throw std::string("integer expressions expected for this operator");
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::processBinaryLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 1);
  node->right()->accept(this, lvl + 1);
  if (!node->left()->is_typed(cdk::TYPE_INT) || !node->right()->is_typed(cdk::TYPE_INT)) // Logical ops on integers
    throw std::string("integer expressions expected in logical operator");
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT)); // Result is int (0 or 1)
}

void udf::type_checker::do_add_node(cdk::add_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); }
void udf::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); }
void udf::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); }
void udf::type_checker::do_div_node(cdk::div_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); }
void udf::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) { processBinaryIntegerExpression(node, lvl); } // Modulo only for int

void udf::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));} // Result of comparison is int
void udf::type_checker::do_le_node(cdk::le_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));}
void udf::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));}
void udf::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));}

void udf::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));} // Can also be pointers
void udf::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) { processBinaryNumericExpression(node, lvl); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));} // Can also be pointers

void udf::type_checker::do_and_node(cdk::and_node *const node, int lvl) { processBinaryLogicalExpression(node, lvl); }
void udf::type_checker::do_or_node(cdk::or_node *const node, int lvl) { processBinaryLogicalExpression(node, lvl); }

void udf::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<udf::symbol> symbol = _symtab.find(id);
  if (symbol) {
    node->type(symbol->type());
  } else {
    throw "undeclared variable '" + id + "'";
  }
}
void udf::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}
void udf::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl); // Check lvalue, sets its type
  node->rvalue()->accept(this, lvl); // Check rvalue, sets its type

  check_compatible_types(node->lvalue()->type(), node->rvalue()->type(), "assignment");
  node->type(node->lvalue()->type()); // Type of assignment is type of lvalue
}

// 037 Program and statement nodes
void udf::type_checker::do_program_node(udf::program_node *const node, int lvl) {
  if (node->declarations()) node->declarations()->accept(this, lvl + 1);
}
void udf::type_checker::do_evaluation_node(udf::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 1);
}

// The print_node, read_node, while_node from 037 are being replaced.
// We add stubs for them for now if the old parser still generates them.
void udf::type_checker::do_print_node(udf::print_node * const node, int lvl) {
    node->argument()->accept(this, lvl + 1);
    // UDF print handles int, real, string. This logic would be more complex.
    if (!is_numeric(node->argument()->type()) && !node->argument()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("Cannot print expression of this type.");
    }
}
void udf::type_checker::do_read_node(udf::read_node * const node, int lvl) {
    node->argument()->accept(this, lvl + 1); // lvalue
    // The read value will be int or real, lvalue type must be compatible.
    // For 037, it was simpler. For UDF, 'input' node handles this.
    if (!is_numeric(node->argument()->type())) {
        throw std::string("Cannot read into a non-numeric variable.");
    }
}
void udf::type_checker::do_while_node(udf::while_node * const node, int lvl) {
    node->condition()->accept(this, lvl + 1);
    if (!node->condition()->is_typed(cdk::TYPE_INT)) // Condition must be convertible to boolean (int)
        throw std::string("integer expression expected in while condition");
    if (node->block()) node->block()->accept(this, lvl + 1);
}
void udf::type_checker::do_if_node(udf::if_node * const node, int lvl) {
    node->condition()->accept(this, lvl + 1);
    if (!node->condition()->is_typed(cdk::TYPE_INT))
        throw std::string("integer expression expected in if condition");
    if (node->block()) node->block()->accept(this, lvl + 1);
}
void udf::type_checker::do_if_else_node(udf::if_else_node * const node, int lvl) {
    node->condition()->accept(this, lvl + 1);
     if (!node->condition()->is_typed(cdk::TYPE_INT))
        throw std::string("integer expression expected in if-else condition");
    if (node->thenblock()) node->thenblock()->accept(this, lvl + 1);
    if (node->elseblock()) node->elseblock()->accept(this, lvl + 1);
}

//---------------------------------------------------------------------------
// SKELETONS for New UDF Nodes
//---------------------------------------------------------------------------
void udf::type_checker::do_address_of_node(udf::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 1);
  if (!node->lvalue()->type()) throw std::string("Cannot take address of untyped lvalue.");
  node->type(cdk::reference_type::create(4, node->lvalue()->type())); // or cdk::pointer_type
}
void udf::type_checker::do_block_node(udf::block_node *const node, int lvl) {
  _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 1);
  if (node->instructions()) node->instructions()->accept(this, lvl + 1);
  _symtab.pop();
}
void udf::type_checker::do_break_node(udf::break_node *const node, int lvl) { /* Nothing to type check */ }
void udf::type_checker::do_continue_node(udf::continue_node *const node, int lvl) { /* Nothing to type check */ }

void udf::type_checker::do_for_node(udf::for_node *const node, int lvl) {
  _symtab.push(); // Scope for loop-specific variables in init
  if (node->init()) node->init()->accept(this, lvl + 1);
  if (node->condition()) {
      node->condition()->accept(this, lvl + 1);
      // Assuming condition is a sequence, check each element is int for boolean context
      // For simplicity, if it's a single expression:
      if (node->condition()->size() > 0) {
          auto cond_expr = dynamic_cast<cdk::expression_node*>(node->condition()->node(0));
          if (cond_expr && !cond_expr->is_typed(cdk::TYPE_INT))
            throw std::string("integer expression expected in for loop condition");
      }
  }
  if (node->increment()) node->increment()->accept(this, lvl + 1);
  if (node->instruction()) node->instruction()->accept(this, lvl + 1);
  _symtab.pop();
}
void udf::type_checker::do_function_call_node(udf::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;
  // Look up function symbol, check argument types against parameter types, set return type.
  // This is complex. For now, a placeholder.
  // std::shared_ptr<udf::symbol> func_symbol = _symtab.find(node->identifier());
  // if (!func_symbol || !func_symbol->is_typed(cdk::TYPE_FUNCTIONAL)) throw "unknown function: " + node->identifier();
  // auto func_type = std::dynamic_pointer_cast<cdk::functional_type>(func_symbol->type());
  // node->type(func_type->output(0)); // Set return type
  // if (node->arguments()) node->arguments()->accept(this, lvl + 1); // Check args
}
void udf::type_checker::do_function_declaration_node(udf::function_declaration_node *const node, int lvl) {
  // Add symbol to table. Check for redefinitions.
  // Type of parameters needs to be set/checked.
  // std::string id = node->identifier();
  // if (_symtab.find_local(id)) throw "Redeclaration of " + id;
  // _symtab.insert(id, std::make_shared<udf::symbol>(node->type(), id)); // node->type() is return type
  // if (node->parameters()) node->parameters()->accept(this, lvl + 1); // Type check params
}
void udf::type_checker::do_function_definition_node(udf::function_definition_node *const node, int lvl) {
  // Similar to declaration, then push scope for body.
  // _symtab.push();
  // if (node->parameters()) node->parameters()->accept(this, lvl + 1); // Add params to new scope
  // if (node->body()) node->body()->accept(this, lvl + 1);
  // _symtab.pop();
}
void udf::type_checker::do_input_node(udf::input_node *const node, int lvl) {
  ASSERT_UNSPEC;
  // Type is determined by context. Manual: "de acordo com o tipo esperado (inteiro ou real)".
  // If used with write/writeln, it's int.
  // This requires type inference or context passing. For now, default to int.
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}
void udf::type_checker::do_memory_reservation_node(udf::memory_reservation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->count_expression()->accept(this, lvl + 1);
  if (!node->count_expression()->is_typed(cdk::TYPE_INT))
    throw std::string("integer expression expected for objects count");
  // Result is ptr<auto> or context-dependent. For now, ptr<void> or ptr<unspec>.
  node->type(cdk::pointer_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}
void udf::type_checker::do_nullptr_literal_node(udf::nullptr_literal_node *const node, int lvl) {
  ASSERT_UNSPEC;
  // nullptr is compatible with any pointer type. Represent as ptr<void> or ptr<unspec>.
  node->type(cdk::pointer_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}
void udf::type_checker::do_pointer_index_node(udf::pointer_index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->base_ptr()->accept(this, lvl + 1);
  node->index()->accept(this, lvl + 1);
  if (!node->base_ptr()->is_typed(cdk::TYPE_POINTER))
    throw std::string("pointer type expected for base of index operation");
  if (!node->index()->is_typed(cdk::TYPE_INT))
    throw std::string("integer type expected for index of pointer operation");
  auto ptr_type = std::dynamic_pointer_cast<cdk::pointer_type>(node->base_ptr()->type());
  if (!ptr_type || !ptr_type->referenced()) throw std::string("Invalid pointer type for indexing.");
  node->type(ptr_type->referenced()); // Result type is the pointed-to type
}
void udf::type_checker::do_return_node(udf::return_node *const node, int lvl) {
  if (node->value()) {
    node->value()->accept(this, lvl + 1);
    // Check if return value's type matches function's declared return type (needs context).
  }
}
void udf::type_checker::do_sizeof_node(udf::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 1); // Argument needs to be type-checked to know its size
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT)); // sizeof returns an int
}
void udf::type_checker::do_variable_declaration_node(udf::variable_declaration_node *const node, int lvl) {
  // Type should already be set by parser (e.g. int, real, or a placeholder for auto).
  // If 'auto', initializer type determines variable type.
  if (node->initializer()) {
    node->initializer()->accept(this, lvl + 1);
    if (node->is_typed(cdk::TYPE_UNSPEC)) { // 'auto' case
        if(!node->initializer()->type()) throw std::string("Cannot deduce auto type from untyped initializer.");
        node->type(node->initializer()->type()); // Infer type
    } else {
        check_compatible_types(node->type(), node->initializer()->type(), "variable initialization");
    }
  }
  // Add to symbol table
  // std::string id = node->identifier();
  // if (_symtab.find_local(id)) throw "Redeclaration of " + id;
  // _symtab.insert(id, std::make_shared<udf::symbol>(node->type(), id));
}
void udf::type_checker::do_write_node(udf::write_node *const node, int lvl) {
  if (node->arguments()) node->arguments()->accept(this, lvl + 1);
  // Each argument must be printable (int, real, string)
}
void udf::type_checker::do_writeln_node(udf::writeln_node *const node, int lvl) {
  if (node->arguments()) node->arguments()->accept(this, lvl + 1);
}

// Tensor Nodes Skeletons
void udf::type_checker::do_tensor_capacity_node(udf::tensor_capacity_node *const node, int lvl) {
    ASSERT_UNSPEC; node->tensor_expr()->accept(this, lvl + 1); /* Check tensor_expr is tensor */ node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}
void udf::type_checker::do_tensor_contraction_node(udf::tensor_contraction_node *const node, int lvl) {
    ASSERT_UNSPEC; node->left()->accept(this, lvl + 1); node->right()->accept(this, lvl + 1); /* Check compatibility and result type */
}
void udf::type_checker::do_tensor_dim_function_node(udf::tensor_dim_function_node *const node, int lvl) {
    ASSERT_UNSPEC; node->tensor_expr()->accept(this, lvl + 1); node->index()->accept(this, lvl + 1); /* Check index is int */ node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}
void udf::type_checker::do_tensor_dims_node(udf::tensor_dims_node *const node, int lvl) {
    ASSERT_UNSPEC; node->tensor_expr()->accept(this, lvl + 1); /* Result is ptr to int vector */
    // node->type(cdk::pointer_type::create(4, cdk::primitive_type::create(4, cdk::TYPE_INT)));
}
void udf::type_checker::do_tensor_index_node(udf::tensor_index_node *const node, int lvl) {
    ASSERT_UNSPEC; node->tensor_expr()->accept(this, lvl + 1); if(node->coordinates()) node->coordinates()->accept(this, lvl + 1); /* Check result type */
}
void udf::type_checker::do_tensor_rank_node(udf::tensor_rank_node *const node, int lvl) {
    ASSERT_UNSPEC; node->tensor_expr()->accept(this, lvl + 1); node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}
void udf::type_checker::do_tensor_reshape_node(udf::tensor_reshape_node *const node, int lvl) {
    ASSERT_UNSPEC; node->tensor_expr()->accept(this, lvl + 1); if(node->dimensions()) node->dimensions()->accept(this, lvl + 1); /* Check result type */
}