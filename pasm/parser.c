#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cpu_default_defines.h>
#include <strhelper.h>
#include "parser.h"

static int str_in_list(const char *str, const char **list) {
  const char **p = list;
  while(*p) {
    if(streq(str, *p)) return 1;
    p++;
  }
  return 0;
}

static enum literal_type parse_literal(const char *str, union multival *v)
{
    char *endp;
    long int val_int;
    double val_double;
    if (1 == sscanf(str,"\"%[^\"]\"", v->s)) {
       return STR;
    } else if (1 == sscanf(str,"xx\"%[0-9A-Fa-f]\"", v->s)
                && 0 == strlen(v->s) % 2) {
      return HEX;
    } else if (( (val_int = strtol(str, &endp, 0)) || endp != str)  &&  *endp == '\0') {
      v->i = (int)val_int;
      return INT;
    } else if( (0.0 != (val_double = strtod(str, &endp)) || (endp != str)) && *endp == '\0' ) {
      v->f = (float)val_double;
      return FLOAT;
    }

    else return NONE;
}

static bool is_id(const char *str) {
  if (strlen(str) == 0) return false;
  if (!isalpha(str[0])) return false;
  for (int i = 1; i < strlen(str); i++) {
    if (!isalnum(str[i])) return false;
  }
  return true;
}

static enum cpu_default_instruction parse_instruction(const char *str)
{
  if(streq(str,"NOP")) return CPU_DEFAULT_NOP;
  else if (streq(str,"LDA")) return CPU_DEFAULT_LDA;
  else if (streq(str,"STA")) return CPU_DEFAULT_STA;
  else if (streq(str,"ADD")) return CPU_DEFAULT_ADD;
  else if (streq(str,"JMP")) return CPU_DEFAULT_JMP;
  else if (streq(str,"JMPF")) return CPU_DEFAULT_JMPF;
  else return CPU_DEFAULT_NONE;
}

const char *token_to_str(struct token *t)
{
  static char buff[1024];
  switch (t->type) {
    case NEWLINE:
      strcpy(buff,"<newline>");
      break;
    case SYMBOL:
      sprintf(buff,"<SYMBOL %s>",t->v.s);
      break;
    case SYMREF:
      sprintf(buff,"<lref %s>",t->v.s);
      break;
    case INSTRUCTION:
      sprintf(buff,"<op %s>", t->v.s);
      break;
    case LITERAL:
      switch(t->subtype) {
         case STR:
           sprintf(buff,"<lit.str %s>", t->v.s);
           break;
         case HEX:
           sprintf(buff,"<lit.hex %s>", t->v.s);
           break;
         /*case UINT8:
           sprintf(buff,"<lit.ui8 %02x>", t->v.ui8);
           break;
         case INT8:
           sprintf(buff,"<lit.i8 %d>", t->v.i8);
           break;
         case UINT16:
           sprintf(buff,"<lit.ui16 %04x>", t->v.ui16);
           break;
         case INT16:
           sprintf(buff,"<lit.i16 %d>", t->v.i16);
           break;
           */
         case FLOAT:
           sprintf(buff,"<lit.f %f>", t->v.f);
           break;
         case INT:
           sprintf(buff,"<lit.i %d/%#x>", t->v.i, t->v.i);
           break;
         }; break;
    case DIRECTIVE:
      sprintf(buff,"<dir %s>", t->v.s);
      break;
  }
  return buff;
}

static const char *KW_DIRECTIVE[] = {".GLOBAL", ".HEX",".STR",".BYTE",".WORD",NULL};
static const char *KW_INSTRUCTION[] = {"NOP", "LDA","STA","ADD","DIV","SUB","MUL","TEST","JMP","JMPF","JMPR", NULL};

static int parse_line(char *line, int num, struct list_item *tokens)
{
  char *token_str;
  struct token *parsed_token = NULL;
  enum literal_type parsed_literal_type;
  union multival parsed_value;
  enum cpu_default_instruction parsed_instruction;

  printf("Parsing line: >>>%s<<<\n", line);
  //cut comments
  if(token_str = strchr(line, TOKEN_COMMENT)) {
    *token_str = '\0';
  }
  token_str = strtok(line, DELIMS);
  while(token_str) {
      parsed_token = (struct token *)calloc(1, sizeof(struct token));
      parsed_token->line = num;
      if (lastch(token_str) == TOKEN_LABEL) {
        parsed_token->type = SYMBOL;
        strncpy(parsed_token->v.s, token_str, strlen(token_str) - 1);
      } else if (token_str[0] == TOKEN_LABELREF) {
        parsed_token->type = SYMREF;
        strcpy(parsed_token->v.s, token_str + 1);
      } else if (str_in_list(token_str, KW_DIRECTIVE)) {
        parsed_token->type = DIRECTIVE;
        strcpy(parsed_token->v.s, token_str);
      } else if (CPU_DEFAULT_NONE != (parsed_instruction = parse_instruction(token_str))) {
        parsed_token->type = INSTRUCTION;
        parsed_token->subtype = parsed_instruction;
        strcpy(parsed_token->v.s, token_str);
      } else if (NONE != (parsed_literal_type = (parse_literal(token_str, &parsed_value)))) {
        parsed_token->type = LITERAL;
        parsed_token->subtype = parsed_literal_type;
        memcpy(&parsed_token->v, &parsed_value, sizeof(union multival));
      } else {
        fprintf(stderr,"Can't parse token: %s\n", token_str);
        free(parsed_token);
        goto error;
      }
      list_add_data(tokens, parsed_token);
      token_str = strtok(NULL, DELIMS);
    }
    parsed_token = (struct token *)calloc(1, sizeof(struct token));
    parsed_token->type = NEWLINE;
    list_add_data(tokens, parsed_token);
    return 0;
error:
    if (parsed_token) free(parsed_token);
    return -1;
}

int parse_text(const char *text, int text_len, struct list_item *tokens)
{
    char line[MAX_LINE_LEN];
    const char *p = text, *prev = text;
    int line_num = 1;
    while (p < text + text_len ) {
      if (*p == '\n') {
         memcpy(line, prev, p - prev);
         line[p - prev] = '\0';
         if (0 != parse_line(line, line_num, tokens)) {
           fprintf(stderr, "Error on line %d\n", line_num);
           return -1;
         }
         line_num++;
         prev = p + 1;
      }
      p++;
    }
    return 0;
}

int parse_file(const char *path, struct list_item *tokens)
{
  size_t n;
  FILE *f = fopen(path, "r");
  if (!f) {
    fprintf(stderr, "Could not open %s\n", path);
    return -1;
  }
  struct stat st;
  fstat(fileno(f), &st);
  char *buff = (char *)malloc(st.st_size);
  if (!buff) {
    fprintf(stderr,"Could not allocate buffer for %s\n", path);
    return -1;
  }
  n = fread(buff, 1, st.st_size, f);
  fclose(f);
  if (n != st.st_size) {
    fprintf(stderr,"Could not read all bytes from %s\n", path);
    free(buff);
    return -1;
  }
  int res = parse_text(buff, n, tokens);
  free(buff);
  return res;
}
