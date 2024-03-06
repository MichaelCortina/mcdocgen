#ifndef MCDOCGEN_H
#define MCDOCGEN_H

#define FALSE 0
#define TRUE 1
typedef short bool;

/*
 * defines all of the tokens that will
 * be lexed and parsed by mcdocgen
 */
typedef enum token_header {
  TOK_COMMENTSTART,
  TOK_COMMENTCONTENTS,
  TOK_COMMENTEND,
  TOK_FUNCTIONHEADER, 
  TOK_STRUCTDEF,
  TOK_ENUMDEF,
  TOK_OPENBRACE,
  TOK_CLOSEBRACE,
  TOK_OPENPAREN,
  TOK_CLOSEPAREN,
  TOK_CHAR
} TokHeader;

typedef struct token_char {
    TokHeader header;
    char c;
} TokChar;

typedef struct token_enum_definition {
  TokHeader header;
  char enum_def[1];
} TokEnumDef;

typedef struct token_struct_definition {
  TokHeader header;
  char struct_def[1];
} TokStructDef;

typedef struct token_function_header {
  TokHeader header;
  char function_header[1];
} TokFunctionHeader;

typedef struct token_comment_contents {
  TokHeader header;
  char contents[1];
} TokCommentContents;

typedef struct token_list {
  int capacity;
  TokHeader **top;
  TokHeader **elements;
} TokList;

TokList *lex(char *pathname);

char *trimwhitespace(char *str);
short startswith(char *target, char *substring);

TokList *toklist_create(int capacity);
void     toklist_destroy(TokList **toklist);
void     toklist_add(TokList *list, TokHeader *token);

#endif
