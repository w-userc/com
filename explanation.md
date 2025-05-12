
---

### 1. `address_of_node.h`
-   **UDF Construct:** Expression `lval?` (Address-of operator)
-   **Manual Reference:** Section 7.2.4 "Expressão de indicação de posição".
    > "O operador sufixo `?` aplica-se a left-values, retornando o endereço (com o tipo ponteiro) correspondente. Exemplo: `a?`"
-   **Reasoning:** This is a UDF-specific unary operator that takes an l-value and yields its memory address. It's distinct from standard arithmetic/logical operations and requires storing the l-value operand. It results in a pointer type.

---

### 2. `block_node.h`
-   **UDF Construct:** Statement block `{ [declarations] [instructions] }`
-   **Manual Reference:**
    -   Section 4 (Grammar): `bloco	→	{ ⟨ declaração ⟩ ⟨ instrução ⟩ }`
    -   Section 6.1 "Blocos":
        > "Cada bloco tem uma zona de declarações de variáveis locais (facultativa), seguida por uma zona com instruções (possivelmente vazia)."
-   **Reasoning:** Blocks are fundamental for defining scopes and grouping declarations and instructions. This node needs to hold two optional sequences: one for local variable declarations and one for instructions.

---

### 3. `break_node.h`
-   **UDF Construct:** Statement `break;`
-   **Manual Reference:** Section 6.4 "Instrução de terminação".
    > "A instrução `break` termina o ciclo mais interior em que a instrução se encontrar..."
-   **Reasoning:** A specific control flow statement indicating an unconditional jump out of the innermost loop. It doesn't store complex data but its presence changes the flow of execution.

---

### 4. `continue_node.h`
-   **UDF Construct:** Statement `continue;`
-   **Manual Reference:** Section 6.5 "Instrução de continuação".
    > "A instrução `continue` reinicia o ciclo mais interior em que a instrução se encontrar..."
-   **Reasoning:** Another specific control flow statement indicating a jump to the next iteration point of the innermost loop.

---

### 5. `for_node.h`
-   **UDF Construct:** Statement `for ( [init]; [condition]; [increment] ) instruction`
-   **Manual Reference:**
    -   Section 4 (Grammar): `instrução-de-iteração	→	for ( [ variáveis | expressões ] ; [ expressões ] ; [ expressões ] ) instrução`
    -   Section 6.3 "Instrução de iteração":
        > "Esta instrução tem comportamento idêntico ao da instrução `for` em C."
-   **Reasoning:** UDF features a C-style `for` loop, which is more complex than a simple `while` loop (present in the `037` base). This node needs to store three optional sequences (for initializers, condition expressions, and increment expressions) and the loop body instruction.

---

### 6. `function_call_node.h`
-   **UDF Construct:** Expression `identifier(arguments)`
-   **Manual Reference:** Section 5.2 "Invocação".
    > "Se existirem argumentos, na invocação da função, o identificador é seguido de uma lista de expressões delimitadas por parênteses curvos."
-   **Reasoning:** Represents the invocation of a function. It must store the function's identifier and a sequence of expression nodes representing the arguments passed to the function.

---

### 7. `function_declaration_node.h`
-   **UDF Construct:** Declaration `[qualifier] type identifier (parameters);` (without a body)
-   **Manual Reference:**
    -   Section 5.1 "Declaração" (regarding functions without a body).
        > "A declaração de uma função sem corpo é utilizada para tipificar um identificador exterior ou para efectuar declarações antecipadas..."
    -   Section 4.5 "Símbolos globais" (for `forward` and `public` qualifiers).
-   **Reasoning:** Represents a function prototype or a forward/external declaration. It's crucial for enabling mutual recursion and linking with external modules. It stores qualifiers (e.g., `public`, `forward`), the return type, the function identifier, and a sequence of parameter declarations. It explicitly lacks a function body.

---

### 8. `function_definition_node.h`
-   **UDF Construct:** Declaration `[qualifier] type identifier (parameters) block` (with a body)
-   **Manual Reference:**
    -   Section 5.1 "Declaração", Section 5.3 "Corpo".
        > "Caso a declaração tenha corpo, define-se uma nova função..."
    -   Section 4 (Grammar): `função	→	[ public | forward ] ( tipo | auto ) identificador ( [ variáveis ] ) [ bloco ]`
-   **Reasoning:** Represents the complete definition of a function, including its implementation. It contains similar information to `function_declaration_node` (qualifiers, return type, identifier, parameters) but critically includes a `block_node` representing the function's body.

---

### 9. `input_node.h`
-   **UDF Construct:** Expression `input`
-   **Manual Reference:** Section 7.1.2 "Leitura".
    > "A operação de leitura de um valor inteiro ou real pode ser efectuado pela expressão indicada pela palavra reservada `input`, que devolve o valor lido..."
-   **Reasoning:** This node represents an expression that evaluates to a value read from the standard input. This differs from the `read_node` in the `037` base, which was a statement assigning to an l-value. The type of the `input` expression (integer or real) is determined by its context.

---

### 10. `memory_reservation_node.h`
-   **UDF Construct:** Expression `objects(integer_expression)`
-   **Manual Reference:** Section 7.2.3 "Reserva de memória".
    > "A expressão reserva de memória, indicada por `objects` (argumento inteiro), devolve o ponteiro que aponta para a zona de memória..."
-   **Reasoning:** Represents a UDF-specific expression for stack-based memory allocation. It takes an integer expression specifying the number of objects and evaluates to a pointer to the allocated memory.

---

### 11. `nullptr_literal_node.h`
-   **UDF Construct:** Expression `nullptr`
-   **Manual Reference:** Section 3.8.4 "Ponteiros".
    > "O único literal admissível para ponteiros é indicado pela palavra reservada `nullptr`..."
-   **Reasoning:** Represents the null pointer literal, a distinct constant value for pointer types.

---

### 12. `pointer_index_node.h`
-   **UDF Construct:** L-value / Expression `pointer_expression [ index_expression ]`
-   **Manual Reference:** Section 7.2.1 "Indexação de ponteiros".
    > "A indexação de ponteiros devolve o valor de uma posição de memória indicada por um ponteiro. Consiste de uma expressão ponteiro seguida do índice entre parênteses rectos. O resultado de uma indexação de ponteiros é um left-value."
-   **Reasoning:** This node represents pointer dereferencing with an offset (array-like access on pointers). It is an l-value, meaning it can appear on the left side of an assignment. It needs to store the base pointer expression and the index expression.

---

### 13. `return_node.h`
-   **UDF Construct:** Statement `return [expression];`
-   **Manual Reference:**
    -   Section 6.6 "Instrução de retorno".
        > "Uma instrução `return` causa a interrupção imediata da função, assim como o retorno dos valores indicados como argumento da instrução."
    -   Grammar: `return [ expressões ] ;`
-   **Reasoning:** Represents the return statement from a function. While the grammar mentions `expressões` (plural), typical function semantics and the description for void functions ("não admite valores") suggest zero or one return value. The node is designed for an optional single expression.

---

### 14. `sizeof_node.h`
-   **UDF Construct:** Expression `sizeof(expression)`
-   **Manual Reference:** Section 7.2.5 "Expressão de dimensão".
    > "O operador `sizeof` aplica-se a expressões, retornando a dimensão correspondente em bytes. Aplicado a um tensor, retorna a capacidade do tensor (em bytes)."
-   **Reasoning:** An operator that evaluates to the size (in bytes) of its operand expression's type (or a tensor's capacity). It needs to store the operand expression.

---

### 15. `variable_declaration_node.h`
-   **UDF Construct:** Declaration `[qualifier] type identifier [ = initializer_expression ];`
-   **Manual Reference:**
    -   Section 4.4 "Declaração de variáveis".
    -   Section 4 (Grammar): `variável	→	[ public | forward ] tipo identificador [ = expressão ]` and `→	[ public ] auto identificador = expressão`
-   **Reasoning:** Represents the declaration of a variable. It must store any qualifiers (`public`, `forward`), the variable's specified type (which can be `auto` for type inference), its identifier, and an optional initializer expression.

---

### 16. `write_node.h`
-   **UDF Construct:** Statement `write expression_sequence;`
-   **Manual Reference:** Section 6.8 "Instruções de impressão".
    > "`write` ... apresenta a expressão sem mudar de linha. Quando existe mais de uma expressão, as várias expressões são apresentadas sem separação."
-   **Reasoning:** UDF's `write` statement for outputting values without a trailing newline. It differs from the `print_node` in the `037` base by accepting a sequence of expressions.

---

### 17. `writeln_node.h`
-   **UDF Construct:** Statement `writeln expression_sequence;`
-   **Manual Reference:** Section 6.8 "Instruções de impressão".
    > "`writeln` ... apresenta a expressão mudando de linha."
-   **Reasoning:** Similar to `write_node` but appends a newline character after printing the sequence of expressions.

---

### 18. `tensor_capacity_node.h`
-   **UDF Construct:** Expression `tensor_expression.capacity`
-   **Manual Reference:** Section 7.3.1 "Capacidade".
    > "Para um tensor t, o comprimento, indicado por `t.capacity` (valor inteiro), corresponde ao número de células do tensor..."
-   **Reasoning:** A UDF-specific expression to get the capacity (total number of elements) of a tensor. It needs to store the tensor expression it operates on.

---

### 19. `tensor_contraction_node.h`
-   **UDF Construct:** Expression `tensor_expression1 ** tensor_expression2`
-   **Manual Reference:** Section 7.3.6 "Contracção".
    > "Dados dois tensores, t1 e t2, a operação de contracção, representada por `t1 ** t2` é possível se..."
-   **Reasoning:** Represents the tensor contraction operation (generalized matrix multiplication). This is a distinct binary operation specific to tensors.

---

### 20. `tensor_dim_function_node.h`
-   **UDF Construct:** Expression `tensor_expression.dim(index_expression)`
-   **Manual Reference:** Section 7.3.3 "Dimensões".
    > "...obter uma única dimensão através da função dim: e.g. `t.dim(0)` (primeira dimensão)."
-   **Reasoning:** Represents a "method-like" call to get a specific dimension's size from a tensor. It needs to store the tensor expression and the integer index expression.

---

### 21. `tensor_dims_node.h`
-   **UDF Construct:** Expression `tensor_expression.dims`
-   **Manual Reference:** Section 7.3.3 "Dimensões".
    > "Para um tensor t, as dimensões, indicadas por `t.dims` (ponteiro para um vector inteiro), correspondem ao vector com as dimensões do tensor."
-   **Reasoning:** An expression to get a pointer to the vector containing all dimensions of a tensor. It needs to store the tensor expression.

---

### 22. `tensor_index_node.h`
-   **UDF Construct:** L-value / Expression `tensor_expression @ ( coordinate_expressions )`
-   **Manual Reference:** Section 7.3.4 "Indexação".
    > "A indexação de tensores devolve o valor de uma posição de um tensor indicada por um conjunto de coordenadas... Consiste de um tensor seguido do operador `@` e as coordenadas a indexar. O resultado ... é um left-value."
-   **Reasoning:** Represents element access within a tensor using a specific UDF syntax (`@`). It is an l-value. It needs to store the tensor expression and a sequence of coordinate expressions.

---

### 23. `tensor_rank_node.h`
-   **UDF Construct:** Expression `tensor_expression.rank`
-   **Manual Reference:** Section 7.3.2 "Número de dimensões".
    > "Para um tensor t, o número de dimensões é indicado por `t.rank` (valor inteiro)."
-   **Reasoning:** An expression to get the rank (number of dimensions) of a tensor. It needs to store the tensor expression.

---

### 24. `tensor_reshape_node.h`
-   **UDF Construct:** Expression `tensor_expression.reshape(literal_dimension_sequence)`
-   **Manual Reference:** Section 7.3.5 "Redimensionamento".
    > "...a operação de redimensionamento `t.reshape(S1, ..., Sk)` (S1 ... Sk são os comprimentos em cada nova dimensão) produz um novo tensor..."
-   **Reasoning:** Represents the tensor reshape operation. It needs to store the tensor expression being reshaped and a sequence of *literal integer* expressions for the new dimensions.

---

This set of AST nodes aims to comprehensively cover the unique syntactic and semantic constructs of the UDF language as described in the manual, providing a solid foundation for subsequent compiler phases.