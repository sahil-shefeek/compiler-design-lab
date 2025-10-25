#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct instruction {
	string result, arg1, arg2, op;
};

bool is_number(const std::string& s) {
    if (s.empty()) {
        return false;
    }

    return std::all_of(s.begin(), s.end(), ::isdigit);
}

int main()
{
	string input;
	vector <instruction> instructions;
	getline(cin, input);
	while (input != "done") {
		stringstream ss(input);
		string equals;
		instruction ins;
		if (!(ss >> ins.result >> equals >> ins.arg1))
		{
			cout << "Malformed instruction" << "\n";
			return 1;
		}
		if (!(ss >> ins.op >> ins.arg2)) {
			ins.op = "=";
			ins.arg2 = "";
		}
		instructions.emplace_back(ins);
		getline(cin, input);
	}

	cout << "Input entered:\n";
	cout << "|Op\t\t|Arg1\t\t|Arg2\t\t|Result|\n";
	for (auto const& ins: instructions) {
		if (ins.op == "=") {
			cout << "=" << "\t\t|" << ins.arg1 << "\t\t|" << "-" << "\t\t|" << ins.result << "\t|\n";
		} else {
			cout << ins.op << "\t\t|" << ins.arg1 << "\t\t|" << ins.arg2 << "\t\t|" << ins.result << "\t|\n";
		}
	}

	unordered_map<string, int> constants;

	for (auto &ins: instructions) {
		if (constants.count(ins.arg1)) {
			ins.arg1 = to_string(constants[ins.arg1]);
		}
		if (constants.count(ins.arg2)) {
			ins.arg2 = to_string(constants[ins.arg2]);
		}
		if (ins.op == "=" && is_number(ins.arg1)) {
			constants[ins.result] = stoi(ins.arg1);
		} else if (is_number(ins.arg1) && is_number(ins.arg2)) {
			if (ins.op == "+") {
				int result = stoi(ins.arg1) + stoi(ins.arg2);
				ins.arg1 = to_string(result);
			} else if (ins.op == "-") {
				int result = stoi(ins.arg1) - stoi(ins.arg2);
				ins.arg1 = to_string(result);
			} else if (ins.op == "*") {
				int result = stoi(ins.arg1) * stoi(ins.arg2);
				ins.arg1 = to_string(result);
			} else if (ins.op == "/") {
				int result = stoi(ins.arg1) / stoi(ins.arg2);
				ins.arg1 = to_string(result);
			}
			constants[ins.result] = stoi(ins.arg1);
			ins.op = "=";
			ins.arg2 = "";
		} else {
			constants.erase(ins.result);
		}
	}

	cout << "Optimized output:\n";
	for (auto const& ins: instructions) {
		if (ins.op == "=") {
			cout << "=" << "\t\t|" << ins.arg1 << "\t\t|" << "-" << "\t\t|" << ins.result << "\t|\n";
		} else {
			cout << ins.op << "\t\t|" << ins.arg1 << "\t\t|" << ins.arg2 << "\t\t|" << ins.result << "\t|\n";
		}
	}

	return 0;
}
