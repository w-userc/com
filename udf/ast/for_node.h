#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing for-loop nodes.
   * UDF `for` loop: for ( [init] ; [condition] ; [increment] ) instruction
   * init can be variable declarations or expressions. condition/increment are expressions.
   */
  class for_node : public cdk::basic_node {
    cdk::sequence_node *_init;        // Sequence of variable_declarations or expressions
    cdk::sequence_node *_condition;   // Sequence of expressions (usually one non-empty for loop)
    cdk::sequence_node *_increment;   // Sequence of expressions
    cdk::basic_node *_instruction;    // The loop body

  public:
    for_node(int lineno, cdk::sequence_node *init, cdk::sequence_node *condition,
             cdk::sequence_node *increment, cdk::basic_node *instruction) :
        cdk::basic_node(lineno), _init(init), _condition(condition), 
        _increment(increment), _instruction(instruction) {
    }

  public:
    cdk::sequence_node* init() { return _init; }
    cdk::sequence_node* condition() { return _condition; }
    cdk::sequence_node* increment() { return _increment; }
    cdk::basic_node* instruction() { return _instruction; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_for_node(this, level); }
  };

} // udf
