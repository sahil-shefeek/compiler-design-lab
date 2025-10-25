#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
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

	vector<instruction> optimized;
	optimized.reserve(instructions.size());
	// optimized.emplace_back(instructions[0]);
	
	for (auto const& ins : instructions) {
		if (ins.op == "=") {
			optimized.emplace_back(ins);
			continue;
		}
		int found_index = -1;
		for (int i = optimized.size() - 1; i >= 0; i--) {
			if (ins.op == optimized[i].op && ins.arg1 == optimized[i].arg1 && ins.arg2 == optimized[i].arg2) {
				found_index = i;
				break;
			}

			if (optimized[i].result == ins.arg1 || optimized[i].result == ins.arg2) {
				break;
			}
		}
		instruction temp = ins;
		if (found_index != -1) {
			temp.arg1 = optimized[found_index].result;
			temp.arg2 = "";
			temp.op = "=";
		}
		optimized.emplace_back(temp);
	}

	cout << "After sub-expression elimination:\n";
	for (auto const& ins: optimized) {
		if (ins.op == "=") {
			cout << "=" << "\t\t|" << ins.arg1 << "\t\t|" << "-" << "\t\t|" << ins.result << "\t|\n";
		} else {
			cout << ins.op << "\t\t|" << ins.arg1 << "\t\t|" << ins.arg2 << "\t\t|" << ins.result << "\t|\n";
		}
	}

	unordered_set<string> live;
	vector<bool> dead(optimized.size(), 0);
	live.emplace(optimized.back().result);

	for(int i = optimized.size() - 1; i >= 0; i--) {
		if (live.count(optimized[i].result)) {
			live.erase(optimized[i].result);
			if (!is_number(optimized[i].arg1)) {
				live.emplace(optimized[i].arg1);
			}
			if (!is_number(optimized[i].arg2)) {
				live.emplace(optimized[i].arg2);
			}
		} else {
			dead[i] = true;
		}
	}

	cout << "Optimized output:\n";
	for (int i = 0; i < optimized.size(); i++) {
		if (dead[i]) continue;
		if (optimized[i].op == "=") {
			cout << "=" << "\t\t|" << optimized[i].arg1 << "\t\t|" << "-" << "\t\t|" << optimized[i].result << "\t|\n";
		} else {
			cout << optimized[i].op << "\t\t|" << optimized[i].arg1 << "\t\t|" << optimized[i].arg2 << "\t\t|" << optimized[i].result << "\t|\n";
		}
	}
	return 0;
}
