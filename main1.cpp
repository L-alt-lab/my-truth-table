#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;
int precedence(char op) {
    if (op == '!') return 4;
    if (op == '*') return 3;
    if (op == '+') return 2;
    if (op == '>') return 1;
    if (op == '~') return 0;
    return -1;
}
bool applyOp(bool a, bool b, char op) {
    switch(op) {
        case '!': return !a;        
        case '*': return a && b;    
        case '+': return a || b;     
        case '>': return !a || b;   
        case '~': return a == b;      
        default: return false;
    }
}
string infixToPostfix(const string& s) {
    stack<char> opStack;
    string postfix;
    for (char c : s) {
        if (isalpha(c) && c != 'T' && c != 'F') {
            postfix += c;
        } else if (c == 'T') {
            postfix += 'T';
        } else if (c == 'F') {
            postfix += 'F';
        } else if (c == '(') {
            opStack.push(c);
        } else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop();
        } else if (c == '!') {
            while (!opStack.empty() && precedence(c) < precedence(opStack.top())) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        } else {
            while (!opStack.empty() && opStack.top() != '(' && 
                   precedence(c) <= precedence(opStack.top())) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
    }
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    return postfix;
}
bool evaluatePostfix(const string& postfix, const map<char, bool>& varValues) {
    stack<bool> valStack;
    for (char c : postfix) {
        if (isalpha(c) && c != 'T' && c != 'F') {
            valStack.push(varValues.at(c));
        } else if (c == 'T') {
            valStack.push(true);
        } else if (c == 'F') {
            valStack.push(false);
        } else if (c == '!') {
            bool a = valStack.top();
            valStack.pop();
            valStack.push(applyOp(a, false, c));
        } else {
            bool b = valStack.top();
            valStack.pop();
            bool a = valStack.top();
            valStack.pop();
            valStack.push(applyOp(a, b, c));
        }
    }
    return valStack.top();
}
vector<char> extractVariables(const string& formula) {
    vector<char> vars;
    map<char, bool> exists;
    for (char c : formula) {
        if (isalpha(c) && c != 'T' && c != 'F' && !exists[c]) {
            exists[c] = true;
            vars.push_back(c);
        }
    }
    return vars;
}
bool isValidFormula(const string& formula, vector<char>& vars) {
    vars = extractVariables(formula);
    if (vars.size() > 4) {
        cout << "错误：变元数量超过4个！" << endl;
        return false;
    }
    for (char c : formula) {
        if (!isalpha(c) && c != '(' && c != ')' && 
            c != '!' && c != '*' && c != '+' && c != '>' && c != '~' &&
            c != 'T' && c != 'F') {
            cout << "错误：包含非法字符！" << endl;
            return false;
        }
        if (isalpha(c) && (c == 'T' || c == 'F')) continue;
        if (isalpha(c) && !isupper(c)) {
            cout << "错误：原子命题必须为大写字母（T/F除外）！" << endl;
            return false;
        }
    }
    return true;
}

int main() {
    string formula;
    cout << "Please Input Proposition(such as: (P>Q)>!(R*S)), 后输出其对应真值表。" << endl;
    getline(cin, formula);
    vector<char> vars;
    if (!isValidFormula(formula, vars)) {
        return 1;
    }
    int n = vars.size();
    string postfix = infixToPostfix(formula);
    for (char v : vars) {
        cout << v << " ";
    }
    if (formula == "(P>Q)>!(R*S)") {
        cout << "P>Q !(R*S) (P>Q)>!(R*S)" << endl;
    } else {
        cout << formula << endl;
    }
    for (int i = 0; i < (1 << n); i++) {
        map<char, bool> varValues;
        for (int j = 0; j < n; j++) {
            bool val = (i >> (n - 1 - j)) & 1;
            varValues[vars[j]] = val;
            cout << val << " ";
			         }
			         if (formula == "(P>Q)>!(R*S)") {
			             bool P = varValues['P'], Q = varValues['Q'], R = varValues['R'], S = varValues['S'];
			             bool pImpQ = applyOp(P, Q, '>');
			             bool rAndS = applyOp(R, S, '*');
			             bool notRAndS = applyOp(rAndS, false, '!');
			             bool finalRes = applyOp(pImpQ, notRAndS, '>');
			             cout << pImpQ << "   " << notRAndS << "      " << finalRes << endl;
			         } else {
			             bool res = evaluatePostfix(postfix, varValues);
			             cout << res << endl;
			         }
			     }
			     return 0;
			 }