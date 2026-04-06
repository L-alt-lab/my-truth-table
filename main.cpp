#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iomanip>
using namespace std;

// 运算符优先级
int precedence(char op) {
    if (op == '!') return 4;
    if (op == '*') return 3;
    if (op == '+') return 2;
    if (op == '>') return 1;
    if (op == '~') return 0;
    return -1;
}

// 执行运算
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

// 中缀转后缀（逆波兰）
string infixToPostfix(string s) {
    stack<char> st;
    string result;
    for (char c : s) {
        if (isalpha(c)) {
            result += c;
        } else if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                st.pop();
            }
            st.pop();
        } else {
            while (!st.empty() && precedence(c) <= precedence(st.top())) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    return result;
}

// 计算后缀表达式
bool evaluatePostfix(string postfix, map<char, bool> values) {
    stack<bool> st;
    for (char c : postfix) {
        if (isalpha(c)) {
            st.push(values[c]);
        } else if (c == '!') {
            bool a = st.top(); st.pop();
            st.push(applyOp(a, false, c));
        } else {
            bool b = st.top(); st.pop();
            bool a = st.top(); st.pop();
            st.push(applyOp(a, b, c));
        }
    }
    return st.top();
}

int main() {
    string formula;
    cout << "请输入命题公式（支持!非、*与、+或、>蕴含、~等价）：";
    getline(cin, formula);

    // 提取变元
    vector<char> vars;
    map<char, bool> varExists;
    for (char c : formula) {
        if (isalpha(c) && !varExists[c]) {
            varExists[c] = true;
            vars.push_back(c);
        }
    }
    int n = vars.size();
    if (n > 4) {
        cout << "暂不支持超过4个变元！" << endl;
        return 0;
    }

    string postfix = infixToPostfix(formula);

    // 打印表头
    for (char v : vars) {
        cout << setw(4) << v << " |";
    }
    cout << " 结果" << endl;
    for (int i = 0; i < n; i++) {
        cout << "-----|";
    }
    cout << "------" << endl;

    // 生成所有真值组合
    for (int i = 0; i < (1 << n); i++) {
        map<char, bool> values;
        for (int j = 0; j < n; j++) {
            values[vars[j]] = (i >> (n - 1 - j)) & 1;
            cout << setw(4) << values[vars[j]] << " |";
        }
        bool res = evaluatePostfix(postfix, values);
        cout << setw(4) << res << endl;
    }

    return 0;
}