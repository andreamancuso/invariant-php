# Invariant PHP

**Invariant PHP** is a native PHP extension that brings [Design by Contract (DbC)](https://en.wikipedia.org/wiki/Design_by_contract) principles to PHP. It **automatically enforces class invariants** _and ensures all typed properties are initialized_ without relying on reflection.

## License

Released under the [PHP License 3.01](https://www.php.net/license/3_01.txt).

## Features

- **Class Invariants (`__invariant()`)** – Ensures an object's state remains valid after successful outermost public method calls.
- **Enforced Initialization** – Detects **uninitialized typed properties** at invariant check time and raises a **fatal error** before `__invariant()` runs.
- **Native PHP Extension** – Written in C, avoiding overhead from reflection or AOP.
- **Transparent Execution** – Define a public instance `__invariant()` method and you are good to go.

## Why?

PHP lacks built-in support for **Design by Contract**. Existing solutions (like PHPDeal) rely on **runtime reflection** or **AOP hacks**, which can be slow or brittle. **Invariant PHP** hooks directly into the **Zend Engine**.

## Example Usage
```php
<?php

class BankAccount {
    // Typed properties must be initialized or you'll get a fatal error!
    private int $balance;

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
$account->withdraw(50); // Works fine
$account->withdraw(100); // Throws LogicException if the balance becomes negative
```

## Limitations

Invariant PHP does not automatically detect whether a method changed object state. Public read-only methods may still trigger invariant checks. Destructors are skipped, and invariants are not run after methods that throw.

## Installation

Download a prebuilt NTS Linux extension from the GitHub releases page when one matches your platform:

- PHP 8.3 or 8.4
- x86_64 or arm64
- glibc or musl

Rename the downloaded artifact to `invariant_php.so`, place it in your PHP extension directory, and enable it in `php.ini`:
```ini
extension=invariant_php.so
```

Or build from source:
```sh
phpize
./configure
make -j$(nproc)
sudo make install
```
Then enable it in `php.ini`:
```ini
extension=invariant_php.so
```

## **How It Works**

- **Hooks the Zend Engine** by overriding `zend_execute_ex`.
- **Checks typed properties** for initialization at successful public boundary checks before calling `__invariant()`.
- **Throws a fatal error** if any typed property is uninitialized.
- **Automatically calls `__invariant()`** after successful outermost public method execution.
- **Runs constructor checks after initialization**, not before `__construct()`.

## **Roadmap**

- **Support `requires()` (preconditions)**.
- **Support `ensures()` (postconditions)**.
- **Optimization & caching** for better performance.
- **PECL package for easy installation**.

---

**Want to contribute?** PRs and discussions are welcome! 🎯

