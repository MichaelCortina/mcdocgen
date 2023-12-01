#include "mcdocgen.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
  TokList *a = lex("./mcdocgen.h");
  TokHeader **p;
  char *contents;

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
      } /* end switch */
    } /* end for */

    toklist_destroy(&a); 
  } /* endif a */

  return 0;
}
