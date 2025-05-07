#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <cdk/types/types.h>
#include <cdk/compiler.h>
#include <cdk/generator.h>
#include <cdk/symbol_table.h>
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
#include "compiler.h"
#include "parser.h"

//---------------------------------------------------------------------------

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <source-file>" << std::endl;
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in) {
    std::cerr << "cannot open file '" << argv[1] << "'" << std::endl;
    return 1;
  }

  // create a new compiler
  std::shared_ptr<udf::compiler> compiler = std::make_shared<udf::compiler>();

  // create a new parser
  std::shared_ptr<udf::parser> parser = std::make_shared<udf::parser>(compiler);

  // parse the input file
  try {
    parser->parse(in);
  } catch (const std::string &problem) {
    std::cerr << problem << std::endl;
    return 1;
  }

  // create a new type checker
  std::shared_ptr<udf::type_checker> checker = std::make_shared<udf::type_checker>(compiler, compiler->symtab());

  // check types
  try {
    compiler->ast()->accept(checker, 0);
  } catch (const std::string &problem) {
    std::cerr << problem << std::endl;
    return 1;
  }

  // create a new XML writer
  std::shared_ptr<udf::xml_writer> writer = std::make_shared<udf::xml_writer>(compiler, compiler->symtab());

  // write XML
  try {
    compiler->ast()->accept(writer, 0);
  } catch (const std::string &problem) {
    std::cerr << problem << std::endl;
    return 1;
  }

  // create a new postfix writer
  std::shared_ptr<udf::postfix_writer> postfix = std::make_shared<udf::postfix_writer>(compiler, compiler->symtab());

  // write postfix
  try {
    compiler->ast()->accept(postfix, 0);
  } catch (const std::string &problem) {
    std::cerr << problem << std::endl;
    return 1;
  }

  return 0;
} 