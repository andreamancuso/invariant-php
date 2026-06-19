--TEST--
Read-only public methods still trigger invariant checks
--EXTENSIONS--
invariant_php
--FILE--
<?php
class ReadOnlyInvariant {
    private int $value = 1;

    public function getValue(): int {
        echo "get\n";
        return $this->value;
    }

    public function __invariant(): void {
        echo "invariant {$this->value}\n";
    }
}

(new ReadOnlyInvariant())->getValue();
?>
--EXPECT--
get
invariant 1
