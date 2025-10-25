#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char result[10];
	char arg1[10], arg2[10];
	char op[10];
} instruction;

char live[32][32];
int num_live_vars = 0;

int is_live(char *var_name) {
	for (int i = 0; i < num_live_vars; i++) {
		if (strcmp(live[i], var_name) == 0) {
			return 1;
		}
	}
	return 0;
}

void add_to_live(char *var_name) {
	strcpy(live[num_live_vars], var_name);
	num_live_vars++;
}

void remove_from_live(char *var_name) {
	for (int i = 0; i < num_live_vars; i++) {
		if (strcmp(live[i], var_name) == 0) {
			for (int j = i; j < num_live_vars - 1; j++) {
				strcpy(live[j], live[j + 1]);
			}
			num_live_vars--;
			break;
		}
	}
}

int is_number(const char* str) {
    if (str == NULL || *str == '\0' || isspace(*str)) {
        return 0;
    }

    char* end;
    strtol(str, &end, 10);
    return (*end == '\0');
}

int main()
{
	char input[128];
	int num_instructions;
	printf("Enter the number of instructions: ");
	scanf("%d", &num_instructions);
	instruction instructions[num_instructions];
	printf("Enter instructions (space-separated):\n");
	getchar();
	for (int i = 0; i < num_instructions; i++) {
		fgets(input, sizeof(input), stdin);
		instruction ins;
		if (sscanf(input, "%s = %s %s %s", ins.result, ins.arg1, ins.op, ins.arg2) != 4) {
			sscanf(input, "%s = %s", ins.result, ins.arg1);
			ins.op[0] = '=';
			ins.op[1] = '\0';
			ins.arg2[0] = '\0';
		}
		instructions[i] = ins;
	}

	printf("Entered input:\n");
	printf("|op\t|arg1\t|arg2\t|res\t|\n");
	for (int i = 0; i < num_instructions; i++) {
		printf("|%s\t|%s\t|%s\t|%s\t|\n", instructions[i].op, instructions[i].arg1, instructions[i].arg2, instructions[i].result);
	}

	instruction optimized[num_instructions];
	int num_optimized = 0;
	for (int i = 0; i < num_instructions; i++) {
		if (strcmp(instructions[i].op, "=") == 0) { 
			optimized[num_optimized++] = instructions[i];
			continue;
		}
		instruction temp = instructions[i];
		int found_index = -1;
		for (int j = num_optimized - 1; j >= 0; j--) {
			if ((strcmp(temp.op, optimized[j].op) == 0) 
				&& (strcmp(temp.arg1, optimized[j].arg1) == 0)
				&& (strcmp(temp.arg2, optimized[j].arg2) == 0)) {
				found_index = j;
				break;
			}

			if ((strcmp(temp.arg1, optimized[j].result) == 0) || (strcmp(temp.arg2, optimized[j].result) == 0)) {
				break;
			}

		}
		if (found_index != -1) {
			strcpy(temp.arg1, optimized[found_index].result);
			temp.arg2[0] = '\0';
			strcpy(temp.op, "=");
		}
		optimized[num_optimized++] = temp;
	}

	printf("After sub-expression elimination:\n");
	printf("|op\t|arg1\t|arg2\t|res\t|\n");
	for (int i = 0; i < num_optimized; i++) {
		printf("|%s\t|%s\t|%s\t|%s\t|\n", optimized[i].op, optimized[i].arg1, optimized[i].arg2, optimized[i].result);
	}

	add_to_live(optimized[num_optimized - 1].result);
	int is_dead[num_optimized];
	memset(is_dead, 0, num_optimized * sizeof(int));
	for (int i = num_optimized - 1; i >= 0; i--) {
		if(is_live(optimized[i].result)) {
			remove_from_live(optimized[i].result);
			if (!is_number(optimized[i].arg1)) {
				add_to_live(optimized[i].arg1);
			}
			if (!is_number(optimized[i].arg2)) {
				add_to_live(optimized[i].arg2);
			}
		} else {
			is_dead[i] = 1;
		}
	}


	printf("After dead code elimination:\n");
	printf("|op\t|arg1\t|arg2\t|res\t|\n");
	for (int i = 0; i < num_optimized; i++) {
		if (is_dead[i]) continue;
		printf("|%s\t|%s\t|%s\t|%s\t|\n", optimized[i].op, optimized[i].arg1, optimized[i].arg2, optimized[i].result);
	}
	return 0;
}
