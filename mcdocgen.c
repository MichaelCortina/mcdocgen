#include "mcdocgen.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024

TokList* lex(char *pathname) {
    TokList *toklist = NULL;
    FILE *fp = fopen(pathname, "r");

    if (fp != NULL) {
        char buffer[LINE_MAX];
        toklist = toklist_create(10);

        while (fgets(buffer, LINE_MAX, fp) != NULL) {
            char *trimmed = trimwhitespace(buffer);
            
            if (startswith(trimmed, "/*")) {
                TokHeader *token = malloc(sizeof(*token));
                *token = TOK_COMMENTSTART;
                toklist_add(toklist, token);

            } else if (startswith(trimmed, "* ")) {
                TokCommentContents *token = malloc(sizeof(TokCommentContents) + strlen(trimmed += 2) + 1);
                token->header = TOK_COMMENTCONTENTS;
                strcpy(token->contents, trimmed);
                toklist_add(toklist, (TokHeader *) token);

            } else if (startswith(trimmed, "*/")) {
                TokHeader *token = malloc(sizeof(*token));
                *token = TOK_COMMENTEND;
                toklist_add(toklist, token);
            }

        }

        fclose(fp);
    } else {
        fprintf(stderr, "error opening file: %s", pathname);
        exit(1);
    }

    return toklist;
}

/* --------------------------- TokList --------------------------- */

TokList* toklist_create(int capacity) {
    TokList *retval = malloc(sizeof(*retval) + sizeof(TokHeader*) * (capacity - 1));

    retval->capacity = capacity;
    retval->top = retval->elements;

    return retval;
}

void toklist_add(TokList *list, TokHeader *element) {
    if (&list->elements[list->capacity] == list->top) {
        list = realloc(list, sizeof(TokList) + sizeof(TokHeader*) * (list->capacity * 2 - 1));
        list->capacity *= 2;
    }
    *(list->top++) = element;
}

/* --------------------- String Manipulation ------------------- */

/*
 * stole this one from stackoverflow and it has proven useful
 */
char *trimwhitespace(char *str)
{
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0) 
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  end[1] = '\0';

  return str;
}

short startswith(char *target, char *substring) { 
    return strstr(target, substring) == target;
}