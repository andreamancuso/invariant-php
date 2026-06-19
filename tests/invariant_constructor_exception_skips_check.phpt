--TEST--
Invariant is skipped when constructor throws
--EXTENSIONS--
invariant_php
--FILE--
<?php
class ThrowingConstructor {
    private int $value = -1;

    public function __construct() {
        echo "construct\n";
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
    new ThrowingConstructor();
} catch (\RuntimeException $e) {
    echo "caught {$e->getMessage()}\n";
}
?>
--EXPECT--
construct
caught boom
