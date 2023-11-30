#ifndef MCDOCGEN_H
#define MCDOCGEN_H

/*
 * defines all of the tokens that will
 * be lexed and parsed by mcdocgen
 */
typedef enum token_header {
    TOK_COMMENTSTART,
    TOK_COMMENTCONTENTS,
    TOK_COMMENTEND
} TokHeader;

typedef struct token_comment_contents {
    TokHeader header;
    char contents[1];
} TokCommentContents;

typedef struct token_list {
    int capacity;
    TokHeader **top;
    TokHeader *elements[1];
} TokList;

TokList *lex(char *pathname);

char *trimwhitespace(char *str);
short startswith(char *target, char *substring);

TokList *toklist_create(int capacity);
void     toklist_add(TokList *list, TokHeader *token);

#endif