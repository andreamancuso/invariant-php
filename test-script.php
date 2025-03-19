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
