--TEST--
Uninitialized typed properties are enforced even without __invariant()
--EXTENSIONS--
invariant_php
--INI--
display_errors=1
log_errors=0
error_reporting=E_ALL
--FILE--
<?php
class NoInvariantUninitialized {
    private int $value;

    public function run(): void {
        echo "run\n";
    }
}

(new NoInvariantUninitialized())->run();
?>
--EXPECTF--
run

Fatal error: Invariant check failed: Uninitialized property 'value' in class 'NoInvariantUninitialized' in %s on line %d
