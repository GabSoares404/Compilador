# Compilador G-V1 para MIPS Assembly 🚀

Este projeto é um tradutor completo e funcional feito do zero. Ele pega arquivos escritos na linguagem didática **G-V1** (uma linguagem com estruturação PASCAL e comandos lógicos em português) e os converte magicamente para **MIPS Assembly**, a linguagem de máquina eletromecânica clássica.

---

## 🛠️ O Que Eu Preciso Para Rodar?

Este projeto foi construído para ecossistemas **Linux**, então se você está usando Windows, abra o seu terminal do **WSL** (Ubuntu).

Você precisará garantir que seu Linux possui as seguintes ferramentas base. Cole o comando abaixo no seu terminal para baixar e instalar todas elas:

```bash
sudo apt-get update
sudo apt-get install gcc g++ flex bison make 
```

**O que exatamente acabamos de baixar?**
- `gcc / g++`: O Cérebro base do Linux para compilar projetos em C.
- `flex` e `bison`: Construtores de tradutores universais.
- `make`: O pedreiro que vai ler as plantas do nosso projeto e construir nosso arquivo sozinho.

---

## 📂 Arquitetura do Compilador (Módulos)
O pipeline de compilação segue o modelo prático de tradução sequencial. Cada diretório encapsula uma etapa do processo, sendo que a Árvore de Sintaxe Abstrata (AST) e a Tabela de Símbolos atuam como as estruturas de dados centrais unificando as interações:

* 🔤 **`Lexer/` (Análise Léxica)**
  * **O que é:** a etapa responsável por ler o código-fonte caractere por caractere e transformá-lo em unidades menores de significado, chamadas de tokens.
  * **Para que serve:** o Lexer limpa a entrada inicial do programa, removendo espaços em branco, quebras de linha e comentários, já que esses elementos não mudam o significado da linguagem. Ele também agrupa sequências de caracteres em blocos lógicos, como palavras reservadas, identificadores, números e símbolos.
  
    Por exemplo, ele reconhece que as letras `i-n-t` formam uma palavra reservada de tipo, e que os caracteres `1-5` juntos representam um único valor numérico. Isso permite que o restante do compilador trabalhe com partes já organizadas do código, em vez de lidar com texto bruto.
  
  * **Como funciona:** o analisador léxico é implementado com **Flex** e usa **expressões regulares** para identificar padrões válidos no arquivo de entrada. A leitura segue regras bem definidas, comparando o texto do programa com os padrões esperados pela linguagem.
  
    Sempre que reconhece uma sequência válida, ele gera um **token** e o envia para a função `yylex()`. Junto com esse token, também é armazenado o texto exato encontrado no código original, disponível na variável global `yytext`.
  
    Assim, o Lexer atua como a primeira etapa real de interpretação do código, convertendo a entrada textual em elementos estruturados que serão consumidos pelo Parser nas próximas fases.

* 🌳 **`Parser/` (Análise Sintática)**
  * **O que é:** a etapa responsável por analisar a estrutura do código e verificar se ele segue a gramática da linguagem. Ela recebe os tokens produzidos pelo analisador léxico e organiza tudo em uma estrutura válida para o compilador.

  * **Para que serve:** o Parser confere se declarações, comandos e expressões estão na ordem correta e obedecem às regras formais da linguagem. Quando encontra uma construção fora do padrão esperado, ele sinaliza um erro sintático. É nessa etapa que o compilador detecta, por exemplo, comandos colocados em posições inválidas ou expressões usadas em contextos que aceitam apenas declarações ou blocos específicos.

  * **Como funciona:** o Parser é implementado com **Bison** e trabalha sobre a sequência de tokens enviada pela análise léxica. A partir da gramática livre de contexto definida para a linguagem, ele reconhece as estruturas válidas do programa e decide como cada trecho deve ser interpretado.

    O processo segue o modelo **LALR(1)**, usando a estratégia **shift-reduce**. Na prática, isso significa que o parser vai lendo os tokens, empilhando e reduzindo partes da entrada até conseguir reconhecer construções completas da linguagem. Sempre que uma regra da gramática é satisfeita, ele executa as ações semânticas escritas em C, que normalmente criam nós da **AST** e conectam essas estruturas entre si.

    Dessa forma, o Parser não apenas valida a sintaxe, mas também já participa da construção da árvore sintática abstrata, que será usada nas etapas seguintes do compilador.

* 🌿 **`AST/` (Representação Intermediária)**
  * **O que é:** A Árvore de Sintaxe Abstrata, ou AST, é uma representação intermediária do código-fonte. Ela organiza a estrutura do programa de forma hierárquica, mostrando como as partes do código se relacionam entre si.
  * **Para que serve:** A AST ajuda o compilador nas etapas seguintes, principalmente na análise semântica e na geração de código. Em vez de precisar reler o texto bruto ou refazer a interpretação dos tokens, o compilador passa a trabalhar diretamente sobre uma estrutura já montada e mais fácil de processar. Além disso, ela preserva a hierarquia correta das expressões, respeitando a precedência dos operadores e o agrupamento natural das construções da linguagem.
  * **Como funciona:** a AST é implementada em C com o uso de `structs` e ponteiros, formando uma estrutura em árvore com nós ligados entre si. Durante a construção pelo parser, cada nó representa uma parte do programa, como uma expressão, um comando ou uma declaração. Esses nós podem ter conexões à esquerda e à direita, permitindo representar corretamente a organização lógica do código.
  
    Outro ponto importante é que, no momento em que cada nó é criado, ele registra a linha do código original em que apareceu, por meio da variável global `yylineno`, vinda do lexer. Isso torna a AST mais útil para depuração, porque permite localizar com precisão onde cada trecho do programa foi lido.
  
    Dessa forma, a AST funciona como uma base sólida para o restante do compilador, já que concentra a estrutura essencial do programa em uma forma mais limpa, organizada e fácil de percorrer nas fases posteriores.

* 🗃️ **`SymbolTable/` (Motor de Escopos & Endereçamento)**
  * **O que é:** a Tabela de Símbolos é a estrutura responsável por gerenciar todos os identificadores do programa (variáveis, funções, constantes), junto com suas informações associadas, como tipo, escopo e posição de memória.
  * **Para que serve:** ela garante a consistência do uso dos identificadores ao longo do programa, evitando conflitos como declarações duplicadas no mesmo escopo ou usos inválidos. Também controla corretamente o conceito de escopo, permitindo que variáveis com o mesmo nome coexistam em diferentes contextos sem interferência.
  
    Um ponto central é o tratamento de *shadowing*: variáveis declaradas em escopos internos podem temporariamente sobrescrever identificadores externos (como o clássico `i` em loops), sem comprometer o estado global. Isso permite que blocos internos operem de forma isolada e segura.
  
    Além disso, a tabela de símbolos também atua como base para o mapeamento de memória na geração de código, organizando como cada identificador será representado no nível do MIPS.
  
  * **Como funciona:** a estrutura é organizada como uma pilha (LIFO) de tabelas de símbolos, onde cada nível representa um escopo ativo. Internamente, cada tabela utiliza uma hash table para permitir buscas e inserções eficientes.
  
    Quando o compilador entra em um novo bloco (`{`), uma nova tabela é empilhada, representando um novo escopo. Ao sair do bloco, essa tabela é removida, restaurando o contexto anterior. Isso garante isolamento entre escopos e implementa naturalmente o comportamento de shadowing.
  
    As operações principais, como `lookup()` e inserção, percorrem a pilha de cima para baixo — ou seja, do escopo mais interno para o mais externo — garantindo que a versão mais recente de um identificador seja sempre a considerada válida.
  
    Durante a geração de código para MIPS, a tabela de símbolos também fornece as informações necessárias para o endereçamento das variáveis. Cada identificador é associado a um deslocamento (*offset*) em memória, normalmente relativo ao `frame pointer`. Esses offsets seguem um padrão de alinhamento (geralmente em múltiplos de 4 bytes), permitindo acesso consistente e previsível aos dados durante a execução.

* 🔎 **`Semantic/` (Análise Semântica)**
  * **O que é:** a etapa responsável por validar o significado do programa após a análise sintática. Ela garante que o código não seja apenas bem formado, mas também logicamente coerente dentro das regras da linguagem.
  * **Para que serve:** a análise semântica impede que construções inválidas avancem no processo de compilação. Ela verifica compatibilidade de tipos, uso correto de variáveis e coerência nas operações realizadas.
  
    Atua barrando casos como operações inválidas (por exemplo, tentar aplicar operações matemáticas em tipos não suportados), uso de variáveis não declaradas (“fantasmas”) e atribuições inconsistentes. Dessa forma, evita que um código sintaticamente válido gere comportamento incorreto ou imprevisível nas etapas seguintes.
  
    Também depende diretamente da SymbolTable para garantir que todos os identificadores usados estejam corretamente definidos dentro de seus respectivos escopos.
  
  * **Como funciona:** a análise é feita percorrendo a AST em profundidade (DFS), normalmente de forma recursiva. Durante esse percurso, cada nó da árvore é analisado com base em suas regras semânticas, e informações são consultadas constantemente na SymbolTable.
  
    A verificação acontece de forma bottom-up: primeiro são avaliados os nós mais internos (subexpressões), e então os resultados são propagados para níveis superiores da árvore. Isso permite inferir tipos e validar operações com base nos seus operandos reais.
  
    Caso seja encontrada alguma inconsistência, o sistema reporta o erro utilizando a linha original do código (`yylineno`) associada ao nó problemático. Se um erro crítico for detectado, a compilação é interrompida imediatamente com `exit(1)`, exibindo no terminal uma mensagem clara indicando o ponto exato da falha.
  
    Isso garante que apenas programas semanticamente válidos avancem para a geração de código.

* ⚙️ **`GeradorCodigo/` (Geração de Código MIPS)**
  * **O que é:** a etapa responsável por transformar a representação intermediária do programa em código Assembly MIPS. É nela que as estruturas abstratas do compilador passam a ser traduzidas para instruções reais de baixo nível.

  * **Para que serve:** ela faz a ponte entre a lógica do compilador e a execução na máquina-alvo. Depois que a análise sintática e semântica confirmam que o programa está correto, o gerador converte essa estrutura em instruções MIPS equivalentes, prontas para serem executadas por um simulador como o SPIM.

    Isso inclui traduzir expressões, comandos condicionais, laços, leituras, escritas e demais operações da linguagem para instruções compatíveis com a arquitetura MIPS de 32 bits.

  * **Como funciona:** a geração de código acontece com base na AST e nas informações já validadas pelas etapas anteriores. A partir daí, o compilador percorre a árvore e converte cada construção da linguagem para sua forma em Assembly.

    Expressões são transformadas em operações sobre registradores, usando um registrador acumulador principal, como o `$s0`, para armazenar resultados intermediários. Quando necessário, valores temporários são salvos na pilha, respeitando o uso do `$sp` e mantendo a organização da execução.

    Estruturas de controle, como `if`, `else` e `while`, são traduzidas em instruções de desvio e rótulos (`branch` e `jump`), permitindo representar o fluxo condicional e os laços do programa em MIPS.

    Já nas operações de entrada e saída, o gerador utiliza chamadas de sistema (`syscall`) para interagir com o ambiente. Isso permite imprimir valores, ler dados e realizar outras ações básicas de I/O no simulador.

    Em resumo, essa etapa pega a lógica já validada do programa e a converte em código executável no modelo MIPS, preservando o comportamento original da linguagem de forma fiel e organizada.

---

## 🧱 Parte 1: Fabricando o Compilador

No terminal, apenas navegue até a pasta do projeto que contém este README e digite os comandos:

```bash
make clean
make
```
Se tudo der certo, um arquivo sem extensão nenhuma chamado **`g-v1`** irá surgir na sua pasta. Ele é o próprio compilador.

---

## 💻 Parte 2: Traduzindo o primeiro Código
Para que o compilador funcione, você precisa escrever algo na linguagem inventada dele. 
Crie um arquivo chamado **`teste.g`** e escreva o código nele, como o exemplo abaixo (O G-V1 exige rigorosamente que a criação das variáveis fique numa jaula de chaves `{}` separada dos comandos):

```pascal
principal 
{
    a, b : int;
}
{
    escreva "Ola! Qual o valor Mágico de A? ";
    leia a;

    escreva "Digite o Somador B: ";
    leia b;

    escreva "O Resultado brutal da soma e: ";
    escreva a + b;
    novalinha;
}
```

Agora jogue esse arquivo para o Compilador, usando o terminal:
```bash
./g-v1 teste.g
```

Se as suas chaves tiverem no lugar, a tela vai printar um **SUCESSO** grandão informando que não houve erros de grafia. Agora olhe dentro da sua pasta novamente... O Compilador cuspiu o arquivo **`teste.g.s`**. 

Esse novo arquivo abriga os comandos MIPS `li`, `add` e `syscall`. 

---

## 🎮 Parte 3: Executando a Máquina

Se quiser rodar o código, você pode usar o SPIM, um emulador levíssimo de PlayStation 1/MIPS para vermos o código da máquina funcionar.

```bash
sudo apt-get install spim
spim -file teste.g.s
```
