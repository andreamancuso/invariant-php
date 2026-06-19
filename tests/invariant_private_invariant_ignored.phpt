--TEST--
Private __invariant() methods are ignored
--EXTENSIONS--
invariant_php
--FILE--
<?php
class PrivateInvariantMethod {
    private int $value = -1;

    public function run(): void {
        echo "run\n";
    }

    private function __invariant(): void {
        echo "private invariant\n";

        if ($this->value < 0) {
            throw new \LogicException("invalid");
        }
    }
}

(new PrivateInvariantMethod())->run();
?>
--EXPECT--
run
