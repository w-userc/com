import os

# Dictionary mapping filename to its rationale block
# The rationale should be formatted as a C++ block comment.
# Using raw string literals (r"""...""") to handle multi-line strings easily.
# Make sure to escape any internal C-style comment terminators (*/) if they appear in rationale.
# For this specific rationale set, it's unlikely.

rationale_map = {
    "address_of_node.h": r"""/*
 * UDF Manual: Section 7.2.4 "Expressão de indicação de posição".
 * "O operador sufixo `?` aplica-se a left-values, retornando o endereço
 * (com o tipo ponteiro) correspondente. Exemplo: `a?`"
 * Reasoning: This is a specific unary operator that takes an l-value and
 * yields its memory address. It's not a standard arithmetic or logical
 * operation covered by generic CDK nodes. It needs to store the l-value
 * it operates on.
 */
""",
    "block_node.h": r"""/*
 * UDF Manual: Section 4 (Grammar: bloco	→	{ ⟨ declaração ⟩ ⟨ instrução ⟩ }),
 * Section 6.1 "Blocos". "Cada bloco tem uma zona de declarações de variáveis
 * locais (facultativa), seguida por uma zona com instruções (possivelmente vazia)."
 * Reasoning: Blocks define scopes and group declarations and instructions.
 * This node needs to hold two sequences: one for declarations and one for
 * instructions.
 */
""",
    "break_node.h": r"""/*
 * UDF Manual: Section 6.4 "Instrução de terminação".
 * "A instrução `break` termina o ciclo mais interior..."
 * Reasoning: A specific control flow statement. It doesn't need to store
 * much data itself, but its presence signifies a jump.
 */
""",
    "continue_node.h": r"""/*
 * UDF Manual: Section 6.5 "Instrução de continuação".
 * "A instrução `continue` reinicia o ciclo mais interior..."
 * Reasoning: Another specific control flow statement.
 */
""",
    "for_node.h": r"""/*
 * UDF Manual: Section 4 (Grammar: instrução-de-iteração	→	for ( [ variáveis | expressões ] ; [ expressões ] ; [ expressões ] ) instrução),
 * Section 6.3 "Instrução de iteração". "Esta instrução tem comportamento idêntico
 * ao da instrução `for` em C."
 * Reasoning: UDF has a C-style for loop, which is distinct from the while
 * loop in the 037 base. It needs to store three optional sequences
 * (initializers, condition, increment) and the body instruction.
 */
""",
    "function_call_node.h": r"""/*
 * UDF Manual: Section 5.2 "Invocação".
 * "Se existirem argumentos, na invocação da função, o identificador é seguido
 * de uma lista de expressões delimitadas por parênteses curvos."
 * Reasoning: Represents calling a function. It needs to store the function's
 * identifier (or an expression that evaluates to a function pointer, though
 * the manual primarily shows identifier calls) and a sequence of argument
 * expressions.
 */
""",
    "function_declaration_node.h": r"""/*
 * UDF Manual: Section 5.1 "Declaração" (for forward/external without body),
 * Section 4.5 "Símbolos globais" (`forward`). "A declaração de uma função sem
 * corpo é utilizada para tipificar um identificador exterior ou para efectuar
 * declarações antecipadas..."
 * Reasoning: Represents a function declaration (prototype), often used for
 * forward declarations or external functions. It stores qualifiers (`forward`,
 * `public`), return type, identifier, and a sequence of parameter declarations.
 * It does not have a body.
 */
""",
    "function_definition_node.h": r"""/*
 * UDF Manual: Section 5.1 "Declaração" (with body), Section 5.3 "Corpo",
 * Section 4 (Grammar: função	→	[ public | forward ] ( tipo | auto ) identificador ( [ variáveis ] ) [ bloco ]).
 * "Caso a declaração tenha corpo, define-se uma nova função..."
 * Reasoning: Represents the full definition of a function. It's similar to
 * function_declaration_node but crucially includes a block_node for the
 * function's body.
 */
""",
    "input_node.h": r"""/*
 * UDF Manual: Section 7.1.2 "Leitura".
 * "A operação de leitura de um valor inteiro ou real pode ser efectuado pela
 * expressão indicada pela palavra reservada `input`, que devolve o valor lido..."
 * Reasoning: This is an expression that reads a value. The read_node from 037
 * was a statement (`tREAD lval ;`). UDF's `input` is an expression. Its type is
 * determined by context.
 */
""",
    "memory_reservation_node.h": r"""/*
 * UDF Manual: Section 7.2.3 "Reserva de memória".
 * "A expressão reserva de memória, indicada por `objects` (argumento inteiro),
 * devolve o ponteiro que aponta para a zona de memória..."
 * Reasoning: A specific expression for dynamic memory allocation on the stack.
 * It takes an integer expression for the count and returns a pointer.
 */
""",
    "nullptr_literal_node.h": r"""/*
 * UDF Manual: Section 3.8.4 "Ponteiros".
 * "O único literal admissível para ponteiros é indicado pela palavra reservada `nullptr`..."
 * Reasoning: Represents the specific null pointer literal.
 */
""",
    "pointer_index_node.h": r"""/*
 * UDF Manual: Section 7.2.1 "Indexação de ponteiros".
 * "A indexação de ponteiros devolve o valor de uma posição de memória indicada
 * por um ponteiro. Consiste de uma expressão ponteiro seguida do índice entre
 * parênteses rectos. O resultado de uma indexação de ponteiros é um left-value."
 * Reasoning: This is pointer dereferencing with an offset. It's an l-value.
 * It needs to store the base pointer expression and the index expression.
 */
""",
    "return_node.h": r"""/*
 * UDF Manual: Section 6.6 "Instrução de retorno".
 * "Uma instrução `return` causa a interrupção imediata da função, assim como o
 * retorno dos valores indicados como argumento da instrução."
 * Grammar: `return [ expressões ] ;`.
 * Reasoning: The return statement. The grammar allows `expressões` (plural),
 * but typically functions return zero or one value. The node is designed for
 * an optional single expression, aligning with common practice and the manual's
 * description for void functions ("não admite valores").
 */
""",
    "sizeof_node.h": r"""/*
 * UDF Manual: Section 7.2.5 "Expressão de dimensão".
 * "O operador `sizeof` aplica-se a expressões, retornando a dimensão
 * correspondente em bytes."
 * Reasoning: An operator that returns the size of an expression's type
 * (or a tensor's capacity). It needs to store the expression it operates on.
 */
""",
    "variable_declaration_node.h": r"""/*
 * UDF Manual: Section 4.4 "Declaração de variáveis",
 * Section 4 (Grammar: variável	→	[ public | forward ] tipo identificador [ = expressão ]
 * and `[ public ] auto identificador = expressão`).
 * Reasoning: Represents the declaration of a variable. It needs to store
 * qualifiers (`public`, `forward`), the variable's type (which can be `auto`),
 * its identifier, and an optional initializer expression.
 */
""",
    "write_node.h": r"""/*
 * UDF Manual: Section 6.8 "Instruções de impressão".
 * "`write` ... apresenta a expressão sem mudar de linha. Quando existe mais de
 * uma expressão, as várias expressões são apresentadas sem separação."
 * Reasoning: UDF's `write` is different from 037's `print` (which took one
 * argument). It takes a sequence of expressions.
 */
""",
    "writeln_node.h": r"""/*
 * UDF Manual: Section 6.8 "Instruções de impressão".
 * "`writeln` ... apresenta a expressão mudando de linha."
 * Reasoning: Similar to `write_node` but adds a newline. Takes a sequence of
 * expressions.
 */
""",
    "tensor_capacity_node.h": r"""/*
 * UDF Manual: Section 7.3.1 "Capacidade".
 * "Para um tensor t, o comprimento, indicado por `t.capacity`..."
 * Reasoning: A specific member access-like expression for tensors. Needs the
 * tensor expression.
 */
""",
    "tensor_contraction_node.h": r"""/*
 * UDF Manual: Section 7.3.6 "Contracção".
 * "Dados dois tensores, t1 e t2, a operação de contracção, representada por `t1 ** t2`..."
 * Reasoning: A specific binary operation for tensors, distinct from regular
 * multiplication.
 */
""",
    "tensor_dim_function_node.h": r"""/*
 * UDF Manual: Section 7.3.3 "Dimensões".
 * "...obter uma única dimensão através da função dim: e.g. `t.dim(0)`..."
 * Reasoning: Represents calling the `dim` "method" on a tensor. Needs the
 * tensor expression and the index expression.
 */
""",
    "tensor_dims_node.h": r"""/*
 * UDF Manual: Section 7.3.3 "Dimensões".
 * "Para um tensor t, as dimensões, indicadas por `t.dims`..."
 * Reasoning: Accessing the `dims` "property" of a tensor. Needs the tensor
 * expression.
 */
""",
    "tensor_index_node.h": r"""/*
 * UDF Manual: Section 7.3.4 "Indexação".
 * "A indexação de tensores devolve o valor de uma posição de um tensor...
 * Consiste de um tensor seguido do operador `@` e as coordenadas a indexar.
 * O resultado ... é um left-value."
 * Reasoning: Tensor element access. It's an l-value. Needs the tensor
 * expression and a sequence of coordinate expressions.
 */
""",
    "tensor_rank_node.h": r"""/*
 * UDF Manual: Section 7.3.2 "Número de dimensões".
 * "Para um tensor t, o número de dimensões é indicado por `t.rank`..."
 * Reasoning: Accessing the `rank` "property" of a tensor.
 */
""",
    "tensor_reshape_node.h": r"""/*
 * UDF Manual: Section 7.3.5 "Redimensionamento".
 * "...a operação de redimensionamento `t.reshape(S1, ..., Sk)`..."
 * Reasoning: Represents the `reshape` operation. Needs the tensor expression
 * and a sequence of new dimension (literal integer) expressions.
 */
"""
}

def prepend_to_file(filepath, text_to_prepend):
    """Prepends the given text to the specified file."""
    try:
        with open(filepath, 'r+', encoding='utf-8') as f:
            content = f.read()
            # Check if the rationale is already there (simple check for the start)
            if content.startswith(text_to_prepend.strip()):
                print(f"Rationale already present in {filepath}. Skipping.")
                return
            f.seek(0, 0)
            f.write(text_to_prepend + content)
        print(f"Prepended rationale to {filepath}")
    except FileNotFoundError:
        print(f"Error: File not found - {filepath}")
    except Exception as e:
        print(f"An error occurred with file {filepath}: {e}")

def process_directory(directory_path):
    """Processes all .h files in the given directory."""
    if not os.path.isdir(directory_path):
        print(f"Error: Directory not found - {directory_path}")
        return

    for filename in os.listdir(directory_path):
        if filename.endswith(".h") and filename in rationale_map:
            filepath = os.path.join(directory_path, filename)
            rationale_text = rationale_map[filename]
            prepend_to_file(filepath, rationale_text)
        elif filename.endswith(".h") and filename not in rationale_map:
            print(f"Warning: No rationale found for {filename}. Skipping.")

if __name__ == "__main__":
    # --- IMPORTANT: SET THIS TO YOUR AST NODE DIRECTORY ---
    ast_nodes_directory = "./udf/ast/"  # Example path, adjust as needed
    # --- ---

    if not os.path.exists(ast_nodes_directory) or not os.path.isdir(ast_nodes_directory):
        print(f"Please ensure the directory '{ast_nodes_directory}' exists and contains your AST node .h files.")
        print("You might need to create it and place the generated .h files there first.")
    else:
        process_directory(ast_nodes_directory)
        print("\nScript finished.")