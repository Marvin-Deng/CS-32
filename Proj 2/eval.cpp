#include "Set.h"  
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

using namespace std;

bool isValid(string infix, const Set& trueValues, const Set& falseValues);

string removeSpaces(string infix);

string InToPost(string infix);

string PostToEval(string post);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
	if (!isValid) {
		return 1;
	}
	return 1;

}

bool isValid(string infix, const Set& trueValues, const Set& falseValues) {
    stack<char> paren;
	for (char c : infix) {
        // Checks if characters in infix are included in the value sets
		if (isalpha(c) && !trueValues.contains(c) || !falseValues.contains(c)) {
			return false;
		}

        // Checks valid operators
        if (!isalpha(c)) {

        }

        // Checks valid parentheses
        if (c == '(') {
            paren.push('(');
        }
        else if (c == ')') {
            if (paren.empty()) {
                return false;
            }
            paren.pop();
        }
	}
    return paren.empty();
}

int main()
{
    string trueChars = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);

    string pf;
    bool answer;
    assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && answer);
    assert(evaluate("y|", trues, falses, pf, answer) == 1);
    assert(evaluate("n t", trues, falses, pf, answer) == 1);
    assert(evaluate("nt", trues, falses, pf, answer) == 1);
    assert(evaluate("()", trues, falses, pf, answer) == 1);
    assert(evaluate("()z", trues, falses, pf, answer) == 1);
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("n+y", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
        && pf == "ff!tn&&|" && !answer);
    assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
    falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x" && answer);
    trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
    falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);
    cout << "Passed all tests" << endl;
}


