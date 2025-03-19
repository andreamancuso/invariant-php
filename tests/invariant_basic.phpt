--TEST--
Basic invariant test
--EXTENSIONS--
invariant_php
--FILE--
<?php
class Example {
    function __invariant() {
        echo "Invariant check executed!\n";
    }

    function doSomething() {
        echo "Doing something...\n";
    }
}

$obj = new Example();
$obj->doSomething();
?>
--EXPECT--
Invariant check executed!
Doing something...
Invariant check executed!
