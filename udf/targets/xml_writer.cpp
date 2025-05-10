#include <string>
#include <sstream> // For std::ostringstream
#include "targets/xml_writer.h"
#include "targets/type_checker.h" // For CHECK_TYPES, ASSERT_SAFE_EXPRESSIONS
#include ".auto/all_nodes.h"
#include <cdk/types/types.h>
#include <cdk/types/primitive_type.h>
#include <cdk/types/pointer_type.h>
#include <cdk/types/reference_type.h>
#include <cdk/types/functional_type.h>


// Helper function for type to string (copied from my previous response's type_checker.cpp)
static std::string type_to_xml_string(std::shared_ptr<cdk::basic_type> type) {
    if (!type) return "unknown_type";
    std::string name_str;
    switch (type->name()) {
        case cdk::TYPE_INT: name_str = "int"; break;
        case cdk::TYPE_DOUBLE: name_str = "real"; break;
        case cdk::TYPE_STRING: name_str = "string"; break;
        case cdk::TYPE_VOID: name_str = "void"; break;
        case cdk::TYPE_POINTER: {
            auto pt = std::dynamic_pointer_cast<cdk::pointer_type>(type);
            if (pt && pt->referenced()) return "ptr<" + type_to_xml_string(pt->referenced()) + ">";
            return "ptr<unknown>";
        }
         case cdk::TYPE_REFERENCE: { // For address_of
            auto rt = std::dynamic_pointer_cast<cdk::reference_type>(type);
            if (rt && rt->referenced()) return "ref<" + type_to_xml_string(rt->referenced()) + ">";
            return "ref<unknown>";
        }
        case cdk::TYPE_FUNCTIONAL: { /* ... */ name_str = "function"; break;}
        case cdk::TYPE_UNSPEC: name_str = "auto_or_unspecified"; break;
        default: name_str = "other_type_" + std::to_string(type->name()); break;
    }
    return name_str;
}

// Qualifier to string helpers (copied from my previous response)
static std::string qualifier_to_xml_string(udf::FunctionQualifier qual) {
    switch (qual) {
        case udf::FunctionQualifier::PUBLIC: return "public";
        case udf::FunctionQualifier::FORWARD: return "forward";
        case udf::FunctionQualifier::NONE: return "private";
        default: return "unknown_qualifier";
    }
}
static std::string qualifier_to_xml_string(udf::DeclarationQualifier qual) {
    switch (qual) {
        case udf::DeclarationQualifier::PUBLIC: return "public";
        case udf::DeclarationQualifier::FORWARD: return "forward";
        case udf::DeclarationQualifier::AUTO_DEDUCED: return "auto_deduced"; // This might be part of type now
        case udf::DeclarationQualifier::NONE: return "private_or_local";
        default: return "unknown_qualifier";
    }
}


namespace udf {
// Implementations from your provided xml_writer.cpp for CDK nodes
void xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) { openTag(node, lvl); closeTag(node, lvl); }
void xml_writer::do_data_node(cdk::data_node * const node, int lvl) { openTag(node, lvl); closeTag(node, lvl); }

void xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    if (node->node(i)) node->node(i)->accept(this, lvl + 2);
    else { os() << std::string(lvl + 2, ' ') << "<nil_node/>" << std::endl; }
  closeTag(node, lvl);
}

void xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) { process_literal(node, lvl); }
void xml_writer::do_string_node(cdk::string_node * const node, int lvl) { process_literal(node, lvl); }
void xml_writer::do_double_node(cdk::double_node * const node, int lvl) { process_literal(node, lvl); } // UDF uses 'real'

void xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) { // Helper from your file
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void xml_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) { do_unary_operation(node, lvl); }
void xml_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) { do_unary_operation(node, lvl); }
void xml_writer::do_not_node(cdk::not_node * const node, int lvl) { do_unary_operation(node, lvl); } // UDF '~'

void xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) { // Helper from your file
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void xml_writer::do_add_node(cdk::add_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_div_node(cdk::div_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_le_node(cdk::le_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) { do_binary_operation(node, lvl); }
void xml_writer::do_and_node(cdk::and_node * const node, int lvl) { do_binary_operation(node, lvl); } // UDF '&&'
void xml_writer::do_or_node(cdk::or_node * const node, int lvl) { do_binary_operation(node, lvl); }   // UDF '||'

void xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->name() << "'";
  if (node->type()) os() << " type='" << type_to_xml_string(node->type()) << "'";
  os() << "/>" << std::endl;
}
void xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2); // Adjusted from lvl + 4
  closeTag(node, lvl);
}
void xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2); // Adjusted
  reset_new_symbol(); // From your file
  node->rvalue()->accept(this, lvl + 2); // Adjusted
  closeTag(node, lvl);
}

// 037-UDF specific nodes
void xml_writer::do_program_node(udf::program_node * const node, int lvl) { // My version of program_node
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if (node->declarations()) { // My program_node has _declarations
      node->declarations()->accept(this, lvl + 1);
  }
  closeTag(node, lvl);
}
void xml_writer::do_evaluation_node(udf::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
// print_node, read_node, while_node from 037 are being replaced. Stubs for compilation if needed.
void udf::xml_writer::do_print_node(udf::print_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS; openTag(node,lvl); node->argument()->accept(this, lvl+2); closeTag(node,lvl);
}
void udf::xml_writer::do_read_node(udf::read_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS; openTag(node,lvl); node->argument()->accept(this, lvl+2); closeTag(node,lvl);
}
void udf::xml_writer::do_while_node(udf::while_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    openTag("condition", lvl + 2); node->condition()->accept(this, lvl + 4); closeTag("condition", lvl + 2);
    openTag("block", lvl + 2); node->block()->accept(this, lvl + 4); closeTag("block", lvl + 2);
    closeTag(node, lvl);
}
void udf::xml_writer::do_if_node(udf::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2); node->condition()->accept(this, lvl + 4); closeTag("condition", lvl + 2);
  openTag("then", lvl + 2); node->block()->accept(this, lvl + 4); closeTag("then", lvl + 2);
  closeTag(node, lvl);
}
void udf::xml_writer::do_if_else_node(udf::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2); node->condition()->accept(this, lvl + 4); closeTag("condition", lvl + 2);
  openTag("then", lvl + 2); node->thenblock()->accept(this, lvl + 4); closeTag("then", lvl + 2);
  openTag("else", lvl + 2); node->elseblock()->accept(this, lvl + 4); closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

// Skeletons for NEW UDF Nodes (merged with my previous, more detailed versions)
void xml_writer::do_address_of_node(udf::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 1, ' ') << "<lvalue_argument>" << std::endl;
  node->lvalue()->accept(this, lvl + 2);
  os() << std::string(lvl + 1, ' ') << "</lvalue_argument>" << std::endl;
  closeTag(node, lvl);
}
void xml_writer::do_block_node(udf::block_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("declarations", lvl + 1); if (node->declarations()) node->declarations()->accept(this, lvl + 2); closeTag("declarations", lvl + 1);
  openTag("instructions", lvl + 1); if (node->instructions()) node->instructions()->accept(this, lvl + 2); closeTag("instructions", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_break_node(udf::break_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); closeTag(node, lvl); }
void xml_writer::do_continue_node(udf::continue_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); closeTag(node, lvl); }
void xml_writer::do_for_node(udf::for_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("init", lvl + 1); if (node->init()) node->init()->accept(this, lvl + 2); closeTag("init", lvl + 1);
  openTag("condition", lvl + 1); if (node->condition()) node->condition()->accept(this, lvl + 2); closeTag("condition", lvl + 1);
  openTag("increment", lvl + 1); if (node->increment()) node->increment()->accept(this, lvl + 2); closeTag("increment", lvl + 1);
  openTag("instruction", lvl + 1); if (node->instruction()) node->instruction()->accept(this, lvl + 2); closeTag("instruction", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_function_call_node(udf::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 1, ' ') << "<identifier>" << node->identifier() << "</identifier>" << std::endl;
  openTag("arguments", lvl + 1); if (node->arguments()) node->arguments()->accept(this, lvl + 2); closeTag("arguments", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_function_declaration_node(udf::function_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 1, ' ') << "<qualifier>" << qualifier_to_xml_string(node->qualifier()) << "</qualifier>" << std::endl;
  os() << std::string(lvl + 1, ' ') << "<return_type>" << type_to_xml_string(node->type()) << "</return_type>" << std::endl;
  os() << std::string(lvl + 1, ' ') << "<identifier>" << node->identifier() << "</identifier>" << std::endl;
  openTag("parameters", lvl + 1); if (node->parameters()) node->parameters()->accept(this, lvl + 2); closeTag("parameters", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_function_definition_node(udf::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 1, ' ') << "<qualifier>" << qualifier_to_xml_string(node->qualifier()) << "</qualifier>" << std::endl;
  os() << std::string(lvl + 1, ' ') << "<return_type>" << type_to_xml_string(node->type()) << "</return_type>" << std::endl;
  os() << std::string(lvl + 1, ' ') << "<identifier>" << node->identifier() << "</identifier>" << std::endl;
  openTag("parameters", lvl + 1); if (node->parameters()) node->parameters()->accept(this, lvl + 2); closeTag("parameters", lvl + 1);
  openTag("body", lvl + 1); if (node->body()) node->body()->accept(this, lvl + 2); closeTag("body", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_input_node(udf::input_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  if (node->type()) os() << std::string(lvl+1,' ') << "<expected_type>" << type_to_xml_string(node->type()) << "</expected_type>" << std::endl;
  closeTag(node, lvl);
}
void xml_writer::do_memory_reservation_node(udf::memory_reservation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  openTag("count_expression", lvl + 1); node->count_expression()->accept(this, lvl + 2); closeTag("count_expression", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_nullptr_literal_node(udf::nullptr_literal_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); os() << std::string(lvl + 1, ' ') << "<value>nullptr</value>" << std::endl; closeTag(node, lvl);
}
void xml_writer::do_pointer_index_node(udf::pointer_index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  openTag("base_ptr", lvl + 1); node->base_ptr()->accept(this, lvl + 2); closeTag("base_ptr", lvl + 1);
  openTag("index", lvl + 1); node->index()->accept(this, lvl + 2); closeTag("index", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_return_node(udf::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  if (node->value()) { openTag("value", lvl+1); node->value()->accept(this, lvl + 2); closeTag("value", lvl+1); }
  closeTag(node, lvl);
}
void xml_writer::do_sizeof_node(udf::sizeof_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  openTag("argument_expression", lvl+1); node->argument()->accept(this, lvl + 2); closeTag("argument_expression", lvl+1);
  closeTag(node, lvl);
}
void xml_writer::do_variable_declaration_node(udf::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  os() << std::string(lvl + 1, ' ') << "<qualifier>" << qualifier_to_xml_string(node->qualifier()) << "</qualifier>" << std::endl;
  os() << std::string(lvl + 1, ' ') << "<type>" << type_to_xml_string(node->type()) << "</type>" << std::endl;
  os() << std::string(lvl + 1, ' ') << "<identifier>" << node->identifier() << "</identifier>" << std::endl;
  if (node->initializer()) { openTag("initializer", lvl + 1); node->initializer()->accept(this, lvl + 2); closeTag("initializer", lvl + 1); }
  closeTag(node, lvl);
}
void xml_writer::do_write_node(udf::write_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  openTag("arguments", lvl + 1); if (node->arguments()) node->arguments()->accept(this, lvl + 2); closeTag("arguments", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_writeln_node(udf::writeln_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl);
  openTag("arguments", lvl + 1); if (node->arguments()) node->arguments()->accept(this, lvl + 2); closeTag("arguments", lvl + 1);
  closeTag(node, lvl);
}
void xml_writer::do_tensor_capacity_node(udf::tensor_capacity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("tensor_expression", lvl+1); node->tensor_expr()->accept(this, lvl+2); closeTag("tensor_expression", lvl+1); closeTag(node, lvl);
}
void xml_writer::do_tensor_contraction_node(udf::tensor_contraction_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("left", lvl+1); node->left()->accept(this, lvl+2); closeTag("left", lvl+1); openTag("right", lvl+1); node->right()->accept(this, lvl+2); closeTag("right", lvl+1); closeTag(node, lvl);
}
void xml_writer::do_tensor_dim_function_node(udf::tensor_dim_function_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("tensor_expression", lvl+1); node->tensor_expr()->accept(this, lvl+2); closeTag("tensor_expression", lvl+1); openTag("index", lvl+1); node->index()->accept(this, lvl+2); closeTag("index", lvl+1); closeTag(node, lvl);
}
void xml_writer::do_tensor_dims_node(udf::tensor_dims_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("tensor_expression", lvl+1); node->tensor_expr()->accept(this, lvl+2); closeTag("tensor_expression", lvl+1); closeTag(node, lvl);
}
void xml_writer::do_tensor_index_node(udf::tensor_index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("tensor_expression", lvl+1); node->tensor_expr()->accept(this, lvl+2); closeTag("tensor_expression", lvl+1); openTag("coordinates", lvl+1); if(node->coordinates()) node->coordinates()->accept(this, lvl+2); closeTag("coordinates", lvl+1); closeTag(node, lvl);
}
void xml_writer::do_tensor_rank_node(udf::tensor_rank_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("tensor_expression", lvl+1); node->tensor_expr()->accept(this, lvl+2); closeTag("tensor_expression", lvl+1); closeTag(node, lvl);
}
void xml_writer::do_tensor_reshape_node(udf::tensor_reshape_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; openTag(node, lvl); openTag("tensor_expression", lvl+1); node->tensor_expr()->accept(this, lvl+2); closeTag("tensor_expression", lvl+1); openTag("dimensions", lvl+1); if(node->dimensions()) node->dimensions()->accept(this, lvl+2); closeTag("dimensions", lvl+1); closeTag(node, lvl);
}
} // namespace udf