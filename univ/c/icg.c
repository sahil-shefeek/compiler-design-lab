#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char arg1[32], arg2[32], op, res[32];
} instruction;

char input[128], postfix[128];
instruction instructions[32];
int num_instructions = 0;

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

void print_quadruples() {
  printf("\n|  OP  |  ARG-1  |  ARG-2  |  Result  |\n");
  printf("--------------------------------------\n");
  for (int i = 0; i < num_instructions; i++) {
    printf("|  %c   |  %-6s |  %-6s |  %-6s |\n", instructions[i].op,
           instructions[i].arg1, instructions[i].arg2, instructions[i].res);
  }
}

void icg() {
  int tcount = 1;
  for (int i = 0; postfix[i] != '\0'; i++) {
    char c = postfix[i];
    if (isalnum(c)) {
      char temp[2] = {c, '\0'};
      push_operand(temp);
    } else if (c == '=') {
      char rhs[32], lhs[32];
      strcpy(rhs, pop_operand());
      strcpy(lhs, pop_operand());
      instruction inst;
      sprintf(inst.arg1, "%s", rhs);
      inst.arg2[0] = '\0';
      inst.op = '=';
      sprintf(inst.res, "%s", lhs);
      instructions[num_instructions++] = inst;
      push_operand(lhs);
    } else {
      char op2[32], op1[32];
      strcpy(op2, pop_operand());
      strcpy(op1, pop_operand());
      instruction inst;
      sprintf(inst.arg1, "%s", op1);
      sprintf(inst.arg2, "%s", op2);
      inst.op = c;
      sprintf(inst.res, "T%d", tcount++);
      instructions[num_instructions++] = inst;
      push_operand(inst.res);
    }
  }
  print_quadruples();
}

int main() {
  printf("Enter input expression: ");
  scanf("%s", input);
  printf("Input: %s\n", input);

  parse_postfix();
  printf("Postfix: %s\n", postfix);

  icg();
  return 0;
}
