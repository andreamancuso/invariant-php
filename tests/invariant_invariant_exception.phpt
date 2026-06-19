--TEST--
Exceptions thrown by __invariant() propagate to the caller
--EXTENSIONS--
invariant_php
--FILE--
<?php
class FailingInvariant {
    private int $value = 1;

    public function breakIt(): void {
        echo "break\n";
        $this->value = -1;
    }

    public function __invariant(): void {
        if ($this->value < 0) {
            throw new \LogicException("invalid invariant");
        }
    }
}

try {
    (new FailingInvariant())->breakIt();
} catch (\LogicException $e) {
    echo "caught {$e->getMessage()}\n";
}
?>
--EXPECT--
break
caught invalid invariant
