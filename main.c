#include "mcdocgen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE* get_file_or_exit(int, char**);

int main(int argc, char* argv[]) {
  FILE* fp = get_file_or_exit(argc, argv);
  TokList *a = lex(fp);
  TokHeader **p;
  char *contents;
  char c;

  if (a != NULL) { 
    for (p = a->elements; p < a->top; p++) {
      switch (**p) {
        case TOK_COMMENTSTART:
          printf("/*\n");
          break;
        case TOK_COMMENTCONTENTS:
          contents = ((TokCommentContents *) (*p))->contents;
          printf(" * %s\n", contents);
          break;
        case TOK_COMMENTEND:
          printf(" */\n");
          break;
        case TOK_FUNCTIONHEADER:
          contents = ((TokFunctionHeader *) (*p))->function_header;
          printf("%s\n", contents);
          break;
        case TOK_STRUCTDEF:
          contents = ((TokStructDef *) (*p))->struct_def;
          printf("STRUCTDEF: %s\n", contents);
          break;
        case TOK_ENUMDEF:
          contents = ((TokEnumDef *) (*p))->enum_def;
          printf("ENUMDEF: %s\n", contents);
          break;
        case TOK_CHAR:
          c = ((TokChar *) (*p))->c;
          printf("%c", c);
          break;
      } /* end switch */
    } /* end for */

    toklist_destroy(&a); 
  } /* endif a */

  return 0;
}

FILE* get_file_or_exit(int argc, char* argv[]) {
  char* error_message = NULL;
  FILE* fp = NULL;

  if (argc != 2) {
    error_message = "Incorrect Number of Arguments! \nCorrect Usage: mcdocgen <filename> \n";
  }

  else { 
    fp = fopen(argv[1], "r"); 

    if (fp == NULL) {
      error_message = "File Not Found! \n"; 
    }
  }

  if (error_message != NULL) {
    fprintf(stderr, error_message, strlen(error_message));
    exit(2);
  }

  return fp;
} /* endfn */
