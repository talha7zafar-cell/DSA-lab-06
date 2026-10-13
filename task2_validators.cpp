#include <iostream>
#include <string>
using namespace std;

// stack for brackets
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
    bool isEmpty() { return top == -1; }
};

// stack for tag names
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
    string peek() {
        if (top == -1) return "";
        return arr[top];
    }
    bool isEmpty() { return top == -1; }
};

// ---------- Part A ----------
bool isValidBrackets(string s) {
    CharStack st;

    for (int i = 0; i < s.length(); i++) {
        char c = s[i];

        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        }
        else if (c == ')' || c == '}' || c == ']') {
            if (st.isEmpty()) return false;   // nothing to match with

            char open = st.pop();
            if (c == ')' && open != '(') return false;
            if (c == '}' && open != '{') return false;
            if (c == ']' && open != '[') return false;
        }
    }
    // if something is left in stack it was never closed
    return st.isEmpty();
}

// ---------- Part B ----------
// returns true/false, errors are stored in the errors array
bool validate_markup(string s, string errors[], int &errorCount) {
    StringStack st;
    errorCount = 0;
    int i = 0;

    while (i < s.length()) {
        if (s[i] == '<') {
            int j = s.find('>', i);
            if (j == -1) {
                errors[errorCount++] = "Tag not closed with >";
                break;
            }

            string tag = s.substr(i + 1, j - i - 1);   // text inside < >

            if (tag[0] == '!') {
                // comment or doctype, ignore it
            }
            else if (tag[0] == '/') {
                // closing tag
                string name = tag.substr(1);
                if (st.isEmpty()) {
                    errors[errorCount++] = "</" + name + "> has no opening tag";
                }
                else if (st.peek() != name) {
                    errors[errorCount++] = "Expected </" + st.peek() + "> but found </" + name + ">";
                    st.pop();
                }
                else {
                    st.pop();   // correct match
                }
            }
            else if (tag[tag.length() - 1] == '/') {
                // self closing tag like <br/>, nothing to push
            }
            else {
                // opening tag, take name before any space (ignore attributes)
                string name = "";
                for (int k = 0; k < tag.length() && tag[k] != ' '; k++)
                    name += tag[k];
                st.push(name);
            }
            i = j + 1;
        }
        else {
            i++;
        }
    }

    // tags left in stack were never closed
    while (!st.isEmpty()) {
        errors[errorCount++] = "<" + st.pop() + "> is not closed";
    }

    return errorCount == 0;
}

void testMarkup(string s) {
    string errors[50];
    int count;
    bool valid = validate_markup(s, errors, count);

    cout << s << endl;
    if (valid) cout << "Valid" << endl;
    else {
        cout << "Invalid" << endl;
        for (int i = 0; i < count; i++)
            cout << "  - " << errors[i] << endl;
    }
    cout << endl;
}

int main() {
    cout << "Part A: Bracket Validator" << endl;
    string tests[] = {"()", "{[()]}", "([)]", "((()", "a+(b*c)-{d/[e]}", "}"};
    for (int i = 0; i < 6; i++) {
        cout << tests[i] << " -> ";
        if (isValidBrackets(tests[i])) cout << "Valid" << endl;
        else cout << "Invalid" << endl;
    }

    cout << endl << "Part B: HTML/XML Validator" << endl;
    testMarkup("<html><body><p>Hello</p></body></html>");
    testMarkup("<div><p>Text</div></p>");
    testMarkup("<a><b></b>");
    testMarkup("</p>");
    testMarkup("<note><to>Ali</to><br/></note>");
    testMarkup("<div class=\"box\"><span>Hi</span></div>");

    return 0;
}
