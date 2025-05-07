#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void udf::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void udf::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void udf::xml_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

void udf::xml_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void udf::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void udf::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_program_node(udf::program_node * const node, int lvl) {
  openTag(node, lvl);
  node->statements()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_evaluation_node(udf::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_print_node(udf::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_read_node(udf::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_while_node(udf::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_if_node(udf::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_if_else_node(udf::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_tensor_capacity_node(udf::tensor_capacity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_rank_node(udf::tensor_rank_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_dimensions_node(udf::tensor_dimensions_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_indexing_node(udf::tensor_indexing_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  node->indices()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_reshape_node(udf::tensor_reshape_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  node->dimensions()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_contraction_node(udf::tensor_contraction_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_function_declaration_node(udf::function_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<public>" << (node->is_public() ? "true" : "false") << "</public>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<forward>" << (node->is_forward() ? "true" : "false") << "</forward>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<type>" << node->type()->name() << "</type>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<name>" << node->name() << "</name>" << std::endl;
  if (node->arguments()) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }
  if (node->block()) {
    openTag("block", lvl + 2);
    node->block()->accept(this, lvl + 4);
    closeTag("block", lvl + 2);
  }
  closeTag(node, lvl);
}

void udf::xml_writer::do_function_call_node(udf::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<name>" << node->name() << "</name>" << std::endl;
  if (node->arguments()) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_variable_declaration_node(udf::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<public>" << (node->is_public() ? "true" : "false") << "</public>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<forward>" << (node->is_forward() ? "true" : "false") << "</forward>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<type>" << node->type()->name() << "</type>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<name>" << node->name() << "</name>" << std::endl;
  if (node->initializer()) {
    openTag("initializer", lvl + 2);
    node->initializer()->accept(this, lvl + 4);
    closeTag("initializer", lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_for_node(udf::for_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if (node->initializers()) {
    openTag("initializers", lvl + 2);
    node->initializers()->accept(this, lvl + 4);
    closeTag("initializers", lvl + 2);
  }
  if (node->conditions()) {
    openTag("conditions", lvl + 2);
    node->conditions()->accept(this, lvl + 4);
    closeTag("conditions", lvl + 2);
  }
  if (node->increments()) {
    openTag("increments", lvl + 2);
    node->increments()->accept(this, lvl + 4);
    closeTag("increments", lvl + 2);
  }
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_break_node(udf::break_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_continue_node(udf::continue_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_return_node(udf::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if (node->retvals()) {
    node->retvals()->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_write_node(udf::write_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<newline>" << (node->newline() ? "true" : "false") << "</newline>" << std::endl;
  if (node->arguments()) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }
  closeTag(node, lvl);
}
