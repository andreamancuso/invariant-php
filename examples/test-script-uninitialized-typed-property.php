<?php
class BankAccount {
    private int $balance; // No default value

    public function __invariant(): void {
        if ($this->balance < 0) {
            throw new \LogicException("Invariant failed: Balance cannot be negative.");
        }
    }

    public function withdraw(int $amount): void {
        $this->balance -= $amount;
    }
}

$bankAccount = new BankAccount(-100);
$bankAccount->withdraw(50);
$bankAccount->withdraw(50);
$bankAccount->withdraw(50);


?>