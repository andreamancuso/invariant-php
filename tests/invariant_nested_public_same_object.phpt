--TEST--
Nested same-object public calls do not trigger intermediate invariant checks
--EXTENSIONS--
invariant_php
--FILE--
<?php
class NestedPublicCall {
    private int $value = 1;

    public function outer(): void {
        $this->value = -1;
        $this->inner();
        $this->value = 1;
        echo "outer done\n";
    }

    public function inner(): void {
        echo "inner\n";
    }

    public function __invariant(): void {
        if ($this->value < 0) {
            throw new \LogicException("invalid");
        }

        echo "invariant\n";
    }
}

(new NestedPublicCall())->outer();
?>
--EXPECT--
inner
outer done
invariant
