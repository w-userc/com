/*
#include ".auto/all_nodes.h" // Includes all UDF and CDK node headers
#include "targets/basic_ast_visitor.h" // For the visitor type

// Nodes from 037 base 
void udf::evaluation_node::accept(basic_ast_visitor *sp, int level) { sp->do_evaluation_node(this, level); }
void udf::if_else_node::accept(basic_ast_visitor *sp, int level) { sp->do_if_else_node(this, level); }
void udf::if_node::accept(basic_ast_visitor *sp, int level) { sp->do_if_node(this, level); }
void udf::program_node::accept(basic_ast_visitor *sp, int level) { sp->do_program_node(this, level); }

// New UDF Nodes
void udf::address_of_node::accept(basic_ast_visitor *sp, int level) { sp->do_address_of_node(this, level); }
void udf::block_node::accept(basic_ast_visitor *sp, int level) { sp->do_block_node(this, level); }
void udf::break_node::accept(basic_ast_visitor *sp, int level) { sp->do_break_node(this, level); }
void udf::continue_node::accept(basic_ast_visitor *sp, int level) { sp->do_continue_node(this, level); }
void udf::for_node::accept(basic_ast_visitor *sp, int level) { sp->do_for_node(this, level); }
void udf::function_call_node::accept(basic_ast_visitor *sp, int level) { sp->do_function_call_node(this, level); }
void udf::function_declaration_node::accept(basic_ast_visitor *sp, int level) { sp->do_function_declaration_node(this, level); }
void udf::function_definition_node::accept(basic_ast_visitor *sp, int level) { sp->do_function_definition_node(this, level); }
void udf::input_node::accept(basic_ast_visitor *sp, int level) { sp->do_input_node(this, level); }
void udf::memory_reservation_node::accept(basic_ast_visitor *sp, int level) { sp->do_memory_reservation_node(this, level); }
void udf::nullptr_literal_node::accept(basic_ast_visitor *sp, int level) { sp->do_nullptr_literal_node(this, level); }
void udf::pointer_index_node::accept(basic_ast_visitor *sp, int level) { sp->do_pointer_index_node(this, level); }
void udf::return_node::accept(basic_ast_visitor *sp, int level) { sp->do_return_node(this, level); }
void udf::sizeof_node::accept(basic_ast_visitor *sp, int level) { sp->do_sizeof_node(this, level); }
void udf::variable_declaration_node::accept(basic_ast_visitor *sp, int level) { sp->do_variable_declaration_node(this, level); }
void udf::write_node::accept(basic_ast_visitor *sp, int level) { sp->do_write_node(this, level); }
void udf::writeln_node::accept(basic_ast_visitor *sp, int level) { sp->do_writeln_node(this, level); }
void udf::tensor_capacity_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_capacity_node(this, level); }
void udf::tensor_contraction_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_contraction_node(this, level); }
void udf::tensor_dim_function_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_dim_function_node(this, level); }
void udf::tensor_dims_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_dims_node(this, level); }
void udf::tensor_index_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_index_node(this, level); }
void udf::tensor_rank_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_rank_node(this, level); }
void udf::tensor_reshape_node::accept(basic_ast_visitor *sp, int level) { sp->do_tensor_reshape_node(this, level); }
*/