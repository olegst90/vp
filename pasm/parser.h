#ifndef PARSER_H
#define PARSER_H

#include <list.h>

#define DELIMS " \t"
#define TOKEN_COMMENT ';'
#define TOKEN_LABEL ':'
#define TOKEN_LABELREF '@'

enum token_type {
  NEWLINE,
  SYMBOL,
  SYMREF,
  INSTRUCTION,
  LITERAL,
  DIRECTIVE,
  //MACROBEGIN,
  //MACROEND
};

enum literal_type {
   STR,HEX,INT,FLOAT,NONE
};

union multival {
  int i;
  float f;
  char s[256];
};

struct token {
  enum token_type type;
  int subtype;
  union multival v;
  int line;
};

#define MAX_LINE_LEN 1024

const char *token_to_str(struct token *t);

int parse_text(const char *text, int text_len, struct list_item *tokens);
int parse_file(const char *path, struct list_item *tokens);
#endif //PARSER_H
