#include "Set.h"  
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

using namespace std;

bool isValid(string infix);

string removeSpaces(string infix);

void inSets(string infix, const Set& trueValues, const Set& falseValues, bool& onlyOne, bool& inBoth, bool& inNeither);

int checkPrecedence(char c);

string InToPost(string infix);
  
bool PostToEval(string post, const Set& trueValues);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
    // Check if infix is valid
    infix = removeSpaces(infix);
	if (!isValid(infix)) {
		return 1;
	}
    // Determine if the characters of infix are in only one set, both sets, or neither sets
    bool inOnlyOne = false;
    bool inBoth = false;
    bool inNeither = false;
    inSets(infix, trueValues, falseValues, inOnlyOne, inBoth, inNeither);

    // If is valid and chracters are in only set, find postfix and evaulate result
    if (inOnlyOne) {
        string post = InToPost(infix);
        postfix = post;
        result = PostToEval(post, trueValues);
        return 0;
    }
    
    if (inNeither && !inBoth) {
        return 2;
    } else if (inBoth && !inNeither) {
        return 3;
    } else if (inBoth && inNeither) {
        return 2;
    }
    return 2;
}

bool isValid(string infix) {
    int len = infix.size();
    // empty infix string
    if (len == 0) {
        return false;
    }
    // &, |, and ) cannot be in the first index
    if (infix[0] == '&' || infix[0] == '|' || infix[0] == ')') {
        return false;
    }
    // !, &, |, and ( cannot be in the last index
    char last = infix[len - 1];
    if (last == '!' || last == '&' || last == '|' || last == '(') {
        return false;
    }

    int open = 0;
    int letters = 0;

    for (int i = 0; i < len; i++) {
        char c = infix[i];
        // Checks digits
        if (isdigit(c)) {
            return false;
        }

        // Letter checks
        if (isalpha(c)) {
            // Checks lowercase letters
            if (!islower(c)) {
                return false;
            }
            // Checks consecutive letters
            if (i != len - 1 && isalpha(infix[i + 1])) {
                return false;
            }
            // Checks if each letter is followed by an operator or closing parenthese
            if (i != len - 1 && infix[i + 1] != '&' && infix[i + 1] != '|' && infix[i + 1] != ')') {
                return false;
            }
            // Counts operands
            letters++;
        }

        // Operator checks
        if (!isalpha(c) && c != '(' && c != ')') {
            // Checks if operators are valid
            if (c != '!' && c != '&' && c != '|' && c != '(' && c != ')') {
                return false;
            }
            // Checks values after ! operator, cannot be binary operators or )
            if (c == '!' && i != len - 1 && (infix[i + 1] == '&' || infix[i + 1] == '|' || infix[i + 1] == ')')) {
                return false;
            }
            // Checks values after | and & operators, cannot be followed by binary operators or )
            if (infix[i] == '|' || infix[i] == '&') {
                if (i != len - 1 && (infix[i + 1] == '|' || infix[i + 1] == '&' || infix[i + 1] == ')')) {
                    return false;
                }
            }
        }

        // Parenthese checks
        if (c == '(' || c == ')') {
            // Checks if all parentheses are closed
            if (c == '(') {
                open++;
            }
            else if (c == ')') {
                open--;
            }
            // Open parenthese can't be followed by an &, |, or )
            if (c == '(' && i != len - 1 && (infix[i + 1] == '&' || infix[i + 1] == '|' || infix[i + 1] == ')')) {
                return false;
            }
            // Closed parenthese must be followed by a binary operator or closed parenthese
            if (c == ')' && i != len - 1 && infix[i + 1] != '|' && infix[i + 1] != '&' && infix[i + 1] != ')') {
                return false;
            }
        }
	}
    return open == 0 && letters != 0;
}

string removeSpaces(string infix) {
    string res = "";
    for (char c : infix) {
        if (c != ' ') {
            res += c;
        }
    }
    return res;
}

void inSets(string infix, const Set& trueValues, const Set& falseValues, bool& onlyOne, bool& inBoth, bool& inNeither) {
    for (int i = 0; i < infix.length(); i++) {
        char c = infix[i];
        if (!isalpha(c)) {
            continue;
        }
        bool trueContains = trueValues.contains(c);
        bool falseContains = falseValues.contains(c);
        if (trueContains && falseContains) { // at least one letter in infix is in both trueValues and falseValues
            inBoth = true;
        }
        if (!trueContains && !falseContains) { // at least one letter in infix is in neither trueValues and falseValues
            inNeither = true;
        }
    }
    if (!inBoth && !inNeither) { // if there are no values that are in both or neither of the value Sets, then all letters are in only one Set
        onlyOne = true;
    }
}

int checkPrecedence(char c) {
    if (c == '!') { // highest precedence
        return 3;
    } else if (c == '&') {
        return 2;
    } else if (c == '|') { // lowest precedence
        return 1;
    }
    return 0;
}

string InToPost(string infix) {
    string post = "";
    stack<char> op;
    for (char c : infix) {
        if (isalpha(c)) {
            post += c;
        } 
        else if (c == '(' || c == '!') {
            op.push(c);
        } 
        else if (c == ')') {
            // pop stack until matching '('
            while (op.top() != '(') {
                post += op.top();
                op.pop(); // remove the '('
            }
            op.pop();
        } else {
            while (!op.empty() && op.top() != '(' && checkPrecedence(c) <= checkPrecedence(op.top())) {
                post += op.top();
                op.pop();
            }
            op.push(c);
        }
    }
    while (!op.empty()) {
        post += op.top();
        op.pop();
    }
    return post;
}

bool PostToEval(string post, const Set& trueValues) {
    stack<bool> op;
    for (char c : post) {
        if (isalpha(c)) { // if letter
            op.push(trueValues.contains(c)); // if letter is in trueValues, push true, else, push false
        }
        else if (c == '!') { // if char is !, push the opposite boolean onto the stack (true -> false, false -> true)
            bool curr = op.top();
            op.pop();
            op.push(!curr); 
        } else { // if character is a relational operator
            bool operand2 = op.top();
            op.pop();
            bool operand1 = op.top();
            op.pop();
            if (c == '|') {
                op.push(operand2 || operand1);
            }
            else if (c == '&') {
                op.push(operand2 && operand1);
            }
        }
    }
    return op.top();
}

int main()
{
    /*string trueChars = "tywz";
    string falseChars = "fnx";*/
    string trueChars = "aclun";
    string falseChars = "sxn";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);

    string pf;
    bool answer;

    assert(evaluate("u", trues, falses, pf, answer) == 0 && answer);
    assert(evaluate("u&c&l&a & !(u&s&c)", trues, falses, pf, answer) == 0 && answer);
    assert(evaluate("a&!(s|u&c|n)|!!!(s & u & n)", trues, falses, pf, answer) == 3 /*&& answer*/);
    assert(evaluate("(n)", trues, falses, pf, answer) == 3 /*&& !answer*/);
    assert(evaluate("a&(s)", trues, falses, pf, answer) == 0 && !answer);
    assert(evaluate("a & !s", trues, falses, pf, answer) == 0 && answer);
    assert(evaluate("a&)", trues, falses, pf, answer) == 1);
    assert(evaluate("8888", trues, falses, pf, answer) == 1);
    assert(evaluate("uuu", trues, falses, pf, answer) == 1);
    assert(evaluate("U&U", trues, falses, pf, answer) == 1);
    assert(evaluate("***u", trues, falses, pf, answer) == 1);
    assert(evaluate("!!!((s))", trues, falses, pf, answer) == 0 && answer);
    assert(evaluate("!(n|u)", trues, falses, pf, answer) == 3 /*&& !answer*/);
    assert(evaluate("!n|u", trues, falses, pf, answer) == 3 /*&& answer*/);
    assert(evaluate("a|n&n", trues, falses, pf, answer) == 3 /*&& answer*/);


    /*assert(evaluate("!((y&w)&!(w(w&y&w)))", trues, falses, pf, answer) == 1);
    assert(evaluate("!(w)", trues, falses, pf, answer) == 0 && !answer);
    assert(evaluate("!((w))", trues, falses, pf, answer) == 0 && !answer);
    assert(evaluate("!!w", trues, falses, pf, answer) == 0);

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
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0 && pf == "ff!tn&&|" && !answer);
    
    assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
    falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x" && answer);
    trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
    falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);*/
    cout << "Passed all tests" << endl;
}

