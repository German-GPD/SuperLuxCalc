#include <bits/stdc++.h>
using namespace std;

static bool isDigit(char c) { return c >= '0' && c <= '9'; }

struct Token {
    string type;
    string value;
    double num = 0;
};

int precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

bool isRightAssoc(const string& op) { return op == "^"; }

double applyOp(const string& op, double a, double b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    if (op == "^") return pow(a, b);
    throw runtime_error("Unsupported operator");
}

double callFunc(const string& fn, const vector<double>& args) {
    if (fn == "sin") return sin(args[0]);
    if (fn == "cos") return cos(args[0]);
    if (fn == "tan") return tan(args[0]);
    if (fn == "sqrt") return sqrt(args[0]);
    if (fn == "log") return log10(args[0]);
    if (fn == "ln") return log(args[0]);
    if (fn == "abs") return fabs(args[0]);
    if (fn == "pow") return pow(args[0], args[1]);
    if (fn == "pi") return M_PI;
    if (fn == "e") return M_E;
    throw runtime_error("Unknown function");
}

vector<Token> tokenize(const string& expr) {
    vector<Token> tokens;
    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        if (isspace((unsigned char)c)) continue;

        if (isDigit(c) || c == '.') {
            size_t j = i;
            bool hasDot = false;
            while (j < expr.size()) {
                if (expr[j] == '.') {
                    if (hasDot) break;
                    hasDot = true;
                    ++j;
                    continue;
                }
                if (isDigit(expr[j])) {
                    ++j;
                    continue;
                }
                break;
            }
            Token t; t.type = "number"; t.value = expr.substr(i, j - i); t.num = stod(t.value);
            tokens.push_back(t);
            i = j - 1;
            continue;
        }

        if (isalpha((unsigned char)c)) {
            size_t j = i;
            while (j < expr.size() && isalpha((unsigned char)expr[j])) ++j;
            Token t; t.type = "func"; t.value = expr.substr(i, j - i);
            tokens.push_back(t);
            i = j - 1;
            continue;
        }

        if (c == '(' || c == ')') {
            Token t; t.type = "par"; t.value = string(1, c);
            tokens.push_back(t);
            continue;
        }

        if (c == ',' ) {
            Token t; t.type = "comma"; t.value = ",";
            tokens.push_back(t);
            continue;
        }

        if (string("+-*/^").find(c) != string::npos) {
            Token t; t.type = "op"; t.value = string(1, c);
            tokens.push_back(t);
            continue;
        }

        throw runtime_error("Unexpected character");
    }
    return tokens;
}

double evaluateExpression(const string& expr) {
    vector<Token> tokens = tokenize(expr);
    vector<Token> output;
    vector<Token> ops;

    auto pushOutput = [&](Token t) { output.push_back(t); };

    for (const auto& tok : tokens) {
        if (tok.type == "number") {
            output.push_back(tok);
        } else if (tok.type == "func") {
            ops.push_back(tok);
        } else if (tok.type == "comma") {
            while (!ops.empty() && !(ops.back().type == "par" && ops.back().value == "(")) {
                output.push_back(ops.back());
                ops.pop_back();
            }
        } else if (tok.type == "op") {
            while (!ops.empty() && ops.back().type == "op") {
                const string o2 = ops.back().value;
                const string o1 = tok.value;
                if ((!isRightAssoc(o1) && precedence(o1) <= precedence(o2)) ||
                    (isRightAssoc(o1) && precedence(o1) < precedence(o2))) {
                    output.push_back(ops.back());
                    ops.pop_back();
                } else {
                    break;
                }
            }
            ops.push_back(tok);
        } else if (tok.type == "par") {
            if (tok.value == "(") {
                ops.push_back(tok);
            } else {
                while (!ops.empty() && !(ops.back().type == "par" && ops.back().value == "(")) {
                    output.push_back(ops.back());
                    ops.pop_back();
                }
                if (ops.empty()) throw runtime_error("Mismatched parentheses");
                ops.pop_back();
                if (!ops.empty() && ops.back().type == "func") {
                    output.push_back(ops.back());
                    ops.pop_back();
                }
            }
        }
    }

    while (!ops.empty()) {
        if (ops.back().type == "par") throw runtime_error("Mismatched parentheses");
        output.push_back(ops.back());
        ops.pop_back();
    }

    vector<double> stack;
    for (const auto& tok : output) {
        if (tok.type == "number") {
            stack.push_back(tok.num);
        } else if (tok.type == "op") {
            if (stack.size() < 2) throw runtime_error("Not enough operands");
            double b = stack.back(); stack.pop_back();
            double a = stack.back(); stack.pop_back();
            stack.push_back(applyOp(tok.value, a, b));
        } else if (tok.type == "func") {
            if (tok.value == "pow") {
                if (stack.size() < 2) throw runtime_error("pow requires two arguments");
                double b = stack.back(); stack.pop_back();
                double a = stack.back(); stack.pop_back();
                stack.push_back(callFunc(tok.value, {a, b}));
            } else if (tok.value == "pi" || tok.value == "e") {
                stack.push_back(callFunc(tok.value, {}));
            } else {
                if (stack.empty()) throw runtime_error("Function needs an argument");
                double a = stack.back(); stack.pop_back();
                stack.push_back(callFunc(tok.value, {a}));
            }
        }
    }

    if (stack.size() != 1) throw runtime_error("Invalid expression");
    return stack.back();
}

int main() {
    cout << "SuperLuxCalc C++ REPL\n";
    cout << "Examples: 2 + 3 * 4, sqrt(16), sin(1.57), 2^8\n";
    cout << "Type 'exit' to quit\n";

    string line;
    while (true) {
        cout << "calc> ";
        getline(cin, line);
        if (!cin) break;

        if (line == "exit" || line == "quit") break;
        if (line.empty()) continue;

        try {
            string expr = line;
            replace(expr.begin(), expr.end(), '^', '*');
            // simple correction: if user types 2^8, we convert to pow(2,8) for this parser
            if (expr.find('^') != string::npos) {
                // fallback handled by tokenizer not by char replace, so just keep as is
            }
            cout << "= " << evaluateExpression(line) << "\n";
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return 0;
}
