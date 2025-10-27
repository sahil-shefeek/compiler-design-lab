#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_RHS_LENGTH 20

typedef struct {
  char lhs;
  char rhs[MAX_RHS_LENGTH];
} ProductionRule;

char first_set[26][32];
char follow_set[26][32];

int add_to_set(char *set, char symbol) {
  if (strchr(set, symbol) == NULL) {
    int set_len = strlen(set);
    set[set_len] = symbol;
    set[set_len + 1] = '\0';
    return 1;
  }
  return 0;
}

int main() {
  int num_rules;
  char start_symbol;
  char line_buffer[100];

  printf("Enter the number of grammar rules: ");
  scanf("%d", &num_rules);
  ProductionRule grammar[num_rules];
  printf("Enter the start symbol: ");
  scanf(" %c", &start_symbol);
  getchar();
  printf("Enter the %d grammar rules (e.g., E=TX):\n", num_rules);
  for (int i = 0; i < num_rules; i++) {
    fgets(line_buffer, sizeof(line_buffer), stdin);
    sscanf(line_buffer, " %c=%s", &grammar[i].lhs, grammar[i].rhs);
  }

  printf("Grammar entered:\n");
  for (int i = 0; i < num_rules; i++) {
    printf("%c -> %s\n", grammar[i].lhs, grammar[i].rhs);
  }

  int has_changed = 1;
  while (has_changed) {
    has_changed = 0;
    for (int i = 0; i < num_rules; i++) {
      char NT = grammar[i].lhs;
      char *rhs = grammar[i].rhs;
      if (rhs[0] == '#') {
        if (add_to_set(first_set[NT - 'A'], '#')) {
          has_changed = 1;
        }
        continue;
      }
      int rhs_derives_epsilon = 1;
      for (int j = 0; rhs[j] != '\0'; j++) {
        char X = rhs[j];
        int X_derives_epsilon = 0;
        if (!isupper(X)) {
          if (add_to_set(first_set[NT - 'A'], X)) {
            has_changed = 1;
          }
          X_derives_epsilon = 0;
        } else {
          char *first_set_of_X = first_set[X - 'A'];
          for (int k = 0; first_set_of_X[k] != '\0'; k++) {
            char Y = first_set_of_X[k];
            if (Y == '#') {
              X_derives_epsilon = 1;
            } else {
              if (add_to_set(first_set[NT - 'A'], Y)) {
                has_changed = 1;
              }
            }
          }
        }
        if (!X_derives_epsilon) {
          rhs_derives_epsilon = 0;
          break;
        }
      }
      if (rhs_derives_epsilon) {
        if (add_to_set(first_set[NT - 'A'], '#')) {
          has_changed = 1;
        }
      }
    }
  }

  printf("First sets:\n");
  int printed[26] = {0};
  for (int i = 0; i < num_rules; i++) {
    char NT = grammar[i].lhs;
    if (printed[NT - 'A'])
      continue;
    printed[NT - 'A'] = 1;
    printf("%c: { ", NT);
    for (int j = 0; first_set[NT - 'A'][j] != '\0'; j++)
      printf("%c, ", first_set[NT - 'A'][j]);
    printf("}\n");
  }

  add_to_set(follow_set[start_symbol - 'A'], '$');
  has_changed = 1;
  while (has_changed) {
    has_changed = 0;
    for (int i = 0; i < num_rules; i++) {
      char NT = grammar[i].lhs;
      char *rhs = grammar[i].rhs;
      for (int j = 0; rhs[j] != '\0'; j++) {
        char X = rhs[j];
        if (isupper(X)) {
          // Find follow(X)
          // int rhs_len = strlen(rhs);
          int beta_derives_epsilon = 1;
          for (int k = j + 1; rhs[k] != '\0'; k++) {
            char Y = rhs[k];
            int Y_derives_epsilon = 0;
            if (!isupper(Y)) {
              if (add_to_set(follow_set[X - 'A'], Y)) {
                has_changed = 1;
              }
              Y_derives_epsilon = 0;
            } else {
              // Follow(x) contains first(Y)
              char *first_of_y = first_set[Y - 'A'];
              for (int p = 0; first_of_y[p] != '\0'; p++) {
                char Z = first_of_y[p];
                if (Z == '#') {
                  Y_derives_epsilon = 1;
                } else {
                  if (add_to_set(follow_set[X - 'A'], Z)) {
                    has_changed = 1;
                  }
                }
              }
            }
            if (!Y_derives_epsilon) {
              beta_derives_epsilon = 0;
              break;
            }
          }
          if (beta_derives_epsilon || rhs[j + 1] == '\0') {
            char *follow_set_NT = follow_set[NT - 'A'];
            for (int k = 0; follow_set_NT[k] != '\0'; k++) {
              if (add_to_set(follow_set[X - 'A'], follow_set_NT[k])) {
                has_changed = 1;
              }
            }
          }
        }
      }
    }
  }

  printf("Follow sets:\n");
  memset(printed, 0, sizeof(printed));
  for (int i = 0; i < num_rules; i++) {
    char NT = grammar[i].lhs;
    if (printed[NT - 'A'])
      continue;
    printed[NT - 'A'] = 1;
    printf("%c: { ", NT);
    for (int j = 0; follow_set[NT - 'A'][j] != '\0'; j++)
      printf("%c, ", follow_set[NT - 'A'][j]);
    printf("}\n");
  }
  return 0;
}
