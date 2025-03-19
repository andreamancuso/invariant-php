--TEST--
Test with an Uninitialized Typed Property (Fatal Error)
--EXTENSIONS--
invariant_php
--INI--
display_errors=1
log_errors=0
error_reporting=E_ALL
--FILE--
<?php
class BankAccount {
    private int $balance; // No default value

    public function __invariant(): void {
        if ($this->balance < 0) {
            throw new \LogicException("Invariant failed: Balance cannot be negative.");
        }
    }

    public function withdraw(int $amount) {
        $this->balance -= $amount;
    }
}

$account = new BankAccount(-100);
$account->withdraw(50);
?>
--EXPECTF--
Fatal error: Invariant check failed: Uninitialized property 'balance' in class 'BankAccount' in %s on line %d
