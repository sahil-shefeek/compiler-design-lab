#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <format>

using namespace std;

struct instruction {
	string arg1, arg2, op, res;
};

int precedence(char op)
{
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

string parse_postfix(const string& infix) {
	string postfix;
	postfix.reserve(infix.size());
	stack<char> operator_stack;
	for (auto c: infix) {
		if (isalnum(c)) {
			postfix += c;
		} else if (c == '+' || c == '-' || c == '*' || c == '/') {
			while ((!operator_stack.empty()) && precedence(operator_stack.top()) >= precedence(c)) {
				postfix += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.push(c);
		} else if (c == '^' || c == '=') {
			while ((!operator_stack.empty()) && precedence(operator_stack.top()) > precedence(c)) {
				postfix += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.push(c);
		} else if (c == '(') {
			operator_stack.push(c);
		} else if (c == ')') {
			while ((!operator_stack.empty()) && operator_stack.top() != '(') {
				postfix += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.pop();
		}
	}

	while (!operator_stack.empty()) {
		postfix += operator_stack.top();
		operator_stack.pop();
	}

	return postfix;
}

void print_quadruples(const vector<instruction>& instructions)
{
	cout << "|  OP  " << "|  ARG-1  " << "|  ARG-2  " << "|  Result  " << "|\n";
	for (auto const& ins: instructions) {
		cout << "|  " << ins.op << "  " << "|  " << ins.arg1 << "  " << "|  " << ins.arg2 << "  " << "|  " << ins.res << "  |\n";
	}
}

void icg(const string& postfix)
{
	int constant_counter = 0;
	stack<string> operand_stack;
	vector<instruction> instructions;
	for (char c: postfix) {
		if (isalnum(c)) {
			operand_stack.push(string(1, c));
		} else if (c == '=') {
			string arg2 = operand_stack.top();
			operand_stack.pop();
			string arg1 = operand_stack.top();
			operand_stack.pop();
			instructions.emplace_back(instruction{arg2, "", string(1, c), arg1});
			operand_stack.push(arg1);
		} else {
			string arg2 = operand_stack.top();
			operand_stack.pop();
			string arg1 = operand_stack.top();
			operand_stack.pop();
			string temp = format("T{}", constant_counter);
			instructions.emplace_back(instruction{arg1, arg2, string(1, c), temp});
			operand_stack.push(temp);
			constant_counter++;
		}
	}
	print_quadruples(instructions);
}

int main()
{
	string input;
	cout << "Enter input expression: ";
	cin >> input;
	cout << "String input: " << input << "\n";
	string postfix = parse_postfix(input);
	cout << "postfix: " << postfix << "\n";
	icg(postfix);
	return 0;
}
