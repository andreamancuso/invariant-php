--TEST--
Invariant runs after constructor initializes typed properties
--EXTENSIONS--
invariant_php
--FILE--
<?php
class ConstructedAccount {
    private int $balance;

    public function __construct(int $balance) {
        $this->balance = $balance;
        echo "constructed\n";
    }

    public function __invariant(): void {
        echo "invariant {$this->balance}\n";
    }
}

new ConstructedAccount(10);
?>
--EXPECT--
constructed
invariant 10
