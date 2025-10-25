#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char result[10];
	char arg1[10], arg2[10];
	char op[10];
} instruction;

typedef struct {
	char name[10];
	int value;
} constant;

typedef struct {
	int found;
	int value;
} search_result;

constant constants[32];
int num_constants = 0;

void add_constant(char* name, int value)
{
	strcpy(constants[num_constants].name, name);
	constants[num_constants].value = value;
	num_constants++;
}

void remove_constant(char* name)
{
	for (int i = 0; i < num_constants; i++) {
		if (strcmp(constants[i].name, name) == 0) {
			for (int j = i; j < num_constants - 1; j++) {
				constants[j] = constants[j + 1];
			}
			num_constants--;
			break;
		}
	}
}

search_result read_constant(char* name)
{
	for (int i = 0; i < num_constants; i++) {
		if (strcmp(constants[i].name, name) == 0) {
			return (search_result){1, constants[i].value};
		}
	}
	return (search_result){0, 0};
}

void update_constant(char* name, int value)
{
	for (int i = 0; i < num_constants; i++) {
		if (strcmp(constants[i].name, name) == 0) {
			constants[i].value = value;
			return;
		}
	}
	add_constant(name, value);
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

	for (int i = 0; i < num_instructions; i++) {
		search_result res = read_constant(instructions[i].arg1);
		if (res.found) {
			sprintf(instructions[i].arg1, "%d", res.value);
		}
		res = read_constant(instructions[i].arg2);
		if (res.found) {
			sprintf(instructions[i].arg2, "%d", res.value);
		}
		if (is_number(instructions[i].arg1) && (strcmp(instructions[i].op, "=") == 0 || is_number(instructions[i].arg2))) {
			int result = atoi(instructions[i].arg1);
			int arg2 = instructions[i].arg2[0] == '\0' ? 0 : atoi(instructions[i].arg2);
			if (strcmp(instructions[i].op, "+") == 0) {
				result += arg2;
			} else if (strcmp(instructions[i].op, "-") == 0) {
				result -= arg2;
			} else if (strcmp(instructions[i].op, "*") == 0) {
				result *= arg2;
			} else if (strcmp(instructions[i].op, "/") == 0) {
				result /= arg2;
			}
			sprintf(instructions[i].arg1, "%d", result);
			strcpy(instructions[i].op, "=");
			strcpy(instructions[i].arg2, "-");
			update_constant(instructions[i].result, result);
		} else {
			remove_constant(instructions[i].result);
		}
	}


	printf("Optimized code:\n");
	printf("|op\t|arg1\t|arg2\t|res\t|\n");
	for (int i = 0; i < num_instructions; i++) {
		printf("|%s\t|%s\t|%s\t|%s\t|\n", instructions[i].op, instructions[i].arg1, instructions[i].arg2, instructions[i].result);
	}
	return 0;
}
