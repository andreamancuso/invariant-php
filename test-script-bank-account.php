<?php

class BankAccount {
    private int $balance = 0;

    public function __construct(int $balance) {
        $this->balance = $balance;
    }

    public function withdraw(int $amount) {
        $this->balance -= $amount;
    }

    public function __invariant(): void {
        if ($this->balance < 0) {
            throw new \LogicException("Invariant failed: Balance cannot be negative.");
        }
    }
}

$account = new BankAccount(100);
$account->withdraw(50); // ✅ Works fine
$account->withdraw(100); // ❌ Throws LogicException (balance cannot be negative)