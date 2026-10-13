#!/usr/bin/env node
// SuperLuxCalc JavaScript CLI
const rl = require('readline').createInterface({
  input: process.stdin,
  output: process.stdout
});

const safe = {
  sin: Math.sin,
  cos: Math.cos,
  tan: Math.tan,
  sqrt: Math.sqrt,
  log: Math.log10,
  ln: Math.log,
  pow: Math.pow,
  abs: Math.abs,
  pi: Math.PI,
  e: Math.E
};

function evaluateExpression(expr) {
  const cleaned = expr.trim();
  if (!cleaned) return null;
  const sanitized = cleaned.replace(/\^/g, '**');
  if (/[^0-9+\-*/().,\s*^A-Za-z_]/.test(sanitized)) {
    throw new Error('Недопустимые символы');
  }
  const fn = new Function(...Object.keys(safe), 'return (' + sanitized + ');');
  const result = fn(...Object.values(safe));
  if (typeof result !== 'number' || !isFinite(result)) {
    throw new Error('Неверный результат');
  }
  return result;
}

console.log('SuperLuxCalc JavaScript CLI');
console.log('Введите выражение или "exit" для выхода.');

function ask() {
  rl.question('calc> ', (input) => {
    const value = input.trim();
    if (!value) {
      ask();
      return;
    }
    if (value.toLowerCase() === 'exit' || value.toLowerCase() === 'quit') {
      rl.close();
      return;
    }
    try {
      const result = evaluateExpression(value);
      console.log('= ', result);
    } catch (err) {
      console.log('Ошибка:', err.message);
    }
    ask();
  });
}

ask();
