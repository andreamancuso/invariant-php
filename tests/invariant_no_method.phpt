--TEST--
Ensure class without __invariant() does not trigger errors
--EXTENSIONS--
invariant_php
--FILE--
<?php
class NoInvariant {
    function doSomething() {
        echo "Doing something...\n";
    }
}

$obj = new NoInvariant();
$obj->doSomething();
?>
--EXPECT--
Doing something...
