#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void udf::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void udf::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void udf::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

  // in udf, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_unary_minus_node(cdk::unary_minus_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void udf::type_checker::do_unary_plus_node(cdk::unary_plus_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  // in udf, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<udf::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void udf::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void udf::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = std::make_shared<udf::symbol>(cdk::primitive_type::create(4, cdk::TYPE_INT), id);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);
  }

  if (!node->lvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (!node->rvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of assignment expression");

  // in udf, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void udf::type_checker::do_program_node(udf::program_node *const node, int lvl) {
  // EMPTY
}

void udf::type_checker::do_evaluation_node(udf::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void udf::type_checker::do_print_node(udf::print_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_read_node(udf::read_node *const node, int lvl) {
  try {
    node->argument()->accept(this, lvl);
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

//---------------------------------------------------------------------------

void udf::type_checker::do_while_node(udf::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_if_node(udf::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void udf::type_checker::do_if_else_node(udf::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_tensor_capacity_node(udf::tensor_capacity_node *const node, int lvl) {
  node->tensor()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_tensor_rank_node(udf::tensor_rank_node *const node, int lvl) {
  node->tensor()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_tensor_dimensions_node(udf::tensor_dimensions_node *const node, int lvl) {
  node->tensor()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_tensor_indexing_node(udf::tensor_indexing_node *const node, int lvl) {
  node->tensor()->accept(this, lvl + 2);
  node->indices()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_tensor_reshape_node(udf::tensor_reshape_node *const node, int lvl) {
  node->tensor()->accept(this, lvl + 2);
  node->dimensions()->accept(this, lvl + 2);
  node->type(node->tensor()->type());
}

void udf::type_checker::do_tensor_contraction_node(udf::tensor_contraction_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void udf::type_checker::do_function_declaration_node(udf::function_declaration_node *const node, int lvl) {
  std::vector<std::shared_ptr<cdk::basic_type>> args;
  if (node->arguments()) {
    for (size_t i = 0; i < node->arguments()->size(); i++) {
      cdk::typed_node *arg = dynamic_cast<cdk::typed_node*>(node->arguments()->node(i));
      args.push_back(arg->type());
    }
  }

  std::shared_ptr<udf::symbol> symbol = std::make_shared<udf::symbol>(node->type(), node->name(), args);
  if (!_symtab.insert(node->name(), symbol)) {
    throw std::string("function '" + node->name() + "' already defined");
  }

  if (node->block()) {
    _function = node->type();
    _functionArgs = args;
    node->block()->accept(this, lvl + 2);
  }
}

void udf::type_checker::do_function_call_node(udf::function_call_node *const node, int lvl) {
  std::shared_ptr<udf::symbol> symbol = _symtab.find(node->name());
  if (symbol == nullptr) {
    throw std::string("function '" + node->name() + "' not defined");
  }

  if (node->arguments()) {
    if (node->arguments()->size() != symbol->args().size()) {
      throw std::string("wrong number of arguments in function call");
    }

    for (size_t i = 0; i < node->arguments()->size(); i++) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
      arg->accept(this, lvl + 2);
      if (arg->type()->name() != symbol->args()[i]->name()) {
        throw std::string("wrong type in argument " + std::to_string(i + 1));
      }
    }
  }

  node->type(symbol->type());
}

//---------------------------------------------------------------------------

void udf::type_checker::do_variable_declaration_node(udf::variable_declaration_node *const node, int lvl) {
  if (node->initializer()) {
    node->initializer()->accept(this, lvl + 2);
    if (node->type()->name() == cdk::TYPE_INT) {
      if (node->initializer()->type()->name() != cdk::TYPE_INT) {
        throw std::string("wrong type for initializer (integer expected).");
      }
    } else if (node->type()->name() == cdk::TYPE_DOUBLE) {
      if (node->initializer()->type()->name() != cdk::TYPE_INT && node->initializer()->type()->name() != cdk::TYPE_DOUBLE) {
        throw std::string("wrong type for initializer (integer or double expected).");
      }
    } else if (node->type()->name() == cdk::TYPE_STRING) {
      if (node->initializer()->type()->name() != cdk::TYPE_STRING) {
        throw std::string("wrong type for initializer (string expected).");
      }
    } else if (node->type()->name() == cdk::TYPE_POINTER) {
      if (node->initializer()->type()->name() != cdk::TYPE_POINTER) {
        throw std::string("wrong type for initializer (pointer expected).");
      }
    } else {
      throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->name();
  std::shared_ptr<udf::symbol> symbol = std::make_shared<udf::symbol>(node->type(), id, 0);
  if (!_symtab.insert(id, symbol)) {
    throw std::string("variable '" + id + "' already defined");
  }
}

//---------------------------------------------------------------------------

void udf::type_checker::do_for_node(udf::for_node *const node, int lvl) {
  if (node->initializers()) {
    node->initializers()->accept(this, lvl + 2);
  }
  if (node->conditions()) {
    node->conditions()->accept(this, lvl + 2);
  }
  if (node->increments()) {
    node->increments()->accept(this, lvl + 2);
  }
  node->block()->accept(this, lvl + 2);
}

void udf::type_checker::do_break_node(udf::break_node *const node, int lvl) {
  if (!_inLoop) {
    throw std::string("break statement outside loop");
  }
}

void udf::type_checker::do_continue_node(udf::continue_node *const node, int lvl) {
  if (!_inLoop) {
    throw std::string("continue statement outside loop");
  }
}

void udf::type_checker::do_return_node(udf::return_node *const node, int lvl) {
  if (_function == nullptr) {
    throw std::string("return statement outside function");
  }

  if (node->retvals()) {
    if (node->retvals()->size() != 1) {
      throw std::string("wrong number of return values");
    }

    node->retvals()->node(0)->accept(this, lvl + 2);
    cdk::expression_node *retval = dynamic_cast<cdk::expression_node*>(node->retvals()->node(0));
    if (retval->type()->name() != _function->name()) {
      throw std::string("wrong type in return statement");
    }
  } else if (_function->name() != cdk::TYPE_VOID) {
    throw std::string("missing return value");
  }
}

//---------------------------------------------------------------------------

void udf::type_checker::do_write_node(udf::write_node *const node, int lvl) {
  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 2);
  }
}
