#include "mcdocgen.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    TokList *a = lex("./mcdocgen.h");
    TokHeader **p;
    char *contents;
    
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
        }
    }

    free(a);
    return 0;
}
