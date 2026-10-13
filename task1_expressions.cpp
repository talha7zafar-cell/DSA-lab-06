#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// stack for characters (operators)
class CharStack {
    char arr[100];
    int top;
public:
    CharStack() { top = -1; }
    void push(char c) {
        if (top == 99) { cout << "Stack overflow" << endl; return; }
        top++;
        arr[top] = c;
    }
    char pop() {
        if (top == -1) return '\0';
        char c = arr[top];
        top--;
        return c;
    }
    char peek() {
        if (top == -1) return '\0';
        return arr[top];
    }
    bool isEmpty() { return top == -1; }
};

// stack for strings (used in postfix to infix)
class StringStack {
    string arr[100];
    int top;
public:
    StringStack() { top = -1; }
    void push(string s) {
        if (top == 99) { cout << "Stack overflow" << endl; return; }
        top++;
        arr[top] = s;
    }
    string pop() {
        if (top == -1) return "";
        string s = arr[top];
        top--;
        return s;
    }
    bool isEmpty() { return top == -1; }
};

// stack for numbers (used in evaluation)
class NumStack {
    double arr[100];
    int top;
public:
    NumStack() { top = -1; }
    void push(double x) {
        if (top == 99) { cout << "Stack overflow" << endl; return; }
        top++;
        arr[top] = x;
    }
    double pop() {
        if (top == -1) return 0;
        double x = arr[top];
        top--;
        return x;
    }
    bool isEmpty() { return top == -1; }
};

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int precedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

double calculate(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    if (op == '^') return pow(a, b);
    return 0;
}

string infix_to_postfix(string expr) {
    CharStack s;
    string result = "";

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];

        if (c == ' ') continue;

        if (isalnum(c)) {
            result += c;              // operand goes directly to output
        }
        else if (c == '(') {
            s.push(c);
        }
        else if (c == ')') {
            while (!s.isEmpty() && s.peek() != '(')
                result += s.pop();
            s.pop();                  // remove '('
        }
        else if (isOperator(c)) {
            // pop operators with higher or equal precedence
            // (^ is right to left so we dont pop ^ for ^)
            while (!s.isEmpty() && precedence(s.peek()) >= precedence(c)) {
                if (c == '^' && s.peek() == '^') break;
                result += s.pop();
            }
            s.push(c);
        }
    }

    while (!s.isEmpty())
        result += s.pop();

    return result;
}

string infix_to_prefix(string expr) {
    // step 1: reverse the expression and swap ( with )
    string rev = "";
    for (int i = expr.length() - 1; i >= 0; i--) {
        if (expr[i] == '(') rev += ')';
        else if (expr[i] == ')') rev += '(';
        else rev += expr[i];
    }

    // step 2: get postfix of reversed expression
    CharStack s;
    string result = "";
    for (int i = 0; i < rev.length(); i++) {
        char c = rev[i];

        if (c == ' ') continue;

        if (isalnum(c)) {
            result += c;
        }
        else if (c == '(') {
            s.push(c);
        }
        else if (c == ')') {
            while (!s.isEmpty() && s.peek() != '(')
                result += s.pop();
            s.pop();
        }
        else if (isOperator(c)) {
            // here we only pop strictly higher precedence (except ^)
            while (!s.isEmpty() && (precedence(s.peek()) > precedence(c) ||
                   (c == '^' && s.peek() == '^'))) {
                result += s.pop();
            }
            s.push(c);
        }
    }
    while (!s.isEmpty())
        result += s.pop();

    // step 3: reverse the result
    string prefix = "";
    for (int i = result.length() - 1; i >= 0; i--)
        prefix += result[i];

    return prefix;
}

string postfix_to_infix(string expr) {
    StringStack s;

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;

        if (isalnum(c)) {
            s.push(string(1, c));
        }
        else if (isOperator(c)) {
            string b = s.pop();
            string a = s.pop();
            s.push("(" + a + c + b + ")");
        }
    }
    return s.pop();
}

double evaluate_postfix(string expr) {
    NumStack s;

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;

        if (isdigit(c)) {
            s.push(c - '0');          // convert char to number
        }
        else if (isOperator(c)) {
            double b = s.pop();       // second operand comes out first
            double a = s.pop();
            s.push(calculate(a, b, c));
        }
    }
    return s.pop();
}

double evaluate_prefix(string expr) {
    NumStack s;

    // prefix is read from right to left
    for (int i = expr.length() - 1; i >= 0; i--) {
        char c = expr[i];
        if (c == ' ') continue;

        if (isdigit(c)) {
            s.push(c - '0');
        }
        else if (isOperator(c)) {
            double a = s.pop();       // here first operand comes out first
            double b = s.pop();
            s.push(calculate(a, b, c));
        }
    }
    return s.pop();
}

int main() {
    string exp1 = "A+B*C";
    string exp2 = "(A+B)*(C-D)";
    string exp3 = "A^B^C";

    cout << "Infix: " << exp1 << endl;
    cout << "Postfix: " << infix_to_postfix(exp1) << endl;
    cout << "Prefix: " << infix_to_prefix(exp1) << endl << endl;

    cout << "Infix: " << exp2 << endl;
    cout << "Postfix: " << infix_to_postfix(exp2) << endl;
    cout << "Prefix: " << infix_to_prefix(exp2) << endl << endl;

    cout << "Infix: " << exp3 << endl;
    cout << "Postfix: " << infix_to_postfix(exp3) << endl;
    cout << "Prefix: " << infix_to_prefix(exp3) << endl << endl;

    cout << "Postfix AB+C* to infix: " << postfix_to_infix("AB+C*") << endl << endl;

    string num = "(5+3)*(8-4)/2";
    string post = infix_to_postfix(num);
    string pre = infix_to_prefix(num);
    cout << "Infix: " << num << endl;
    cout << "Postfix: " << post << "   Value = " << evaluate_postfix(post) << endl;
    cout << "Prefix: " << pre << "   Value = " << evaluate_prefix(pre) << endl << endl;

    // user input
    string input;
    cout << "Enter an infix expression (single digit numbers or letters): ";
    getline(cin, input);
    cout << "Postfix: " << infix_to_postfix(input) << endl;
    cout << "Prefix: " << infix_to_prefix(input) << endl;

    return 0;
}
