#!/usr/bin/env python3
# SuperLuxCalc Python CLI
import math
import ast
import operator

ALLOWED_NAMES = {
    'pi': math.pi,
    'e': math.e,
    'sin': math.sin,
    'cos': math.cos,
    'tan': math.tan,
    'sqrt': math.sqrt,
    'log': math.log10,
    'ln': math.log,
    'abs': abs,
    'pow': pow,
}

OPS = {
    ast.Add: operator.add,
    ast.Sub: operator.sub,
    ast.Mult: operator.mul,
    ast.Div: operator.truediv,
    ast.Pow: operator.pow,
    ast.Mod: operator.mod,
    ast.USub: operator.neg,
    ast.UAdd: operator.pos,
}


def safe_eval(expr: str):
    node = ast.parse(expr, mode='eval')
    return _eval(node.body)


def _eval(node):
    if isinstance(node, ast.Constant):
        return node.value
    if isinstance(node, ast.Num):
        return node.n
    if isinstance(node, ast.BinOp):
        left = _eval(node.left)
        right = _eval(node.right)
        op = OPS.get(type(node.op))
        if op is None:
            raise ValueError(f'Unsupported operator: {type(node.op).__name__}')
        return op(left, right)
    if isinstance(node, ast.UnaryOp):
        operand = _eval(node.operand)
        op = OPS.get(type(node.op))
        if op is None:
            raise ValueError(f'Unsupported unary operator: {type(node.op).__name__}')
        return op(operand)
    if isinstance(node, ast.Call):
        if not isinstance(node.func, ast.Name):
            raise ValueError('Only direct function calls are allowed')
        func_name = node.func.id
        if func_name not in ALLOWED_NAMES:
            raise ValueError(f'Function not allowed: {func_name}')
        args = [_eval(arg) for arg in node.args]
        return ALLOWED_NAMES[func_name](*args)
    if isinstance(node, ast.Name):
        if node.id not in ALLOWED_NAMES:
            raise ValueError(f'Unknown name: {node.id}')
        return ALLOWED_NAMES[node.id]
    raise ValueError(f'Unsupported syntax: {type(node).__name__}')


def main():
    print('SuperLuxCalc Python CLI')
    print('Type expression or "exit" to quit.')
    while True:
        try:
            expr = input('calc> ').strip()
        except (EOFError, KeyboardInterrupt):
            print()
            break

        if not expr:
            continue
        if expr.lower() in {'exit', 'quit'}:
            break

        try:
            result = safe_eval(expr)
            print('= ', result)
        except Exception as exc:
            print('Ошибка:', exc)


if __name__ == '__main__':
    main()
