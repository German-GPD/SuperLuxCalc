<?php
// SuperLuxCalc PHP CLI
function evalSafe($expr) {
    $expr = trim($expr);
    if ($expr === '') {
        throw new Exception('Пустое выражение');
    }
    $expr = str_replace('^', '**', $expr);
    if (!preg_match('/^[0-9+\-*/().,\sA-Za-z_]+$/', $expr)) {
        throw new Exception('Недопустимые символы');
    }

    $allowed = ['sin', 'cos', 'tan', 'sqrt', 'log', 'ln', 'pow', 'abs', 'pi', 'e'];
    foreach ($allowed as $fn) {
        $expr = preg_replace('/\b' . $fn . '\b/i', ' ' . $fn . ' ', $expr);
    }

    // Light validation: if someone writes unsupported function names, it will fail.
    $result = null;
    eval('$result = ' . $expr . ';');
    if (!is_numeric($result)) {
        throw new Exception('Невалидный результат');
    }
    return (float)$result;
}

echo "SuperLuxCalc PHP CLI\n";
while (true) {
    echo "calc> ";
    $line = trim(fgets(STDIN));
    if ($line === false) {
        break;
    }
    if ($line === '') {
        continue;
    }
    if (strtolower($line) === 'exit' || strtolower($line) === 'quit') {
        break;
    }

    try {
        $res = evalSafe($line);
        echo "= $res\n";
    } catch (Throwable $e) {
        echo "Ошибка: " . $e->getMessage() . "\n";
    }
}
