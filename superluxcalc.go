package main

import (
    "bufio"
    "fmt"
    "math"
    "os"
    "strings"
    "unicode"
)

type Token struct {
    typ  string
    val  string
    num  float64
}

func tokenize(s string) []Token {
    var tokens []Token
    for i := 0; i < len(s); {
        c := s[i]
        if unicode.IsSpace(rune(c)) {
            i++
            continue
        }
        if (c >= '0' && c <= '9') || c == '.' {
            j := i
            dotSeen := false
            for j < len(s) {
                ch := s[j]
                if ch == '.' {
                    if dotSeen { break }
                    dotSeen = true
                    j++
                    continue
                }
                if ch >= '0' && ch <= '9' {
                    j++
                    continue
                }
                break
            }
            n := s[i:j]
            var f float64
            _, _ = fmt.Sscanf(n, "%f", &f)
            tokens = append(tokens, Token{typ: "num", val: n, num: f})
            i = j
            continue
        }
        if unicode.IsLetter(rune(c)) {
            j := i
            for j < len(s) && unicode.IsLetter(rune(s[j])) {
                j++
            }
            tokens = append(tokens, Token{typ: "func", val: s[i:j]})
            i = j
            continue
        }
        if c == '(' || c == ')' {
            tokens = append(tokens, Token{typ: "par", val: string(c)})
            i++
            continue
        }
        if strings.ContainsRune("+-*/^", rune(c)) {
            tokens = append(tokens, Token{typ: "op", val: string(c)})
            i++
            continue
        }
        i++
    }
    return tokens
}

func precedence(op string) int {
    switch op {
    case "+", "-":
        return 1
    case "*", "/":
        return 2
    case "^":
        return 3
    default:
        return 0
    }
}

func rightAssoc(op string) bool { return op == "^" }

func applyOp(op string, a, b float64) float64 {
    switch op {
    case "+": return a + b
    case "-": return a - b
    case "*": return a * b
    case "/": return a / b
    case "^": return math.Pow(a, b)
    }
    panic("unsupported op")
}

func callFunc(fn string, args []float64) float64 {
    switch fn {
    case "sin": return math.Sin(args[0])
    case "cos": return math.Cos(args[0])
    case "tan": return math.Tan(args[0])
    case "sqrt": return math.Sqrt(args[0])
    case "log": return math.Log10(args[0])
    case "ln": return math.Log(args[0])
    case "abs": return math.Abs(args[0])
    case "pow": return math.Pow(args[0], args[1])
    case "pi": return math.Pi
    case "e": return math.E
    }
    panic("unsupported function")
}

func evalExpr(expr string) float64 {
    tokens := tokenize(expr)
    var output []Token
    var ops []Token

    for _, tk := range tokens {
        switch tk.typ {
        case "num":
            output = append(output, tk)
        case "func":
            ops = append(ops, tk)
        case "op":
            for len(ops) > 0 && ops[len(ops)-1].typ == "op" {
                o1, o2 := tk.val, ops[len(ops)-1].val
                if (!rightAssoc(o1) && precedence(o1) <= precedence(o2)) || (rightAssoc(o1) && precedence(o1) < precedence(o2)) {
                    output = append(output, ops[len(ops)-1])
                    ops = ops[:len(ops)-1]
                } else {
                    break
                }
            }
            ops = append(ops, tk)
        case "par":
            if tk.val == "(" {
                ops = append(ops, tk)
            } else {
                for len(ops) > 0 && !(ops[len(ops)-1].typ == "par" && ops[len(ops)-1].val == "(") {
                    output = append(output, ops[len(ops)-1])
                    ops = ops[:len(ops)-1]
                }
                if len(ops) == 0 {
                    panic("mismatched parentheses")
                }
                ops = ops[:len(ops)-1]
                if len(ops) > 0 && ops[len(ops)-1].typ == "func" {
                    output = append(output, ops[len(ops)-1])
                    ops = ops[:len(ops)-1]
                }
            }
        }
    }

    for len(ops) > 0 {
        if ops[len(ops)-1].typ == "par" {
            panic("mismatched parentheses")
        }
        output = append(output, ops[len(ops)-1])
        ops = ops[:len(ops)-1]
    }

    var stack []float64
    for _, tk := range output {
        switch tk.typ {
        case "num":
            stack = append(stack, tk.num)
        case "op":
            if len(stack) < 2 { panic("not enough operands") }
            b := stack[len(stack)-1]
            a := stack[len(stack)-2]
            stack = stack[:len(stack)-2]
            stack = append(stack, applyOp(tk.val, a, b))
        case "func":
            if tk.val == "pow" {
                if len(stack) < 2 { panic("pow requires 2 args") }
                b := stack[len(stack)-1]
                a := stack[len(stack)-2]
                stack = stack[:len(stack)-2]
                stack = append(stack, callFunc(tk.val, []float64{a, b}))
            } else if tk.val == "pi" || tk.val == "e" {
                stack = append(stack, callFunc(tk.val, nil))
            } else {
                if len(stack) < 1 { panic("not enough operands") }
                a := stack[len(stack)-1]
                stack = stack[:len(stack)-1]
                stack = append(stack, callFunc(tk.val, []float64{a}))
            }
        }
    }

    if len(stack) != 1 {
        panic("invalid expression")
    }
    return stack[0]
}

func main() {
    fmt.Println("SuperLuxCalc Go")
    reader := bufio.NewReader(os.Stdin)
    for {
        fmt.Print("calc> ")
        line, err := reader.ReadString('\n')
        if err != nil {
            break
        }
        line = strings.TrimSpace(line)
        if line == "" {
            continue
        }
        if strings.EqualFold(line, "exit") || strings.EqualFold(line, "quit") {
            break
        }

        func() {
            defer func() {
                if r := recover(); r != nil {
                    fmt.Println("Ошибка:", r)
                }
            }()
            result := evalExpr(line)
            fmt.Println("=", result)
        }()
    }
}
