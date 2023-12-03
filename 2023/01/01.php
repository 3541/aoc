<?php

function n($total, $line) {
    foreach (str_split($line) as $c) {
        if (is_numeric($c)) {
            $total += 10 * $c;
            break;
        }
    }

    foreach (array_reverse(str_split($line)) as $c) {
        if (is_numeric($c)) {
            $total += $c;
            break;
        }
    }

    return $total;
}

function first($line, $rev) {
    $numbers = array(
        "one" => 1,
        "two" => 2,
        "three" => 3,
        "four" => 4,
        "five" => 5,
        "six" => 6,
        "seven" => 7,
        "eight" => 8,
        "nine" => 9
    );

    for ($i = 0; $i < strlen($line); ++$i) {
        foreach ($numbers as $s => $n) {
            $c = $rev ? strrev($s) : $s;
            if ($c == substr($line, $i, strlen($s))) {
                return $n;
            }

            if (is_numeric($line[$i])) {
                return $line[$i];
            }
        }
    }
}

function n2($total, $line) {
    return $total + 10 * first($line, false) + first(strrev($line), true);
}

$lines = explode(PHP_EOL, file_get_contents($argv[1]));
echo array_reduce($lines, "n"), "\n";
echo array_reduce($lines, "n2"), "\n";

?>
