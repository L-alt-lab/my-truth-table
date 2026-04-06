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
    if (op == '!') return 5; 
    if (op == '*') return 4; 
    if (op == '+') return 3; 
    if (op == '>') return 2; 
    if (op == '=') return 1; 
    return -1;
}
bool applyOp(bool a, bool b, char op) {
    switch(op) {
        case '!': return !a;          
        case '*': return a && b;    
        case '+': return a || b;   
        case '>': return !a || b;  
        case '=': return a == b; 
        default: return false;
    }
}
string infixToPostfix(const string& s) {
    stack<char> opStack;
    string postfix;
    for (char c : s) {
        if (isalpha(c) && c != 'T' && c != 'F') {
            postfix += c;
        } else if (c == 'T' || c == 'F') {
            postfix += c;
        } else if (c == '(') {
            opStack.push(c);
        } else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop();
        } else {
            if (c == '!') {
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
    }
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    return postfix;
}
bool evaluate(const string& postfix, const map<char, bool>& vars) {
    stack<bool> valStack;
    for (char c : postfix) {
        if (isalpha(c) && c != 'T' && c != 'F') {
            valStack.push(vars.at(c));
        } else if (c == 'T') valStack.push(true);
        else if (c == 'F') valStack.push(false);
        else if (c == '!') {
            bool a = valStack.top(); valStack.pop();
            valStack.push(applyOp(a, false, '!'));
        } else {
            bool b = valStack.top(); valStack.pop();
            bool a = valStack.top(); valStack.pop();
            valStack.push(applyOp(a, b, c));
        }
    }
    return valStack.top();
}
vector<char> getVars(const string& s) {
    vector<char> res;
    map<char, bool> ok;
    for (char c : s) {
        if (isalpha(c) && c != 'T' && c != 'F' && !ok[c]) {
            ok[c] = true;
            res.push_back(c);
        }
    }
    return res;
}
void parseFormula(const string& input, vector<string>& premises, string& conclusion) {
    size_t pos = 0;
    size_t prev = 0;
    while ((pos = input.find(';', prev)) != string::npos) {
        if (pos > prev) {
            premises.push_back(input.substr(prev, pos - prev));
        }
        prev = pos + 1;
    }
    if (prev < input.size()) {
        conclusion = input.substr(prev);
    }
}
int main() {
    string input;
    cout << "Please Input Proposition(such as: P>!Q;!R+Q;R*S;S=!P)" << endl;
    getline(cin, input);
    vector<string> premises;
    string conclusion;
    parseFormula(input, premises, conclusion);
    vector<char> vars = getVars(input);
    int n = vars.size();
    if (n > 4) { cout << "Too many vars!" << endl; return 1; }
    cout << left;
    for (char v : vars) cout << setw(6) << v;
    cout << setw(6) << "P>!Q" << setw(8) << "!R+Q" << setw(8) << "R*S" << setw(12) << "P>!Q;!R+Q;R*S=>!P" << endl;
    bool is_valid = true;
    for (int i = 0; i < (1 << n); i++) {
        map<char, bool> val;
        for (int j = 0; j < n; j++) {
            val[vars[j]] = (i >> (n - 1 - j)) & 1;
        }
        for (char v : vars) cout << setw(6) << (int)val[v];
        bool res1 = evaluate(infixToPostfix(premises[0]), val); 
        bool res2 = evaluate(infixToPostfix(premises[1]), val); 
        bool res3 = evaluate(infixToPostfix(premises[2]), val); 
        cout << setw(6) << (int)res1 << setw(8) << (int)res2 << setw(8) << (int)res3;
        bool P = val['P'];
        bool final_expr = applyOp(res1 && res2 && res3, !P, '>');
        cout << setw(12) << (int)final_expr << endl;
        if (!final_expr) is_valid = false;
		     }
		     cout << "\n推理结果：" << (is_valid ? "P是前提的有效结论（全T）" : "P不是有效结论（有F）") << endl;
		     return 0;
		 }