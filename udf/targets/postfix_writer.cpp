#include <string>
#include <sstream>
#include <algorithm> // for std::remove, std::find
#include "targets/postfix_writer.h"
#include "targets/type_checker.h" // For ASSERT_SAFE_EXPRESSIONS
#include ".auto/all_nodes.h"
#include <cdk/types/types.h>
#include <cdk/types/primitive_type.h>
#include <cdk/types/pointer_type.h>
#include <cdk/types/reference_type.h>
#include <cdk/types/functional_type.h>
#include <cdk/compiler.h> // For _compiler->debug() if needed
#include <cdk/symbol_table.h> // For symbol table operations

// To keep track of function arguments for cleanup (Cdecl)
static int _current_function_arg_bytes = 0;


namespace udf {

// Implementations from your provided postfix_writer.cpp for CDK nodes
void postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) { /* EMPTY */ }
void postfix_writer::do_data_node(cdk::data_node * const node, int lvl) { /* EMPTY */ }

void postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if(node->node(i)) node->node(i)->accept(this, lvl); // lvl for symtab, not pf
  }
}

void postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) { _pf.INT(node->value()); }
void postfix_writer::do_double_node(cdk::double_node * const node, int lvl) { // UDF 'real'
    // From my previous, more complete version
    int lbl = newlbl_int();
    _pf.RODATA();
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl)); // Use the string label here
    _pf.DOUBLE(node->value());
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl));
    _pf.DLOAD();
}
void postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1 = newlbl_int(); // Get int id
  _pf.RODATA(); _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1)); // Use string label
  _pf.SSTRING(node->value()); // CDK's SSTRING might be different from manual _pf.STR
  _pf.TEXT(); _pf.ADDR(mklbl(lbl1));
}

void postfix_writer::do_not_node(cdk::not_node * const node, int lvl) { // UDF '~'
  ASSERT_SAFE_EXPRESSIONS; node->argument()->accept(this, lvl); _pf.NOT();
}
void postfix_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->argument()->accept(this, lvl);
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DNEG(); else _pf.NEG();
}
void postfix_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->argument()->accept(this, lvl); // No operation needed
}

// Binary operations: Add, Sub, Mul, Div, Mod, Lt, Le, Ge, Gt, Ne, Eq, And, Or
// Merging my more type-aware versions with your simpler ones.
void postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DADD(); else _pf.ADD();
}
void postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DSUB(); else _pf.SUB();
}
void postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DMUL(); else _pf.MUL();
}
void postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DDIV(); else _pf.DIV();
}
void postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) { // Modulo only for int
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl); _pf.MOD();
}
// Comparisons - UDF manual implies C semantics, so result is int (0 or 1)
// Need to handle float comparisons correctly (DCMP and then set int based on flags)
void postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) { _pf.DCMP(); _pf.INT(0); /* Placeholder for getting bool from flags */}
  _pf.LT();
}
void postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) { _pf.DCMP(); _pf.INT(0); }
  _pf.LE();
}
void postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) { _pf.DCMP(); _pf.INT(0); }
  _pf.GE();
}
void postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) { _pf.DCMP(); _pf.INT(0); }
  _pf.GT();
}
// Equality can be more complex for floats, and needs to handle pointers too.
void postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) { _pf.DCMP(); _pf.INT(0); }
  _pf.NE();
}
void postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->left()->accept(this, lvl); node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) { _pf.DCMP(); _pf.INT(0); }
  _pf.EQ();
}
// Logical operators with short-circuiting
void postfix_writer::do_and_node(cdk::and_node * const node, int lvl) { // UDF '&&'
  ASSERT_SAFE_EXPRESSIONS; int lbl_false = newlbl_int();
  node->left()->accept(this, lvl); _pf.DUP32(); _pf.JZ(mklbl(lbl_false)); // If left is false, result is 0
  _pf.TRASH(4); node->right()->accept(this, lvl); // Evaluate right, result is right's truthiness
  define_lbl(lbl_false);
}
void postfix_writer::do_or_node(cdk::or_node * const node, int lvl) { // UDF '||'
  ASSERT_SAFE_EXPRESSIONS; int lbl_true = newlbl_int();
  node->left()->accept(this, lvl); _pf.DUP32(); _pf.JNZ(mklbl(lbl_true)); // If left is true, result is 1 (or left's value)
  _pf.TRASH(4); node->right()->accept(this, lvl);
  define_lbl(lbl_true);
}

void postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // This needs to use symbol table to find offset for locals or label for globals.
  // Your version: _pf.ADDR(node->name()); assumes global.
  const auto symbol = _symtab.find(node->name());
  if (symbol) {
      if (symbol->offset() == 0) { // Global or unknown offset (needs proper setup)
          _pf.ADDR(node->name());
      } else { // Local
          _pf.LOCAL(symbol->offset());
      }
  } else { /* Should not happen if type checker ran */ _pf.ADDR(node->name()); } // Fallback
}
void postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->lvalue()->accept(this, lvl); // Address on stack
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DLOAD();
  else if (node->type() && node->type()->name() == cdk::TYPE_INT) _pf.LOAD(); // Your _pf.LDINT()
  else if (node->type() && node->type()->name() == cdk::TYPE_STRING) _pf.LOAD(); // Load address of string
  else if (node->type() && node->type()->name() == cdk::TYPE_POINTER) _pf.LOAD(); // Load pointer value
  else { /* Error or generic load based on size */ _pf.LOAD(); } // Fallback to int-like load
}
void postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // Value on stack
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DDUP(); else _pf.DUP32(); // Keep value for expression result
  node->lvalue()->accept(this, lvl);   // Address on stack
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) _pf.DSTORE();
  else _pf.STORE(); // Your _pf.STINT()
}

// 037-UDF Specific Nodes
void postfix_writer::do_program_node(udf::program_node * const node, int lvl) {
    // Global initializations, externs, etc.
    _pf.TEXT(); _pf.ALIGN();

    // RTS mandates that its name be "udf" for the main user function.
    // The old code generated "_main". UDF manual says "udf".
    // We need to find the udf::function_definition_node for "udf" and generate its code.
    // For now, this structure is a bit mixed with the old main.
    // Let's assume global declarations are processed first.
    if (node->declarations()) {
        for (size_t i = 0; i < node->declarations()->size(); ++i) {
            auto decl = node->declarations()->node(i);
            if (decl) { // Global var decls or function decls/defs
                // Function definitions will generate their own labels and code.
                // Global var decls might generate data segments.
                decl->accept(this, lvl);
            }
        }
    }

    // The actual call to the 'udf' function (the user's main) would be part of
    // the RTS startup or a small C main that calls it.
    // For this deliverable, ensuring function definitions are processed is key.
    // The old _main structure is for a simpler language.

    // Externs from your file (these are RTS functions UDF will use)
    _pf.EXTERN("readi");  _pf.EXTERN("printi");
    _pf.EXTERN("reads");  _pf.EXTERN("prints"); // UDF uses strings
    _pf.EXTERN("readr");  _pf.EXTERN("printr"); // UDF uses reals
    _pf.EXTERN("println");
    _pf.EXTERN("argc"); _pf.EXTERN("argv"); _pf.EXTERN("envp"); // For UDF main args
    _pf.EXTERN("atoi"); // Example utility
    // ... any other RTS functions for tensors, memory, etc.
}

void postfix_writer::do_evaluation_node(udf::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->argument()->accept(this, lvl);
  if (node->argument()->type() && node->argument()->type()->name() != cdk::TYPE_VOID) {
    _pf.TRASH(node->argument()->type()->size());
  }
}

// print_node, read_node, while_node from 037.
// The new UDF nodes (write_node, input_node, for_node) should be used.
// I'll keep these for now in case the old parser parts are still active.
void postfix_writer::do_print_node(udf::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT)) { _pf.CALL("printi"); _pf.TRASH(4); }
  else if (node->argument()->is_typed(cdk::TYPE_STRING)) { _pf.CALL("prints"); _pf.TRASH(4); }
  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE)) { _pf.CALL("printr"); _pf.TRASH(8); }
  else { /* error */ }
  // _pf.CALL("println"); // 037 print_node implies newline
}
void postfix_writer::do_read_node(udf::read_node * const node, int lvl) { // Replaced by input_node
  ASSERT_SAFE_EXPRESSIONS;
  // This was for `tREAD lval`. `input` is an expression.
  // Assuming lval is int for simplicity of old code.
  _pf.CALL("readi"); _pf.PUSH(); // PUSH result from register if readi returns in register
  node->argument()->accept(this, lvl); // lvalue (address)
  _pf.STORE();
}
void postfix_writer::do_while_node(udf::while_node * const node, int lvl) { // Replaced by for_node
  ASSERT_SAFE_EXPRESSIONS;
  int cond_lbl = newlbl_int(), end_lbl = newlbl_int();
  _for_cond_labels.push_back(mklbl(cond_lbl)); // For continue (simplistic)
  _for_end_labels.push_back(mklbl(end_lbl));   // For break

  define_lbl(cond_lbl);
  node->condition()->accept(this, lvl); _pf.JZ(mklbl(end_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(cond_lbl));
  define_lbl(end_lbl);

  _for_cond_labels.pop_back(); _for_end_labels.pop_back();
}
void postfix_writer::do_if_node(udf::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; int lbl_end = newlbl_int();
  node->condition()->accept(this, lvl); _pf.JZ(mklbl(lbl_end));
  node->block()->accept(this, lvl + 2);
  define_lbl(lbl_end);
}
void postfix_writer::do_if_else_node(udf::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; int lbl_else = newlbl_int(), lbl_end = newlbl_int();
  node->condition()->accept(this, lvl); _pf.JZ(mklbl(lbl_else));
  node->thenblock()->accept(this, lvl + 2); _pf.JMP(mklbl(lbl_end));
  define_lbl(lbl_else);
  node->elseblock()->accept(this, lvl + 2);
  define_lbl(lbl_end);
}

// SKELETONS for New UDF Nodes
void postfix_writer::do_address_of_node(udf::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // The lvalue child's accept should leave its address on the stack.
  node->lvalue()->accept(this, lvl);
}
void postfix_writer::do_block_node(udf::block_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 1);
  if (node->instructions()) node->instructions()->accept(this, lvl + 1);
  // Local variable cleanup (adjust SP) would happen here if _symtab managed offsets correctly for ENTER.
  _symtab.pop();
}
void postfix_writer::do_break_node(udf::break_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (!_for_end_labels.empty()) _pf.JMP(_for_end_labels.back());
  else { /* error: break outside loop */ }
}
void postfix_writer::do_continue_node(udf::continue_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (!_for_cond_labels.empty()) _pf.JMP(_for_cond_labels.back());
  else { /* error: continue outside loop */ }
}
void postfix_writer::do_for_node(udf::for_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int init_cond_lbl = newlbl_int(); // Label for start of condition or after init
  int incr_lbl = newlbl_int();      // Label for increment part
  int body_lbl = newlbl_int();      // Label for body start (after condition true)
  int end_lbl = newlbl_int();       // Label for end of loop

  _for_cond_labels.push_back(mklbl(incr_lbl)); // Continue jumps to increment
  _for_end_labels.push_back(mklbl(end_lbl));   // Break jumps to end
  _symtab.push(); // Scope for init vars

  if (node->init()) node->init()->accept(this, lvl);

  define_lbl(init_cond_lbl); // This is effectively the start of condition check after init
  if (node->condition() && node->condition()->size() > 0) {
    node->condition()->accept(this, lvl); // Assume single expression for now
    _pf.JZ(mklbl(end_lbl));
  } // If no condition, it's effectively true, JMP to body or fall through

  // define_lbl(body_lbl); // Optional explicit label for body
  node->instruction()->accept(this, lvl);

  define_lbl(incr_lbl);
  if (node->increment()) node->increment()->accept(this, lvl);
  _pf.JMP(mklbl(init_cond_lbl)); // Jump back to condition test phase

  define_lbl(end_lbl);
  _symtab.pop();
  _for_cond_labels.pop_back(); _for_end_labels.pop_back();
}

void postfix_writer::do_function_call_node(udf::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // Store current arg bytes, process this call's args, then restore.
  int previous_arg_bytes = _current_function_arg_bytes;
  _current_function_arg_bytes = 0;

  if (node->arguments()) {
    for (int i = node->arguments()->size() - 1; i >= 0; --i) {
      auto arg_expr = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
      if (arg_expr) {
          arg_expr->accept(this, lvl);
          if (arg_expr->type()) _current_function_arg_bytes += arg_expr->type()->size();
      }
    }
  }
  _pf.CALL(node->identifier());
  if (_current_function_arg_bytes > 0) _pf.TRASH(_current_function_arg_bytes);

  // If function has a return value (not void), it's now on stack (or in EAX/ST0 by convention)
  // For expressions, it should be left on stack.
  // The type checker should set the function_call_node's type to the return type.
  if (node->type() && node->type()->name() != cdk::TYPE_VOID) {
      // If return is by register, might need to push it. For now, assume stack.
      if (node->type()->name() == cdk::TYPE_DOUBLE) _pf.DPUSH(); // If returned in ST0
      else _pf.PUSH(); // If returned in EAX
  }
  _current_function_arg_bytes = previous_arg_bytes; // Restore
}

void postfix_writer::do_function_declaration_node(udf::function_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // For 'forward' declarations, ensure the symbol is marked as external for linking.
  // _symtab.find(node->identifier()) should give the symbol.
  // If it's forward and not already extern, _pf.EXTERN(node->identifier());
  // This depends on how symbol table and semantic analysis handle this.
  // For now, no code generation, but semantic phase would make it global/extern.
}

void postfix_writer::do_function_definition_node(udf::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // This is a major piece.
  std::string func_id = node->identifier();
  // if (func_id == "udf") func_id = "_main"; // As per old simple compiler, but UDF manual says 'udf'

  // Store and reset arg bytes for this function scope
  int previous_arg_bytes = _current_function_arg_bytes;
  _current_function_arg_bytes = 0; // For calls *within* this function
  
  // Semantic analysis should have:
  // 1. Created a symbol for the function.
  // 2. Calculated the total size of local variables for _pf.ENTER().
  // 3. Stored parameter symbols with their offsets.
  std::shared_ptr<udf::symbol> func_symbol = _symtab.find(func_id);
  int local_var_space = func_symbol ? func_symbol->offset() : 0; // Offset used for local var space here
  _current_function_end_label = mklbl(newlbl_int());


  _pf.TEXT(); _pf.ALIGN();
  if (node->qualifier() == udf::FunctionQualifier::PUBLIC || func_id == "udf") { // 'udf' is implicitly public
      _pf.GLOBAL(func_id, _pf.FUNC());
  }
  _pf.LABEL(func_id);
  _pf.ENTER(local_var_space); // Space for local variables

  _symtab.push(); // New scope for parameters and locals
  // Parameter processing (offsets are positive relative to BP)
  // Local variable processing (offsets are negative relative to BP)
  // This needs to be driven by symbol table info built during semantic pass.

  if (node->body()) node->body()->accept(this, lvl + 1);

  define_lbl(std::stoi(_current_function_end_label.substr(2))); // Define function end label for returns

  // Default return for void functions or if no return hit (for non-void, this is an error caught by type checker)
  if (!node->type() || node->type()->name() == cdk::TYPE_VOID) {
    // No explicit value to return for void
  } else if (node->type()->name() == cdk::TYPE_INT || node->type()->name() == cdk::TYPE_POINTER || node->type()->name() == cdk::TYPE_STRING) {
     _pf.INT(0); // Default return 0 for non-void if no return path gives value
     _pf.STFVAL32(); // Store in EAX
  } else if (node->type()->name() == cdk::TYPE_DOUBLE) {
     _pf.DINT(0); // Push 0.0
     _pf.DSTORE(); // Store in ST0 (this might need specific STFVAL for double)
  }

  _symtab.pop();
  _pf.LEAVE(); _pf.RET();
  _current_function_arg_bytes = previous_arg_bytes; // Restore
}

void postfix_writer::do_input_node(udf::input_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type() && node->type()->name() == cdk::TYPE_DOUBLE) { _pf.CALL("readr"); _pf.DPUSH(); }
  else if (node->type() && node->type()->name() == cdk::TYPE_STRING) { _pf.CALL("reads"); _pf.PUSH(); } // reads returns address
  else { _pf.CALL("readi"); _pf.PUSH(); } // Default or int
}

void postfix_writer::do_memory_reservation_node(udf::memory_reservation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->count_expression()->accept(this, lvl); // Number of elements on stack
  // Type of 'objects' expression is ptr<T>. T is what needs to be known for element_size.
  // Defaulting to ptr<auto> means element_size is context-dependent or fixed (e.g. 1 byte if truly generic)
  // For now, let's assume objects() implies allocating bytes directly or sizeof(int) as a guess.
  // This needs proper type info from semantic analysis for ptr_type->referenced()->size().
  auto ptr_type = std::dynamic_pointer_cast<cdk::pointer_type>(node->type());
  int element_size = 4; // Default if cannot determine, e.g. for ptr<auto> when context isn't resolved.
  if (ptr_type && ptr_type->referenced() && ptr_type->referenced()->name() != cdk::TYPE_UNSPEC) {
      element_size = ptr_type->referenced()->size();
  }
   _pf.INT(element_size); _pf.MUL(); // Total bytes
  _pf.ALLOC(); // Allocate on stack (space for elements)
  _pf.SP();    // Put pointer to allocated memory (start of block) on stack top
}
void postfix_writer::do_nullptr_literal_node(udf::nullptr_literal_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS; _pf.INT(0);
}
void postfix_writer::do_pointer_index_node(udf::pointer_index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->base_ptr()->accept(this, lvl); // Base address on stack
  node->index()->accept(this, lvl);    // Index on stack

  auto base_type = std::dynamic_pointer_cast<cdk::pointer_type>(node->base_ptr()->type());
  if (!base_type || !base_type->referenced()) throw std::string("Cannot index non-pointer or void pointer without cast.");
  
  _pf.INT(base_type->referenced()->size()); // Size of pointed-to type
  _pf.MUL(); // index * size
  _pf.ADD(); // base_address + (index * size) -> final address on stack
  // RValueNode will perform LOAD using this address. AssignmentNode will use it for STORE.
}
void postfix_writer::do_return_node(udf::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->value()) {
    node->value()->accept(this, lvl); // Value to return is on stack
    // Move from stack to EAX/ST0 according to type (ABI)
    if (node->value()->type()) {
        if (node->value()->is_typed(cdk::TYPE_DOUBLE)) _pf.DSTFVAL(); // Pop from stack to ST0
        else _pf.STFVAL32(); // Pop from stack to EAX (int, string, ptr)
    }
  }
  _pf.JMP(_current_function_end_label); // Jump to common function epilogue
}
void postfix_writer::do_sizeof_node(udf::sizeof_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // Type checker must have set the type of the argument.
  // node->argument()->accept(this, lvl); // Argument itself is not evaluated for value
  if (!node->argument()->type()) throw std::string("sizeof on untyped expression");
  _pf.INT(node->argument()->type()->size());
}
void postfix_writer::do_variable_declaration_node(udf::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // For local variables: semantic analysis should assign an offset.
  // For global variables: define in data segment.
  auto symbol = _symtab.find(node->identifier()); // Should have been inserted by semantic pass or earlier in block.
  if (!symbol) { /* error */ return; }

  if (symbol->is_global()) {
      _pf.DATA(); _pf.ALIGN();
      if (node->qualifier() == udf::DeclarationQualifier::PUBLIC) _pf.GLOBAL(node->identifier(), _pf.OBJ());
      _pf.LABEL(node->identifier());
      if (node->initializer()) { // Global vars must be initialized by literals
          // This part is tricky: initializer expression needs to be constant.
          // For now, assume it's handled or simple.
          // A full constant expression evaluator would be needed here.
          // Or, code for initialization is put in a startup section.
          // Simple: initialize to 0/nullptr for now.
          if (node->type()->name() == cdk::TYPE_DOUBLE) _pf.DOUBLE(0.0);
          else _pf.INT(0); // Or .space node->type()->size()
      } else { // Default initialization
          if (node->type()->name() == cdk::TYPE_DOUBLE) _pf.DOUBLE(0.0);
          else _pf.BYTE(node->type()->size(), 0); // Allocate and zero-fill
      }
      _pf.TEXT();
  } else { // Local variable
      // Space is already allocated by ENTER. If initializer, generate assignment.
      if (node->initializer()) {
          node->initializer()->accept(this, lvl); // Value on stack
          _pf.LOCAL(symbol->offset()); // Address on stack
          if (node->type()->name() == cdk::TYPE_DOUBLE) _pf.DSTORE(); else _pf.STORE();
      }
  }
}
void postfix_writer::do_write_node(udf::write_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->arguments()) {
    for (size_t i = 0; i < node->arguments()->size(); ++i) {
      auto arg_expr = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
      if (arg_expr) {
        arg_expr->accept(this, lvl);
        if (arg_expr->type()) {
          if (arg_expr->is_typed(cdk::TYPE_INT)) { _pf.CALL("printi"); _pf.TRASH(4); }
          else if (arg_expr->is_typed(cdk::TYPE_DOUBLE)) { _pf.CALL("printr"); _pf.TRASH(8); }
          else if (arg_expr->is_typed(cdk::TYPE_STRING)) { _pf.CALL("prints"); _pf.TRASH(4); }
          else { /* Cannot print pointers directly in UDF. Error. */ }
        }
      }
    }
  }
}
void postfix_writer::do_writeln_node(udf::writeln_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  do_write_node(node, lvl); // Print arguments
  _pf.CALL("println");
}

// Tensor Nodes - SKELETONS (empty or minimal stubs for compilation)
void postfix_writer::do_tensor_capacity_node(udf::tensor_capacity_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: call RTS */ }
void postfix_writer::do_tensor_contraction_node(udf::tensor_contraction_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: call RTS */ }
void postfix_writer::do_tensor_dim_function_node(udf::tensor_dim_function_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: call RTS */ }
void postfix_writer::do_tensor_dims_node(udf::tensor_dims_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: call RTS */ }
void postfix_writer::do_tensor_index_node(udf::tensor_index_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: calculate address, use LOAD/STORE */ }
void postfix_writer::do_tensor_rank_node(udf::tensor_rank_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: call RTS */ }
void postfix_writer::do_tensor_reshape_node(udf::tensor_reshape_node *const node, int lvl) { ASSERT_SAFE_EXPRESSIONS; /* TODO: call RTS */ }

} // namespace udf