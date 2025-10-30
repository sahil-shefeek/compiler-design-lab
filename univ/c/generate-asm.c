#include <ctype.h>
#include <stdio.h>
#include <string.h>

char input[128], postfix[128];

char operator_stack[16];
int operator_top = -1;

char operand_stack[16][32];
int operand_top = -1;

int precedence(char op) {
  switch (op) {
  case '=':
    return 0;
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  case '^':
    return 3;
  default:
    return -1;
  }
}

void append(char *str, char symbol) {
  int len = strlen(str);
  str[len] = symbol;
  str[len + 1] = '\0';
}

void push_op(char c) { operator_stack[++operator_top] = c; }

char pop_op() { return operator_stack[operator_top--]; }

char top_op() { return operator_stack[operator_top]; }

int is_empty_op() { return operator_top == -1; }

void push_operand(const char *s) { strcpy(operand_stack[++operand_top], s); }

char *pop_operand() { return operand_stack[operand_top--]; }

char *top_operand() { return operand_stack[operand_top]; }

int is_empty_operand() { return operand_top == -1; }

void parse_postfix() {
  postfix[0] = '\0';
  for (int i = 0; input[i] != '\0'; i++) {
    char c = input[i];
    if (isalnum(c)) {
      append(postfix, c);
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      while (!is_empty_op() && precedence(top_op()) >= precedence(c)) {
        append(postfix, pop_op());
      }
      push_op(c);
    } else if (c == '^' || c == '=') {
      while (!is_empty_op() && precedence(top_op()) > precedence(c)) {
        append(postfix, pop_op());
      }
      push_op(c);
    } else if (c == '(') {
      push_op(c);
    } else if (c == ')') {
      while (!is_empty_op() && top_op() != '(') {
        append(postfix, pop_op());
      }
      if (!is_empty_op())
        pop_op();
    }
  }
  while (!is_empty_op()) {
    append(postfix, pop_op());
  }
}

void generate_asm() {
  int tcount = 0;
  printf("Generated assemby code:\n");
  for (int i = 0; postfix[i] != '\0'; i++) {
    char c = postfix[i];
    if (isalnum(c)) {
      char temp[] = {c, '\0'};
      push_operand(temp);
    } else if (c == '=') {
      char lhs[32], rhs[32];
      strcpy(rhs, pop_operand());
      strcpy(lhs, pop_operand());
      printf("MOV %s, %s\n", lhs, rhs);
      push_operand(lhs);
    } else {
      char arg1[32], arg2[32];
      strcpy(arg2, pop_operand());
      strcpy(arg1, pop_operand());
      char temp[32];
      sprintf(temp, "T%d", tcount++);
      printf("MOV R0, %s\n", arg1);
      switch (c) {
      case '+':
        printf("ADD");
        break;
      case '-':
        printf("SUB");
        break;
      case '*':
        printf("MUL");
        break;
      case '/':
        printf("DIV");
        break;
      case '^':
        printf("POW");
        break;
      }
      printf(" R0, %s\n", arg2);
      printf("MOV %s, R0\n", temp);
      push_operand(temp);
    }
  }
}

int main() {
  printf("Enter input expression: ");
  scanf("%s", input);
  printf("Input: %s\n", input);

  parse_postfix();
  printf("Postfix: %s\n", postfix);

  generate_asm();
  return 0;
}
