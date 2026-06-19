--TEST--
Magic __call() triggers one post-call invariant check
--EXTENSIONS--
invariant_php
--FILE--
<?php
class MagicCallInvariant {
    private int $value = 1;

    public function __call(string $name, array $arguments): void {
        echo "__call {$name}\n";
    }

    public function __invariant(): void {
        echo "invariant {$this->value}\n";
    }
}

(new MagicCallInvariant())->missing();
?>
--EXPECT--
__call missing
invariant 1
