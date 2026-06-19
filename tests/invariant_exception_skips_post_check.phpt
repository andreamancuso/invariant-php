--TEST--
Invariant is skipped when a public method throws
--EXTENSIONS--
invariant_php
--FILE--
<?php
class ThrowingOperation {
    private int $value = 1;

    public function fail(): void {
        $this->value = -1;
        throw new \RuntimeException("boom");
    }

    public function __invariant(): void {
        echo "invariant\n";

        if ($this->value < 0) {
            throw new \LogicException("invalid");
        }
    }
}

try {
    (new ThrowingOperation())->fail();
} catch (\RuntimeException $e) {
    echo "caught {$e->getMessage()}\n";
}
?>
--EXPECT--
caught boom
