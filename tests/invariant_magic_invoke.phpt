--TEST--
Magic __invoke() triggers one post-call invariant check
--EXTENSIONS--
invariant_php
--FILE--
<?php
class InvokableInvariant {
    private int $value = 1;

    public function __invoke(): void {
        echo "invoked\n";
    }

    public function __invariant(): void {
        echo "invariant {$this->value}\n";
    }
}

(new InvokableInvariant())();
?>
--EXPECT--
invoked
invariant 1
