--TEST--
Module info exposes invariant_php support and behavior summary
--EXTENSIONS--
invariant_php
--FILE--
<?php
ob_start();
phpinfo(INFO_MODULES);
$info = ob_get_clean();

foreach ([
    'invariant_php support => enabled',
    'Version => ',
    'Invariant checks => after successful outermost public object method calls',
    'Typed property initialization checks => enabled',
] as $expected) {
    echo str_contains($info, $expected) ? "found\n" : "missing {$expected}\n";
}
?>
--EXPECT--
found
found
found
found
