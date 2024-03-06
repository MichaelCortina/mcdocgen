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
        toklist = toklist_create(16);

        while (fgets(buffer, LINE_MAX, fp) != NULL) {
            TokHeader *token;
            char *trimmed = trimwhitespace(buffer);
            bool lexing_comment = FALSE;

            while (*trimmed != '\0') {

                if (startswith(trimmed, "*/")) {
                    TokHeader *token = malloc(sizeof(*token));
                    *token = TOK_COMMENTEND;
                    lexing_comment = FALSE; 
                    trimmed += 2;
                }

                else if (!lexing_comment) {

                    if (startswith(trimmed, "/*")) {
                        token = malloc(sizeof(*token));
                        *token = TOK_COMMENTSTART;
                        lexing_comment = TRUE;
                        trimmed += 2;
                    }

                    else if (startswith(trimmed, "typedef struct")) {
                        TokStructDef *temp = malloc(sizeof(TokStructDef) + strlen(trimmed));
                        temp->header = TOK_STRUCTDEF;
                        strcpy(temp->struct_def, trimmed);
                        token = (TokHeader *) temp;
                        trimmed += 14; 
                    } 

                    else if (startswith(trimmed, "typedef enum")) {
                        TokEnumDef *temp = malloc(sizeof(TokEnumDef) + strlen(trimmed));
                        temp->header = TOK_ENUMDEF;
                        strcpy(temp->enum_def, trimmed);
                        token = (TokHeader *) temp;
                        trimmed += 12;
                    } 

                    else if (startswith(trimmed, "{")) {
                        token = malloc(sizeof(*token));
                        *token = TOK_OPENBRACE; 
                        trimmed += 1;
                    }

                    else if (startswith(trimmed, "}")) {
                        token = malloc(sizeof(*token));
                        *token = TOK_CLOSEBRACE;
                        trimmed += 1;
                    } 

                    else {
                        TokChar *temp = malloc(sizeof(*token) + 1);
                        temp->header = TOK_CHAR;
                        temp->c = trimmed[0];
                        token = (TokHeader *) temp;
                        trimmed += 1;
                    }
                } /* end !lexing_comment */

                else {
                    TokChar *temp = malloc(sizeof(*token) + 1);
                    temp->header = TOK_CHAR;
                    temp->c = trimmed[0];
                    token = (TokHeader *) temp;
                    trimmed += 1;
                }

                toklist_add(toklist, token);
                trimwhitespace(trimmed);
            } /* end inner while */
            
            if (TRUE) { /* for testing purposes */
                TokChar *temp = malloc(sizeof(*token) + 1);
                temp->header = TOK_CHAR;
                temp->c = '\n';
                token = (TokHeader *) temp;
                toklist_add(toklist, token);
            }

        } /* end outer while */
        fclose(fp);
    } else {
        fprintf(stderr, "error opening file: %s", pathname);
        exit(1);
    }

    return toklist;
}

/* --------------------------- TokList --------------------------- */

TokList* toklist_create(int capacity) {
    TokList *retval = malloc(sizeof(*retval));

    retval->capacity = capacity;
    retval->elements = malloc(sizeof(*retval->elements) * capacity);
    retval->top = retval->elements;

    return retval;
}

void toklist_destroy(TokList **toklist) {
    TokHeader **p = (*toklist)->elements;

    while (p != (*toklist)->top) {
        free(*(p++));
    }

    free((*toklist)->elements);
    free(*toklist);
    toklist = NULL;
}


void toklist_add(TokList *list, TokHeader *element) {
    if (&list->elements[list->capacity] == list->top) {
        list->elements = realloc(list->elements, sizeof(TokHeader*) * (list->capacity * 2));
        list->top = &list->elements[list->capacity];
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
