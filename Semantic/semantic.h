#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../AST/ast.h"
#include "../SymbolTable/symbol_table.h"

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] ARQUITETURA DO ANALISADOR SEMÂNTICO (CABEÇALHO)
 * ============================================================================
 * O QUE É: Arquivo de definição (header) descrevendo as assinaturas do validador 
 * estático (Analisador Semântico) e sua interação cruzada com a Árvore Sintática 
 * Abstrata (AST) e a Pilha da Tabela de Símbolos local.
 * 
 * PARA QUE SERVE: Imediatamente após a AST ser construída topologicamente pelo 
 * Parser (Bison), evoca-se essas funções para vasculhar os nós em busca de erros 
 * de tipo ou coerência computacional restritos à linguagem G-V2: variáveis ocultas, 
 * coerções inválidas (Type Checking), passagem errada de parâmetros de vetor e
 * duplos declaradores no mesmo referencial limitrofe.
 * ============================================================================
 */

/* 
 * ROTINA PRINCIPAL DO MÓDULO (ENTRYPOINT DA VERIFICAÇÃO)
 * Engatilha a caminhada em profundidade recursiva (Tree Traversal) iniciando pelo 
 * nó global superior (NODE_PROGRAMA). Durante o trajeto descendente, avalia a taxionomia
 * de cada instrução submetendo-as ao analisador formal.
 */
void checkSemantics(AST* node, Stack* scopes);

/* 
 * ALGORITMO DE INFERÊNCIA DE TIPOS (TYPE CHECKER)
 * Itera puramente ramos definidos como processadores operacionais (Sinais matemáticos, relatórios ou booleanos).
 * Atestará iterativamente a compatibilidade tipificada entre nós esquerdos e diretos
 * proibindo nativamente misturas falhas prescritas em documentação local (ex: Int com Car).
 */
int inferirTipo(AST* expr, Stack* scopes);

/* 
 * INSERÇÃO TÁTICA E VERIFICADORA DE DUPLICAÇÕES
 * Responsável operante restrito dos arrays lexicais de declarações em cadeia (ex: int a, b, x;).
 * Engastada no escopo ativo isolado (lookupCurrentScope) bloqueando sobreposições declarativas 
 * ilegais antes de popular e registrar ativamente os símbolos locais de estado MIPS.
 */
void extrairVariaveis(AST* nodeIds, Stack* scopes, int tipoHerdado);

#endif
