/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "Parser/parser.y"

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 1 - DEFINIÇÕES C GLOBAIS E IMPORTAÇÕES
 * ============================================================================
 * O QUE É: Este cabeçalho delimitado entre '%{' e '%}' injeta código direto em C 
 * no futuro arquivo 'parser.tab.c' gerado automaticamente pelo Bison.
 * 
 * PARA QUE SERVE: Estabelece as pontes de comunicação vital do projeto. Ele liga 
 * o Motor Matemático Sintático (Bison) às bibliotecas auxiliares, importa os Nodes 
 * da Árvore de Sintaxe (AST) e possibilita a inserção das assinaturas globais 
 * requeridas (como funções Semânticas e o Gerador de Código).
 * 
 * COMO FUNCIONA: Durante a geração `.c`, o Bison transcreve esse bloco ao topo. 
 * As diretivas 'extern' são cruciais: Elas exigem permissão do Sistema Operacional 
 * para acessar a RAM indexada pelo 'Lexer', escancarando acesso às strings brutas lidas 
 * no 'yytext' e aos logs de falha na contagem ativa do 'yylineno'.
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../AST/ast.h"
#include "../Semantic/semantic.h"
#include "../GeradorCodigo/codegen.h"

extern int yylineno; /* Puxa a contagem de linhas do motor do Flex */
extern char* yytext; /* Compartilhamento de string crua lida pelo Flex */
extern int yylex();  /* Assinatura para que o Bison invoque o motor de busca do Flex */
extern FILE *yyin;   /* Ponteiro do buffer de arquivo injetado */

AST* ast_raiz = NULL; /* Instância Matriz (Root) repassada ao Semântico e C-Gen ao fim */

void yyerror(const char *s); /* Assinatura protótipo pra avisos impiedosos de crashes Sintáticos */

#line 106 "Parser/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PRINCIPAL = 3,                  /* PRINCIPAL  */
  YYSYMBOL_INT = 4,                        /* INT  */
  YYSYMBOL_CAR = 5,                        /* CAR  */
  YYSYMBOL_LEIA = 6,                       /* LEIA  */
  YYSYMBOL_ESCREVA = 7,                    /* ESCREVA  */
  YYSYMBOL_NOVALINHA = 8,                  /* NOVALINHA  */
  YYSYMBOL_SE = 9,                         /* SE  */
  YYSYMBOL_ENTAO = 10,                     /* ENTAO  */
  YYSYMBOL_SENAO = 11,                     /* SENAO  */
  YYSYMBOL_FIMSE = 12,                     /* FIMSE  */
  YYSYMBOL_ENQUANTO = 13,                  /* ENQUANTO  */
  YYSYMBOL_OU = 14,                        /* OU  */
  YYSYMBOL_E = 15,                         /* E  */
  YYSYMBOL_IGUAL = 16,                     /* IGUAL  */
  YYSYMBOL_DIFERENTE = 17,                 /* DIFERENTE  */
  YYSYMBOL_MAIORIGUAL = 18,                /* MAIORIGUAL  */
  YYSYMBOL_MENORIGUAL = 19,                /* MENORIGUAL  */
  YYSYMBOL_IDENTIFICADOR = 20,             /* IDENTIFICADOR  */
  YYSYMBOL_INTCONST = 21,                  /* INTCONST  */
  YYSYMBOL_CADEIACARACTERES = 22,          /* CADEIACARACTERES  */
  YYSYMBOL_CARCONST = 23,                  /* CARCONST  */
  YYSYMBOL_24_ = 24,                       /* '{'  */
  YYSYMBOL_25_ = 25,                       /* '}'  */
  YYSYMBOL_26_ = 26,                       /* ':'  */
  YYSYMBOL_27_ = 27,                       /* ';'  */
  YYSYMBOL_28_ = 28,                       /* ','  */
  YYSYMBOL_29_ = 29,                       /* '='  */
  YYSYMBOL_30_ = 30,                       /* '('  */
  YYSYMBOL_31_ = 31,                       /* ')'  */
  YYSYMBOL_32_ = 32,                       /* '<'  */
  YYSYMBOL_33_ = 33,                       /* '>'  */
  YYSYMBOL_34_ = 34,                       /* '+'  */
  YYSYMBOL_35_ = 35,                       /* '-'  */
  YYSYMBOL_36_ = 36,                       /* '*'  */
  YYSYMBOL_37_ = 37,                       /* '/'  */
  YYSYMBOL_38_ = 38,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_Programa = 40,                  /* Programa  */
  YYSYMBOL_DeclPrograma = 41,              /* DeclPrograma  */
  YYSYMBOL_Bloco = 42,                     /* Bloco  */
  YYSYMBOL_VarSection = 43,                /* VarSection  */
  YYSYMBOL_IdComLinha = 44,                /* IdComLinha  */
  YYSYMBOL_ListaDeclVar = 45,              /* ListaDeclVar  */
  YYSYMBOL_DeclVar = 46,                   /* DeclVar  */
  YYSYMBOL_Tipo = 47,                      /* Tipo  */
  YYSYMBOL_ListaComando = 48,              /* ListaComando  */
  YYSYMBOL_Comando = 49,                   /* Comando  */
  YYSYMBOL_Expr = 50,                      /* Expr  */
  YYSYMBOL_OrExpr = 51,                    /* OrExpr  */
  YYSYMBOL_AndExpr = 52,                   /* AndExpr  */
  YYSYMBOL_EqExpr = 53,                    /* EqExpr  */
  YYSYMBOL_DesigExpr = 54,                 /* DesigExpr  */
  YYSYMBOL_AddExpr = 55,                   /* AddExpr  */
  YYSYMBOL_MulExpr = 56,                   /* MulExpr  */
  YYSYMBOL_UnExpr = 57,                    /* UnExpr  */
  YYSYMBOL_PrimExpr = 58                   /* PrimExpr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   132

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  104

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   278


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,     2,     2,     2,
      30,    31,    36,    34,    28,    35,     2,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    26,    27,
      32,    29,    33,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   237,   237,   240,   243,   244,   247,   250,   252,   257,
     263,   264,   270,   271,   278,   279,   282,   286,   287,   288,
     292,   293,   294,   295,   296,   299,   302,   303,   306,   307,
     310,   311,   312,   315,   316,   317,   318,   319,   322,   323,
     324,   327,   328,   329,   332,   333,   334,   335,   338,   339,
     340,   341,   342
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PRINCIPAL", "INT",
  "CAR", "LEIA", "ESCREVA", "NOVALINHA", "SE", "ENTAO", "SENAO", "FIMSE",
  "ENQUANTO", "OU", "E", "IGUAL", "DIFERENTE", "MAIORIGUAL", "MENORIGUAL",
  "IDENTIFICADOR", "INTCONST", "CADEIACARACTERES", "CARCONST", "'{'",
  "'}'", "':'", "';'", "','", "'='", "'('", "')'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'!'", "$accept", "Programa", "DeclPrograma",
  "Bloco", "VarSection", "IdComLinha", "ListaDeclVar", "DeclVar", "Tipo",
  "ListaComando", "Comando", "Expr", "OrExpr", "AndExpr", "EqExpr",
  "DesigExpr", "AddExpr", "MulExpr", "UnExpr", "PrimExpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-20)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       3,   -14,    15,   -20,    59,   -20,    -7,   -20,     4,    76,
      -8,     9,    16,    12,   -20,   -20,    22,    28,     5,   -20,
      81,    37,   -20,   -20,   -20,   -20,    76,    98,    98,    98,
      55,    41,    70,    21,    84,   -13,    25,   -20,   -20,   -20,
      76,    76,    76,    71,    69,   -20,    12,   -20,   -20,    27,
     -20,    73,   -20,   -20,   -20,   -20,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    78,    82,
      80,   -20,    22,    53,   -20,   -20,    70,    21,    84,    84,
     -13,   -13,   -13,   -13,    25,    25,   -20,   -20,    90,    81,
     -20,   -20,   -20,   -20,    85,    81,   -20,    71,    -4,   -20,
      81,   -20,    36,   -20
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     0,     3,     0,     1,     0,     0,
       0,     0,     0,     7,    24,    23,    10,     0,     0,    15,
       0,     0,    48,    49,    51,    50,     0,     0,     0,     0,
       0,    25,    27,    29,    32,    37,    40,    43,    47,    18,
       0,     0,     0,     0,     0,     6,     0,     4,    14,     0,
      16,     0,    44,    45,    46,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,    10,     0,     5,    52,    26,    28,    30,    31,
      35,    36,    33,    34,    38,    39,    41,    42,     0,     0,
      19,    11,    12,    13,     0,     0,    22,     9,     0,     8,
       0,    20,     0,    21
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -20,   -20,   -20,   114,   -20,    83,    30,    57,   -20,   -19,
     -18,    33,   -20,    74,    75,    11,    62,    13,    26,    -1
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,    15,     6,    16,    17,    44,    94,    18,
      19,    30,    31,    32,    33,    34,    35,    36,    37,    38
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      48,    49,     8,     9,    10,    11,     1,   100,   101,    12,
       4,     8,     9,    10,    11,     7,    46,    20,    12,    39,
       4,    64,    65,    14,    21,    46,    52,    53,    54,     4,
      47,    48,    14,     8,     9,    10,    11,    58,    59,    40,
      12,    42,     8,     9,    10,    11,    41,    46,   103,    12,
      43,     4,    74,    45,    14,    56,    46,    92,    93,    51,
       4,    66,    67,    14,    50,     8,     9,    10,    11,    78,
      79,    96,    12,    68,    69,    70,    98,    84,    85,    13,
      48,   102,    55,     4,    48,    57,    14,     8,     9,    10,
      11,    71,    86,    87,    12,    73,    22,    23,    24,    25,
      95,    46,    60,    61,    75,     4,    26,    90,    14,    88,
      27,    28,    97,    89,    29,     5,    62,    63,    22,    23,
      24,    25,    80,    81,    82,    83,    72,    99,    26,    91,
      76,     0,    77
};

static const yytype_int8 yycheck[] =
{
      18,    20,     6,     7,     8,     9,     3,    11,    12,    13,
      24,     6,     7,     8,     9,     0,    20,    24,    13,    27,
      24,    34,    35,    27,    20,    20,    27,    28,    29,    24,
      25,    49,    27,     6,     7,     8,     9,    16,    17,    30,
      13,    29,     6,     7,     8,     9,    30,    20,    12,    13,
      28,    24,    25,    25,    27,    14,    20,     4,     5,    26,
      24,    36,    37,    27,    27,     6,     7,     8,     9,    58,
      59,    89,    13,    40,    41,    42,    95,    64,    65,    20,
      98,   100,    27,    24,   102,    15,    27,     6,     7,     8,
       9,    20,    66,    67,    13,    26,    20,    21,    22,    23,
      10,    20,    18,    19,    31,    24,    30,    27,    27,    31,
      34,    35,    27,    31,    38,     1,    32,    33,    20,    21,
      22,    23,    60,    61,    62,    63,    43,    97,    30,    72,
      56,    -1,    57
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    40,    41,    24,    42,    43,     0,     6,     7,
       8,     9,    13,    20,    27,    42,    44,    45,    48,    49,
      24,    20,    20,    21,    22,    23,    30,    34,    35,    38,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    27,
      30,    30,    29,    28,    46,    25,    20,    25,    49,    48,
      27,    50,    58,    58,    58,    27,    14,    15,    16,    17,
      18,    19,    32,    33,    34,    35,    36,    37,    50,    50,
      50,    20,    44,    26,    25,    31,    52,    53,    54,    54,
      55,    55,    55,    55,    56,    56,    57,    57,    31,    31,
      27,    46,     4,     5,    47,    10,    49,    27,    48,    45,
      11,    12,    48,    12
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    41,    42,    42,    43,    44,    45,    45,
      46,    46,    47,    47,    48,    48,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    50,    51,    51,    52,    52,
      53,    53,    53,    54,    54,    54,    54,    54,    55,    55,
      55,    56,    56,    56,    57,    57,    57,    57,    58,    58,
      58,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     3,     4,     3,     1,     6,     5,
       0,     3,     1,     1,     2,     1,     3,     3,     2,     4,
       7,     9,     5,     1,     1,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Programa: DeclPrograma  */
#line 237 "Parser/parser.y"
                        { ast_raiz = (yyvsp[0].node); }
#line 1489 "Parser/parser.c"
    break;

  case 3: /* DeclPrograma: PRINCIPAL Bloco  */
#line 240 "Parser/parser.y"
                               { (yyval.node) = createNode(NODE_PROGRAMA, "principal", yylineno, (yyvsp[0].node), NULL, NULL); }
#line 1495 "Parser/parser.c"
    break;

  case 4: /* Bloco: '{' ListaComando '}'  */
#line 243 "Parser/parser.y"
                                         { (yyval.node) = createNode(NODE_BLOCO, "bloco", yylineno, NULL, (yyvsp[-1].node), NULL); }
#line 1501 "Parser/parser.c"
    break;

  case 5: /* Bloco: VarSection '{' ListaComando '}'  */
#line 244 "Parser/parser.y"
                                         { (yyval.node) = createNode(NODE_BLOCO, "bloco_var", yylineno, (yyvsp[-3].node), (yyvsp[-1].node), NULL); }
#line 1507 "Parser/parser.c"
    break;

  case 6: /* VarSection: '{' ListaDeclVar '}'  */
#line 247 "Parser/parser.y"
                                  { (yyval.node) = (yyvsp[-1].node); }
#line 1513 "Parser/parser.c"
    break;

  case 7: /* IdComLinha: IDENTIFICADOR  */
#line 250 "Parser/parser.y"
                           { (yyval.node) = createNode(NODE_IDENTIFICADOR, (yyvsp[0].str), yylineno, NULL, NULL, NULL); }
#line 1519 "Parser/parser.c"
    break;

  case 8: /* ListaDeclVar: IdComLinha DeclVar ':' Tipo ';' ListaDeclVar  */
#line 252 "Parser/parser.y"
                                                            { 
                 AST* idNode = (yyvsp[-5].node);
                 AST* vars = createNode(NODE_DECL_VAR, "ids", idNode->linha, idNode, (yyvsp[-4].node), (yyvsp[-2].node));
                 (yyval.node) = createNode(NODE_DECL_VAR, "lista_var", idNode->linha, vars, (yyvsp[0].node), NULL);
             }
#line 1529 "Parser/parser.c"
    break;

  case 9: /* ListaDeclVar: IdComLinha DeclVar ':' Tipo ';'  */
#line 257 "Parser/parser.y"
                                               {
                 AST* idNode = (yyvsp[-4].node);
                 (yyval.node) = createNode(NODE_DECL_VAR, "ids", idNode->linha, idNode, (yyvsp[-3].node), (yyvsp[-1].node));
             }
#line 1538 "Parser/parser.c"
    break;

  case 10: /* DeclVar: %empty  */
#line 263 "Parser/parser.y"
                                                                                     { (yyval.node) = NULL; }
#line 1544 "Parser/parser.c"
    break;

  case 11: /* DeclVar: ',' IdComLinha DeclVar  */
#line 264 "Parser/parser.y"
                                 { 
             AST* idNode = (yyvsp[-1].node);
             (yyval.node) = createNode(NODE_DECL_VAR, "ids", idNode->linha, idNode, (yyvsp[0].node), NULL);
        }
#line 1553 "Parser/parser.c"
    break;

  case 12: /* Tipo: INT  */
#line 270 "Parser/parser.y"
           { (yyval.node) = createNode(NODE_TIPO, "int", yylineno, NULL, NULL, NULL); }
#line 1559 "Parser/parser.c"
    break;

  case 13: /* Tipo: CAR  */
#line 271 "Parser/parser.y"
           { (yyval.node) = createNode(NODE_TIPO, "car", yylineno, NULL, NULL, NULL); }
#line 1565 "Parser/parser.c"
    break;

  case 14: /* ListaComando: ListaComando Comando  */
#line 278 "Parser/parser.y"
                                     { (yyval.node) = createNode(NODE_LISTA_COMANDO, "comandos", yylineno, (yyvsp[-1].node), (yyvsp[0].node), NULL); }
#line 1571 "Parser/parser.c"
    break;

  case 15: /* ListaComando: Comando  */
#line 279 "Parser/parser.y"
                                     { (yyval.node) = (yyvsp[0].node); }
#line 1577 "Parser/parser.c"
    break;

  case 16: /* Comando: LEIA IDENTIFICADOR ';'  */
#line 282 "Parser/parser.y"
                                 { 
            AST* idNode = createNode(NODE_IDENTIFICADOR, (yyvsp[-1].str), yylineno, NULL, NULL, NULL);
            (yyval.node) = createNode(NODE_COMANDO, "leia", yylineno, idNode, NULL, NULL); 
        }
#line 1586 "Parser/parser.c"
    break;

  case 17: /* Comando: ESCREVA Expr ';'  */
#line 286 "Parser/parser.y"
                           { (yyval.node) = createNode(NODE_COMANDO, "escreva", yylineno, (yyvsp[-1].node), NULL, NULL); }
#line 1592 "Parser/parser.c"
    break;

  case 18: /* Comando: NOVALINHA ';'  */
#line 287 "Parser/parser.y"
                        { (yyval.node) = createNode(NODE_NOVALINHA, "novalinha", yylineno, NULL, NULL, NULL); }
#line 1598 "Parser/parser.c"
    break;

  case 19: /* Comando: IDENTIFICADOR '=' Expr ';'  */
#line 288 "Parser/parser.y"
                                     { 
            AST* idNode = createNode(NODE_IDENTIFICADOR, (yyvsp[-3].str), yylineno, NULL, NULL, NULL);
            (yyval.node) = createNode(NODE_COMANDO, "=", yylineno, idNode, (yyvsp[-1].node), NULL); 
        }
#line 1607 "Parser/parser.c"
    break;

  case 20: /* Comando: SE '(' Expr ')' ENTAO ListaComando FIMSE  */
#line 292 "Parser/parser.y"
                                                   { (yyval.node) = createNode(NODE_COMANDO, "se", yylineno, (yyvsp[-4].node), (yyvsp[-1].node), NULL); }
#line 1613 "Parser/parser.c"
    break;

  case 21: /* Comando: SE '(' Expr ')' ENTAO ListaComando SENAO ListaComando FIMSE  */
#line 293 "Parser/parser.y"
                                                                      { (yyval.node) = createNode(NODE_COMANDO, "se_senao", yylineno, (yyvsp[-6].node), (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 1619 "Parser/parser.c"
    break;

  case 22: /* Comando: ENQUANTO '(' Expr ')' Comando  */
#line 294 "Parser/parser.y"
                                        { (yyval.node) = createNode(NODE_COMANDO, "enquanto", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1625 "Parser/parser.c"
    break;

  case 23: /* Comando: Bloco  */
#line 295 "Parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1631 "Parser/parser.c"
    break;

  case 24: /* Comando: ';'  */
#line 296 "Parser/parser.y"
              { (yyval.node) = createNode(NODE_COMANDO, "vazio", yylineno, NULL, NULL, NULL); }
#line 1637 "Parser/parser.c"
    break;

  case 25: /* Expr: OrExpr  */
#line 299 "Parser/parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1643 "Parser/parser.c"
    break;

  case 26: /* OrExpr: OrExpr OU AndExpr  */
#line 302 "Parser/parser.y"
                           { (yyval.node) = createNode(NODE_OP, "||", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1649 "Parser/parser.c"
    break;

  case 27: /* OrExpr: AndExpr  */
#line 303 "Parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1655 "Parser/parser.c"
    break;

  case 28: /* AndExpr: AndExpr E EqExpr  */
#line 306 "Parser/parser.y"
                           { (yyval.node) = createNode(NODE_OP, "&", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1661 "Parser/parser.c"
    break;

  case 29: /* AndExpr: EqExpr  */
#line 307 "Parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1667 "Parser/parser.c"
    break;

  case 30: /* EqExpr: EqExpr IGUAL DesigExpr  */
#line 310 "Parser/parser.y"
                                { (yyval.node) = createNode(NODE_OP, "==", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1673 "Parser/parser.c"
    break;

  case 31: /* EqExpr: EqExpr DIFERENTE DesigExpr  */
#line 311 "Parser/parser.y"
                                    { (yyval.node) = createNode(NODE_OP, "!=", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1679 "Parser/parser.c"
    break;

  case 32: /* EqExpr: DesigExpr  */
#line 312 "Parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1685 "Parser/parser.c"
    break;

  case 33: /* DesigExpr: DesigExpr '<' AddExpr  */
#line 315 "Parser/parser.y"
                                  { (yyval.node) = createNode(NODE_OP, "<", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1691 "Parser/parser.c"
    break;

  case 34: /* DesigExpr: DesigExpr '>' AddExpr  */
#line 316 "Parser/parser.y"
                                  { (yyval.node) = createNode(NODE_OP, ">", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1697 "Parser/parser.c"
    break;

  case 35: /* DesigExpr: DesigExpr MAIORIGUAL AddExpr  */
#line 317 "Parser/parser.y"
                                         { (yyval.node) = createNode(NODE_OP, ">=", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1703 "Parser/parser.c"
    break;

  case 36: /* DesigExpr: DesigExpr MENORIGUAL AddExpr  */
#line 318 "Parser/parser.y"
                                         { (yyval.node) = createNode(NODE_OP, "<=", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1709 "Parser/parser.c"
    break;

  case 37: /* DesigExpr: AddExpr  */
#line 319 "Parser/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1715 "Parser/parser.c"
    break;

  case 38: /* AddExpr: AddExpr '+' MulExpr  */
#line 322 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "+", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1721 "Parser/parser.c"
    break;

  case 39: /* AddExpr: AddExpr '-' MulExpr  */
#line 323 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "-", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1727 "Parser/parser.c"
    break;

  case 40: /* AddExpr: MulExpr  */
#line 324 "Parser/parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1733 "Parser/parser.c"
    break;

  case 41: /* MulExpr: MulExpr '*' UnExpr  */
#line 327 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "*", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1739 "Parser/parser.c"
    break;

  case 42: /* MulExpr: MulExpr '/' UnExpr  */
#line 328 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "/", yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1745 "Parser/parser.c"
    break;

  case 43: /* MulExpr: UnExpr  */
#line 329 "Parser/parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1751 "Parser/parser.c"
    break;

  case 44: /* UnExpr: '+' PrimExpr  */
#line 332 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "+", yylineno, NULL, (yyvsp[0].node), NULL); }
#line 1757 "Parser/parser.c"
    break;

  case 45: /* UnExpr: '-' PrimExpr  */
#line 333 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "-", yylineno, NULL, (yyvsp[0].node), NULL); }
#line 1763 "Parser/parser.c"
    break;

  case 46: /* UnExpr: '!' PrimExpr  */
#line 334 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_OP, "!", yylineno, NULL, (yyvsp[0].node), NULL); }
#line 1769 "Parser/parser.c"
    break;

  case 47: /* UnExpr: PrimExpr  */
#line 335 "Parser/parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1775 "Parser/parser.c"
    break;

  case 48: /* PrimExpr: IDENTIFICADOR  */
#line 338 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_IDENTIFICADOR, (yyvsp[0].str), yylineno, NULL, NULL, NULL); }
#line 1781 "Parser/parser.c"
    break;

  case 49: /* PrimExpr: INTCONST  */
#line 339 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_INTCONST, (yyvsp[0].str), yylineno, NULL, NULL, NULL); }
#line 1787 "Parser/parser.c"
    break;

  case 50: /* PrimExpr: CARCONST  */
#line 340 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_CARCONST, (yyvsp[0].str), yylineno, NULL, NULL, NULL); }
#line 1793 "Parser/parser.c"
    break;

  case 51: /* PrimExpr: CADEIACARACTERES  */
#line 341 "Parser/parser.y"
                              { (yyval.node) = createNode(NODE_CARCONST, (yyvsp[0].str), yylineno, NULL, NULL, NULL); }
#line 1799 "Parser/parser.c"
    break;

  case 52: /* PrimExpr: '(' Expr ')'  */
#line 342 "Parser/parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1805 "Parser/parser.c"
    break;


#line 1809 "Parser/parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 346 "Parser/parser.y"


/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 4 - MÓDULO EXECUTIVO ADICIONAL (MAIN E LOGS)
 * ============================================================================
 * O QUE É: As peças gerenciais em puro C atreladas as execuções de fluxo final do software.
 * 
 * PARA QUE SERVE: Engatilha o ponto de ignição (Startpoint) do Software Compilador e 
 * arquiteta e documenta as falhas detectadas caso elas surjam. Adicionalmente atua 
 * acoplando as três fases inteiras de pós-ast (Tabela Ram -> DFS Sym -> Geração em IO).
 * 
 * COMO FUNCIONA: O projeto começa em 'main()', abrindo o arquivo `.g` via IO do kernel ('fopen').
 * Logo engatilha o loop em 'yyparse()' que consome Lexer com o fluxo Shift-Reduce ativo.
 * Caso yyparse traga sucesso, a Main desperta e inicia o caminhante 'checkSemantics' 
 * repassando à ele a Tabela 'Stack' nova em folha. Com o atestado semântico em 'ok', a compilação
 * funde-se e flui no arquivo elétrico chamando e imprimindo o 'generateCode()'.
 * Se no meio da jornada do 'yyparse()' a gramática falhar irrecuperavelmente, 
 * o gatilho automático interno em 'yyerror' chuta no terminal e estilhaça a execução inteira do processo.
 * ============================================================================
 */
void yyerror(const char *s) {
    if (strstr(s, "expecting '{'")) {
        printf("ERRO: ABRE CHAVES ESPERADO %d\n", yylineno - 1);
    } else if (strstr(s, "unexpected ':', expecting '='")) {
        printf("ERRO: DECLARACAO NAO ENVOLVIDA EM CHAVES %d\n", yylineno);
    } else if (strstr(s, "unexpected SENAO")) {
        printf("ERRO: FECHA CHAVES ESPERADO %d\n", yylineno - 1);
    } else if (strstr(s, "unexpected '}'") && !strstr(s, "expecting")) {
        printf("ERRO: FIMSE FALTANDO %d\n", yylineno - 1);
    } else if (strstr(s, "expecting ';'")) {
        printf("ERRO: PONTO E VIRGULA FALTANDO %d\n", yylineno - 1);
    } else {
        printf("ERRO: %s %d\n", s, yylineno);
    }
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_fonte>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r"); /* O Flex irá varrer este arquivo indicado pelo argv */
    if (!yyin) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    yyparse(); /* Inicia a montagem da Árvore AST em fluxo iterativo! */

    fclose(yyin);
    
    /* Verificamos se o Bison montou a Árvore Completa e entregou a Raiz imaculada */
    if (ast_raiz != NULL) {
        /* Inicializamos estaticamente o motor de Escopos central LIFO */
        Stack tabelaDeSimbolos;
        initStack(&tabelaDeSimbolos);
        
        printf("\n>>> Iniciando O CAMINHANTE SEMANTICO...\n");
        checkSemantics(ast_raiz, &tabelaDeSimbolos);
        
        printf("\n>>> SUCESSO! O G-V1 compilou '%s' e nao encontrou \nnenhum erro Semantico, Sintatico ou Lexico na fita!\n", argv[1]);
        
        /* --- INICIANDO ETAPA DE GERAÇÃO DE CÓDIGO FINAL (MIPS ASSEMBLY) --- */
        /* Fabricando o nome do arquivo de saída: removemos a extensão original (.g) e injetamos a nova (.s) */
        char out_nome[256];
        strncpy(out_nome, argv[1], sizeof(out_nome) - 3); /* Copia de forma segura com margem pra concatenação */
        out_nome[sizeof(out_nome) - 3] = '\0';
        
        /* Buscamos o último ponto '.' de trás pra frente no nome fornecido */
        char *ponto = strrchr(out_nome, '.');
        if (ponto != NULL) {
            *ponto = '\0'; /* Cortamos a string exatamente aqui ignorando o .g final */
        }
        
        /* Anexamos a extensão pura do Assembly MIPS e gravamos */
        strncat(out_nome, ".s", sizeof(out_nome) - strlen(out_nome) - 1);
        
        generateCode(ast_raiz, &tabelaDeSimbolos, out_nome);
    }

    return 0;
}
