#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

using namespace std;


int main()
{
	cout << "Enter number of grammar rules: ";
	int num_rules;
	cin >> num_rules;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	unordered_map<char, vector<string>> rules;
	rules.reserve(num_rules);
	char start_symbol;
	cout << "Enter start symbol:";
	cin >> start_symbol;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "Enter grammar rules:\n";
	for (int i = 0; i < num_rules; i++) {
		char start, equals;
		string input, rhs;
		getline(cin, input);
		stringstream ss(input);
		ss >> start >> equals >> rhs;
		rules[start].emplace_back(rhs);
	}

	cout << "Grammar entered:\n";
	for (auto const& rule: rules) {
		for (auto const& rhs: rule.second) {	
			cout << rule.first << " -> " << rhs << "\n";
		}
	}

	unordered_map<char, unordered_set<char>> first_set;
	first_set.reserve(num_rules);
	bool has_changed = true;
	while (has_changed) {
		has_changed = false;
		for (auto const& rule: rules) {
			for (auto const& rhs: rule.second) {
				bool produces_epsilon = true;
				for (auto sym: rhs) {
					if (!isupper(sym)) {
						if (first_set[rule.first].emplace(sym).second) {
							has_changed = true;
						};
						produces_epsilon = false;
						break;
					}

					bool sym_derives_epsilon = false;
					for (auto const& elem: first_set[sym]) {
						if (elem == '#') {
							sym_derives_epsilon = true;
						} else {
							if (first_set[rule.first].emplace(elem).second) {
								has_changed = true;
							}
						}
					}

					if(!sym_derives_epsilon) {
						produces_epsilon = false;
						break;
					}
				}
				if (produces_epsilon) {
					if (first_set[rule.first].emplace('#').second) {
						has_changed = true;
					}
				}
			}
		}
	}

	cout << "First sets:\n";
	for (auto const& elem: first_set) {
		cout << elem.first << ": {";
		for (auto ch: elem.second) {
			cout << ch << ", ";
		}
		cout << "}\n";
	}

	unordered_map<char, unordered_set<char>> follow_set;
	follow_set.reserve(num_rules);
	follow_set[start_symbol].emplace('$');
	has_changed = true;
	while (has_changed) {
		has_changed = false;
		for (auto const& [NT, prods]: rules) {
			for (auto const& rhs: prods) {
				for (int i = 0; i < rhs.size(); i++) {
					char X = rhs[i];
					if(isupper(X)) {
						int j = i + 1;
						bool X_can_be_epsilon = true;
						for (; j < rhs.size(); j++) {
							char Y = rhs[j];
							bool Y_has_epsilon = false;
							if (isupper(Y)) {
								for (auto const& elem: first_set[Y]) {
									if (elem == '#') {
										Y_has_epsilon = true;
									} else {
										if (follow_set[X].emplace(elem).second) {
											has_changed = true;
										}
									}
								}
							} else {
								if (follow_set[X].emplace(Y).second) {
									has_changed = true;
									Y_has_epsilon = false;
								}
							}
							if (!Y_has_epsilon) {
								X_can_be_epsilon = false;
								break;
							}
						}

						if (j == rhs.size() || X_can_be_epsilon) {
							for (auto T: follow_set[NT]) {
								if (follow_set[X].emplace(T).second) {
									has_changed = true;
								}
							}
						}
					}
				}
			}
		}
	}

	cout << "Follow sets:\n";
	for (auto const& elem: follow_set) {
		cout << elem.first << ": {";
		for (auto ch: elem.second) {
			cout << ch << ", ";
		}
		cout << "}\n";
	}
	return 0;
}
