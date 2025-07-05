#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 64

FILE *source;

typedef enum {
  LESS,
  GREATER,
  LESS_EQUALS,
  GREATER_EQUALS,
  EQUALS,
  NOT_EQUALS,
  ASSIGN,
  NOT,
  ADD,
  SUB,
  MUL,
  DIV,
  INCREMENT,
  DECREMENT,
  KEYWORD,
  IDENTIFIER,
  NUMBER,
  SPECIAL_SYMBOL,
  UNKNOWN_TOKEN,
  END_OF_FILE,
} TokenType;

const char *token_types[] = {"LESS",          "GREATER",
                             "LESS_EQUALS",   "GREATER_EQUALS",
                             "EQUALS",        "NOT_EQUALS",
                             "ASSIGN",        "NOT",
                             "ADD",           "SUB",
                             "MUL",           "DIV",
                             "INCREMENT",     "DECREMENT",
                             "KEYWORD",       "IDENTIFIER",
                             "NUMBER",        "SPECIAL_SYMBOL",
                             "UNKNOWN_TOKEN", "END_OF_FILE"};

const char *keywords[] = {"int", "main"};

bool peek_and_check(char expected_char) {
  char next_char = fgetc(source);
  if (next_char != expected_char) {
    if (!isspace(next_char)) {
      ungetc(next_char, source);
    }
    return false;
  }
  return true;
}

bool is_keyword(char *lexeme_buf) {
  int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
  for (int i = 0; i < num_keywords; i++) {
    if (strcmp(keywords[i], lexeme_buf) == 0) {
      return true;
    }
  }
  return false;
}

TokenType get_token(char *lexeme_buf) {
  lexeme_buf[0] = '\0';
  char character;
  do {
    character = fgetc(source);
  } while (isspace(character));
  if (character == EOF) {
    return END_OF_FILE;
  }
  lexeme_buf[0] = character;
  lexeme_buf[1] = '\0';
  switch (character) {
  case '<':
    if (peek_and_check('=')) {
      return LESS_EQUALS;
    } else {
      return LESS;
    }

  case '>':
    if (peek_and_check('=')) {
      return GREATER_EQUALS;
    } else {
      return GREATER;
    }

  case '+':
    if (peek_and_check('+')) {
      return INCREMENT;
    } else {
      return ADD;
    }

  case '-':
    if (peek_and_check('-')) {
      return DECREMENT;
    } else {
      return SUB;
    }

  case '=':
    if (peek_and_check('=')) {
      return EQUALS;
    } else {
      return ASSIGN;
    }
  case '!':
    if (peek_and_check('=')) {
      return NOT_EQUALS;
    } else {
      return NOT;
    }
  case '*':
    return MUL;
  case '/':
    return DIV;
  case '(':
  case ')':
  case '{':
  case '}':
  case ';':
    return SPECIAL_SYMBOL;
  }

  if (character == '_' || isalpha(character)) {
    int i = 0;
    do {
      if (i < BUFSIZE - 1) {
        lexeme_buf[i++] = character;
      }
      character = fgetc(source);
    } while (isalnum(character) || character == '_');
    if (!isspace(character)) {
      ungetc(character, source);
    }
    lexeme_buf[i] = '\0';
    if (is_keyword(lexeme_buf)) {
      return KEYWORD;
    }
    return IDENTIFIER;
  } else if (isdigit(character)) {
    do {
      character = fgetc(source);
    } while (isdigit(character));
    if (!isspace(character)) {
      ungetc(character, source);
    }
    return NUMBER;
  } else {
    return UNKNOWN_TOKEN;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "ERROR: Missing source filename as argument.\n");
    return 1;
  }
  source = fopen(argv[1], "r");
  if (source) {
    TokenType current_token;
    char lexeme[BUFSIZE];
    while ((current_token = get_token(lexeme)) != END_OF_FILE) {
      printf("Token: %-25s\t Lexeme: %-60s\n", token_types[current_token],
             lexeme);
    }
  } else {
    perror("Failed to open source file");
  }
  fclose(source);
  return 0;
}
